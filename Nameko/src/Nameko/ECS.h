#pragma once

#include <unordered_map>
#include <array>

#include "Nameko/Archtype.h" 
#include "Nameko/IdGenerator.h"

namespace Nameko {
	class ECS {
	private:
		std::unordered_map<ArcheID, std::unique_ptr<Archetype>> m_archetypes;
		std::array<size_t, MAX_ENTITIES> m_entityToArche;

	public:
		ECS() = default;
		~ECS() = default;

		template<typename... Components>
		void AddComponents(Entity e, Components&&... components) {
			auto archeId = IdGenerator::GetArche<Components...>();

			if (m_entityToArche[e]) {
				auto newArcheId = archeId | m_entityToArche[e];
				std::cout << newArcheId << std::endl;
				auto arche = std::make_unique<Archetype>();
				m_archetypes[m_entityToArche[e]]->Move<Components...>(arche.get(), e);
				m_archetypes[newArcheId] = std::move(arche);
			}
			else {
				if (!m_archetypes.contains(archeId)) {
					m_archetypes[archeId] = std::make_unique<Archetype>();
				}
				m_archetypes[archeId]->AddComponents(e, std::forward<Components>(components)...);
				m_entityToArche[e] = archeId;
				std::cout << archeId << std::endl;
			}
		}
	};
}