#pragma once

#include <vector>
#include <array>

#include "Nameko/Chunk.h"

namespace Nameko {
	class Archetype {
		using Chunk = std::vector<std::unique_ptr<Pool>>;

	private:
		std::vector<Chunk> m_chunks;
		std::vector<std::pair<FamilyID, size_t>> m_families;
		std::array<uint32_t, MAX_COMPONENTS> m_familyToPool;
		std::array<size_t, MAX_ENTITIES> m_entityToInstance;

		size_t m_size;
		size_t m_chunkSize;

	public:
		Archetype() = default;
		~Archetype() = default;

		void ResistFamily(FamilyID id, size_t size) {
			static unsigned int count = 0;
			m_families.emplace_back(std::make_pair(id, size));
			m_familyToPool[id] = count;
			count++;
		}

		template<typename... Components>
		void CreateInstance(Entity e, Components&&... components) {
			m_chunkSize = m_size / CHUNK_SIZE;
			if (m_chunks.size() <= m_chunkSize) {
				std::cout << "Add Chunk" << std::endl;
				Chunk chunk;
				(chunk.emplace_back(std::move(std::make_unique<Pool>(sizeof(Components), CHUNK_SIZE))), ...);
				(m_families.emplace_back(std::make_pair(IdGenerator::GetFamily<Components>(), sizeof(Components))), ...);
				unsigned int count = 0;
				((m_familyToPool[IdGenerator::GetFamily<Components>()] = count++), ...);
				this->m_chunks.emplace_back(std::move(chunk));
			}

			m_entityToInstance[e] = m_size;
			(::new(m_chunks[m_chunkSize][m_familyToPool[IdGenerator::GetFamily<Components>()]]->get(m_size % CHUNK_SIZE)) Components(std::forward<Components>(components)), ...);

			m_size++;
		}

		template<typename
		void* RetrieveInstance(Entity e, FamilyID family) {
			auto size = m_entityToInstance[e];
			return m_chunks[size / CHUNK_SIZE][m_familyToPool[family]]->get(size % CHUNK_SIZE);
		}
	};
}