#pragma once

#include <vector>
#include <array>
#include <unordered_map>
#include <functional>

#include "Nameko/Pool.h"

namespace Nameko {
	class Archetype {
	private:
		std::vector<BasePool*> m_componentPools;
		Pool<Entity, CHUNK_SIZE>* m_entityPool;
		std::unordered_map<FamilyID, size_t> m_familyToPool;
		std::array<size_t, MAX_ENTITIES> m_entityToInstance;

		size_t m_size = 0;

	public:
		Archetype() {
			m_entityPool = new Pool<Entity, CHUNK_SIZE>;
		}

		~Archetype() {
			std::cout << "Delete Pools" << std::endl;
			for (auto pool : m_componentPools) {
				delete pool;
			}
			delete m_entityPool;
		}

		template<typename Component>
		void AddComponent(Entity entity, Component&& component, Archetype* other) {
			if (other != nullptr) {
				for (const auto& [familyID, poolID] : other->m_familyToPool) {
					if (!this->m_familyToPool.contains(familyID)) {
						std::cout << "Add Component Pool from other" << std::endl;
						this->m_componentPools.emplace_back(other->m_componentPools[poolID]->createPool());
						this->m_familyToPool[familyID] = this->m_componentPools.size() - 1;
					}
					this->m_componentPools[this->m_familyToPool[familyID]]->add(other->m_componentPools[poolID]->get(other->m_entityToInstance[entity]));
				}
				other->RemoveComponents(entity);
			}

			auto family = IdGenerator::GetFamily<Component>();
			if (!this->m_familyToPool.contains(family)) {
				std::cout << "Add Component Pool" << std::endl;
				this->m_componentPools.emplace_back(new Pool<Component, CHUNK_SIZE>);
				this->m_familyToPool[family] = this->m_componentPools.size() - 1;
			}
			static_cast<Pool<Component, CHUNK_SIZE>*>(this->m_componentPools[this->m_familyToPool[family]])->add(component);

			this->m_entityPool->add(entity);
			this->m_entityToInstance[entity] = this->m_size;
			this->m_size++;
		}


		void DeplicateEntity(Entity src, Entity dst) {
			for (auto& pool : m_componentPools) {
				pool->add(pool->get(this->m_entityToInstance[src]));
			}

			this->m_entityPool->add(dst);
			this->m_entityToInstance[dst] = this->m_size;
			this->m_size++;
		}

		template<typename Component>
		Component* GetComponent(Entity entity) {
			auto family = IdGenerator::GetFamily<Component>();
			auto n = m_entityToInstance[entity];
			return static_cast<Component*>(m_componentPools[m_familyToPool[family]]->get(n));
		}

		template<typename Component>
		void RemoveComponent(Entity entity, Archetype* other) {
			auto removeID = IdGenerator::GetFamily<Component>();
			for (const auto& [familyID, poolID] : this->m_familyToPool) {
				if (familyID != removeID) {
					if (!other->m_familyToPool.contains(familyID)) {
						std::cout << "Add Component Pool from other" << std::endl;
						other->m_componentPools.emplace_back(this->m_componentPools[poolID]->createPool());
						other->m_familyToPool[familyID] = other->m_componentPools.size() - 1;
					}
					other->m_componentPools[other->m_familyToPool[familyID]]->add(this->m_componentPools[poolID]->get(this->m_entityToInstance[entity]));
				}
			}
			other->m_entityPool->add(entity);
			other->m_entityToInstance[entity] = other->m_size;
			other->m_size++;

			this->RemoveComponents(entity);
		}

		void RemoveComponents(Entity entity) {
			for (auto& pool : m_componentPools) {
				pool->destroy(m_entityToInstance[entity]);
			}
			m_entityToInstance[*static_cast<Entity*>(m_entityPool->get(m_size - 1))] = m_entityToInstance[entity];
			m_entityPool->destroy(m_entityToInstance[entity]);
			m_size--;
		}

		template<typename... Components>
		void EachComponent(const std::function<void(Components&...)>&& lambda) {
			const size_t chunkSize = m_size / CHUNK_SIZE;
			for (size_t i = 0; i < chunkSize; ++i) {
				this->IterateComponent(lambda, CHUNK_SIZE, static_cast<Components*>(m_componentPools[m_familyToPool[IdGenerator::GetFamily<Components>()]]->get(CHUNK_SIZE * i))...);
			}
	
			const size_t currentSize = m_size - chunkSize * CHUNK_SIZE;
			if (currentSize != 0) {
				this->IterateComponent(lambda, currentSize, static_cast<Components*>(m_componentPools[m_familyToPool[IdGenerator::GetFamily<Components>()]]->get(CHUNK_SIZE * chunkSize))...);
			}
		}

		void EachEntity(const std::function<void(Entity&)>& lambda) {
			const size_t chunkSize = m_size / CHUNK_SIZE;
			for (size_t i = 0; i < chunkSize; ++i) {
				auto ptr = static_cast<Entity*>(m_entityPool->get(CHUNK_SIZE * i));
				for (size_t j = 0; j < CHUNK_SIZE; ++j) {
					lambda(ptr[j]);
				}
			}

			const size_t currentSize = m_size - chunkSize * CHUNK_SIZE;
			if (currentSize != 0) {
				auto ptr = static_cast<Entity*>(m_entityPool->get(CHUNK_SIZE * chunkSize));
				for (size_t j = 0; j < currentSize; ++j) {
					lambda(ptr[j]);
				}
			}
		}

	private:
		template<typename... Components, typename... ComponentArray>
		inline void IterateComponent(const std::function<void(Components&...)>& lambda, const size_t size, ComponentArray*... array) {
			for (size_t i = 0; i < size; ++i) {
				lambda(array[i]...);
			}
		}
	};
}