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
			this->m_entityPool = new Pool<Entity, CHUNK_SIZE>;
		}

		~Archetype() {
			std::cout << "Delete Pools" << std::endl;
			for (auto pool : m_componentPools) {
				delete pool;
			}
			delete m_entityPool;
		}

		template<typename Component>
		void AddComponent(Entity e, Component&& component, Archetype* other) {
			if (other != nullptr) {
				for (auto& pair : other->m_familyToPool) {
					if (!this->m_familyToPool.contains(pair.first)) {
						std::cout << "Add Component Pool from other" << std::endl;
						this->m_componentPools.emplace_back(other->m_componentPools[m_familyToPool[pair.second]]->createPool());
						this->m_familyToPool[pair.first] = this->m_componentPools.size() - 1;
					}
					this->m_componentPools[this->m_familyToPool[pair.second]]->add(other->m_componentPools[m_familyToPool[pair.second]]->get(other->m_entityToInstance[e]));
					other->RemoveComponent(e);
				}
			}

			auto family = IdGenerator::GetFamily<Component>();
			if (!this->m_familyToPool.contains(family)) {
				std::cout << "Add Component Pool" << std::endl;
				this->m_componentPools.emplace_back(new Pool<Component, CHUNK_SIZE>);
				this->m_familyToPool[family] = this->m_componentPools.size() - 1;
			}
			static_cast<Pool<Component, CHUNK_SIZE>*>(this->m_componentPools[this->m_familyToPool[family]])->add(component);

			m_entityPool->add(e);
			m_entityToInstance[e] = m_size;
			m_size++;
		}

		template<typename Component>
		Component* GetComponent(Entity e) {
			auto family = IdGenerator::GetFamily<Component>();
			auto n = m_entityToInstance[e];
			return static_cast<Component*>(this->m_componentPools[m_familyToPool[family]]->get(n));
		}

		void RemoveComponent(Entity e) {
			for (auto& pool : m_componentPools) {
				pool->destroy(m_entityToInstance[e]);
			}
			m_entityToInstance[*static_cast<Entity*>(m_entityPool->get(m_size - 1))] = m_entityToInstance[e];
			m_entityPool->destroy(m_entityToInstance[e]);
			m_size--;
		}
	};
}