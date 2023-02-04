#pragma once

#include <vector>
#include <array>

#include "Nameko/Chunk.h"

namespace Nameko {
	class BaseArchtype {
	public:
		virtual ~BaseArchtype() = default;
	};

	template<typename... Components>
	class Archetype : public BaseArchtype {
		using Chunks = std::vector<std::unique_ptr<Chunk<CHUNK_SIZE, Components...>>>;
		using DenseEntities = std::vector<std::unique_ptr<Chunk<CHUNK_SIZE, Entity>>>;
		using EntityToInstance = std::array<size_t, MAX_ENTITIES>;

	private:
		Chunks m_chunks;
		DenseEntities m_entities;
		EntityToInstance m_entityToInstance;

		size_t m_size = 0;
		size_t m_chunkNum = 0;

	public:
		Archetype() = default;
		~Archetype() = default;

		void AddComponents(Entity e, Components&&... components) {
			m_chunkNum = m_size / CHUNK_SIZE;
			if (m_chunks.size() <= m_chunkNum) {
				std::cout << "Add Chunk" << std::endl;
				auto componentChunk = std::make_unique<Chunk<CHUNK_SIZE, Components...>>();
				m_chunks.emplace_back(std::move(componentChunk));
				
				auto entityChunk = std::make_unique<Chunk<CHUNK_SIZE, Entity>>();
				m_entities.emplace_back(std::move(entityChunk));
			}

			m_entityToInstance[e] = m_size;
			m_chunks[m_chunkNum]->Add(std::forward<Components>(components)...);
			m_entities[m_chunkNum]->Add(std::forward<Entity>(e));
			m_size++;
		}

		template<typename T>
		T* GetComponent(Entity e) {
			auto size = m_entityToInstance[e];
			return m_chunks[size / CHUNK_SIZE]->Get<T>(size % CHUNK_SIZE);
		}

		std::tuple<Components*...> GetComponents(Entity e) {
			return { this->GetComponent<Components>(e)... };
		}

		void RemoveComponents(Entity e) {
			auto n = m_entityToInstance[e];
			auto chunk_size = n / CHUNK_SIZE;
			auto size = n % CHUNK_SIZE;
			auto current_chunk_size = (m_size - 1) / CHUNK_SIZE;
			auto current_size = (m_size - 1) % CHUNK_SIZE;

			auto components = m_chunks[current_chunk_size]->GetAll(current_size);
			std::apply(&Chunk<CHUNK_SIZE, Components...>::Replace, std::tuple_cat(std::make_tuple(m_chunks[chunk_size].get(), size), components));
			m_chunks[current_chunk_size]->Remove(current_size);

			auto entity = m_entities[current_chunk_size]->Get<Entity>(current_size);
			m_entities[chunk_size]->Replace(size, entity);
			m_entities[current_chunk_size]->Remove(current_size);

			m_entityToInstance[*entity] = m_entityToInstance[e];

			m_size--;
		}

		template<typename... Targets>
		void IterateAll(const std::function<void(Targets&...)>&& lambda) {
			for (auto& chunk : m_chunks) {
				chunk->IterateAll<Targets...>(lambda);
			}
		}

	};
}