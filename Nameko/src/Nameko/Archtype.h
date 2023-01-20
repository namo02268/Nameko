#pragma once

#include <vector>
#include <array>

#include "Nameko/Chunk.h"

namespace Nameko {
	class BaseArchtype {
		virtual ~BaseArchtype() = default;
	};

	template<typename... Types>
	class Archtype {
		using Chunks = std::vector<std::unique_ptr<Chunk<CHUNK_SIZE, Types...>>>;
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

		void AddComponents(Entity& e, Types&&... components) {
			m_chunkNum = m_size / CHUNK_SIZE;
			if (m_chunks.size() <= m_chunkNum) {
				std::cout << "Add Chunk" << std::endl;
				auto componentChunk = std::make_unique<Chunk<CHUNK_SIZE, Types...>>();
				m_chunks.emplace_back(std::move(componentChunk));
				
				auto entityChunk = std::make_unique<Chunk<CHUNK_SIZE, Entity>>();
				m_entities.emplace_back(std::move(entityChunk));
			}

			m_entityToInstance[e] = m_size;
			m_chunks[m_chunkNum]->Add(std::forward<Types>(components)...);
			m_entities[m_chunkNum]->Add(std::forward<Entity>(e));
			m_size++;
		}

		template<typename T>
		T& GetComponent(Entity e) {
			auto size = m_entityToInstance[e];
			return m_chunks[size / CHUNK_SIZE]->Get<T>(size % CHUNK_SIZE);
		}

		void RemoveComponents(Entity e) {
			/*
			auto size = m_entityToInstance[e];
			m_chunks[size / CHUNK_SIZE]->Remove(size % CHUNK_SIZE);
			m_size--;
			*/
		}
	};
}