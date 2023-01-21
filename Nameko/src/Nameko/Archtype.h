#pragma once

#include <vector>
#include <array>

#include "Nameko/Chunk.h"

namespace Nameko {
	class BaseArchtype {
		virtual ~BaseArchtype() = default;
	};

	template<typename... Components>
	class Archtype {
		using Chunks = std::vector<std::unique_ptr<Chunk<CHUNK_SIZE, Components...>>>;
		using DenseEntities = std::vector<std::unique_ptr<Chunk<CHUNK_SIZE, Entity>>>;
		using EntityToInstance = std::array<size_t, MAX_ENTITIES>;

	private:
		Chunks m_chunks;
		DenseEntities m_entities;
		EntityToInstance m_entityToInstance;

		ArcheID m_id = 0;
		size_t m_size = 0;
		size_t m_chunkNum = 0;

	public:
		Archtype() = default;
		~Archtype() = default;

		void AddComponents(Entity& e, Components&&... components) {
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
		T& GetComponent(Entity e) {
			auto size = m_entityToInstance[e];
			return m_chunks[size / CHUNK_SIZE]->Get<T>(size % CHUNK_SIZE);
		}

		void RemoveComponents(Entity e) {
			auto n = m_entityToInstance[e];
			auto chunk_size = n / CHUNK_SIZE;
			auto size = n % CHUNK_SIZE;

			auto current_chunk_size = m_size / CHUNK_SIZE;
			auto current_size = m_size % CHUNK_SIZE;

			m_chunks[chunk_size]->Replace((m_chunks[current_chunk_size]->Get<Components>(current_size), ...));

//			m_chunks[chunk_size]->Remove(size);

			m_size--;
		}
	};
}