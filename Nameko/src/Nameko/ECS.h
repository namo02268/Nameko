#pragma once

#include <unordered_map>
#include <array>

#include "Nameko/Archtype.h" 
#include "Nameko/IdGenerator.h"

namespace Nameko {
	class ECS {
	private:
		std::unordered_map<ArcheID, std::unique_ptr<BaseArchtype>> m_archetypes;
		std::array<ArcheID, MAX_ENTITIES> m_entityToArche;

	public:
		ECS() = default;
		~ECS() = default;

		template<typename... Components>
		void AddComponents(Entity e, Components&&... components) {
			auto entityArcheID = m_entityToArche[e];
			auto componentArcheID = IdGenerator::GetArche<Components...>();
			auto archeID = entityArcheID | componentArcheID;
			
			if (entityArcheID != 0) {
				
			}

			if (!m_archetypes.contains(archeID)) {
				m_archetypes.emplace(archeID, std::make_unique<Archetype<std::remove_reference_t<decltype(components)>...>>());
			}
			static_cast<Archetype<std::remove_reference_t<decltype(components)>...>*>(this->m_archetypes[archeID].get())->AddComponents(e, std::forward<Components>(components)...);
		}
	};
}