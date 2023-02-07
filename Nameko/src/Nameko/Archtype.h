#pragma once

#include <vector>
#include <array>

#include "Nameko/Chunk.h"

namespace Nameko {
	class Archetype {
		using Chunk = std::vector<std::unique_ptr<Pool>>;

	private:
		std::vector<Chunk> m_chunks;
		Chunk m_entities;
		std::vector<std::pair<FamilyID, size_t>> m_families;
		std::array<uint32_t, MAX_COMPONENTS> m_familyToPool;
		std::array<size_t, MAX_ENTITIES> m_entityToInstance;

		size_t m_size;
		size_t m_chunkSize;

	public:
		Archetype() = default;
		~Archetype() = default;

		template<typename... Components>
		void AddComponents(Entity e, Components&&... components) {
			m_chunkSize = m_size / CHUNK_SIZE;
			if (m_chunks.size() <= m_chunkSize) {
				std::cout << "Add Chunk" << std::endl;
				Chunk chunk;
				(chunk.emplace_back(std::make_unique<Pool>(sizeof(Components), CHUNK_SIZE)), ...);
				(m_families.emplace_back(std::make_pair(IdGenerator::GetFamily<Components>(), sizeof(Components))), ...);
				unsigned int count = 0;
				((m_familyToPool[IdGenerator::GetFamily<Components>()] = count++), ...);
				this->m_chunks.emplace_back(std::move(chunk));
				this->m_entities.emplace_back(std::make_unique<Pool>(sizeof(Entity), CHUNK_SIZE));
			}

			m_entityToInstance[e] = m_size;
			(::new(m_chunks[m_chunkSize][m_familyToPool[IdGenerator::GetFamily<Components>()]]->get(m_size % CHUNK_SIZE)) Components(std::forward<Components>(components)), ...);
			::new(this->m_entities[m_chunkSize]->get(m_size % CHUNK_SIZE)) Entity(e);
			m_size++;
		}

		template<typename Component>
		Component* GetComponent(Entity e) {
			auto size = m_entityToInstance[e];
			return static_cast<Component*>(m_chunks[size / CHUNK_SIZE][m_familyToPool[IdGenerator::GetFamily<Component>()]]->get(size % CHUNK_SIZE));
		}

		void RemoveComponents(Entity e) {
			auto n = m_entityToInstance[e];
			auto chunk_size = n / CHUNK_SIZE;
			auto size = n % CHUNK_SIZE;
			auto current_chunk_size = (m_size - 1) / CHUNK_SIZE;
			auto current_size = (m_size - 1) % CHUNK_SIZE;

			for (auto& family : m_families) {
				auto dst = m_chunks[chunk_size][m_familyToPool[family.first]]->get(size);
				auto src = m_chunks[current_chunk_size][m_familyToPool[family.first]]->get(current_size);
				std::memcpy(dst, src, family.second);
			}

			auto dst = m_entities[chunk_size]->get(size);
			auto src = m_entities[current_chunk_size]->get(current_size);
			std::memcpy(dst, src, sizeof(Entity));

			m_entityToInstance[*static_cast<Entity*>(src)] = m_entityToInstance[e];

			m_size--;
		}
	};
}