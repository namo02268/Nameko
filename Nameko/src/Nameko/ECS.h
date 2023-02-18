#pragma once

#include <unordered_map>
#include <array>

#include "Nameko/Archtype.h" 
#include "Nameko/IdGenerator.h"

namespace Nameko {
	class ECS {
	private:
		std::unordered_map<ArcheID, Archetype*>  m_archetypes;
		std::array<ArcheID, MAX_ENTITIES> m_entityToArche;

	public:
		ECS() = default;
		~ECS() {
			for (auto& pair : m_archetypes) {
				delete pair.second;
			}
		}

		template<typename Component>
		void AddComponent(Entity e, Component&& component) {
			auto oldArcheID = m_entityToArche[e];
			auto newArcheID = oldArcheID | IdGenerator::GetArche<Component>();

			if (!m_archetypes.contains(newArcheID)) {
				m_archetypes[newArcheID] = new Archetype;
			}

			if (oldArcheID) {
				m_archetypes[newArcheID]->AddComponent<Component>(e, std::forward<Component>(component), m_archetypes[oldArcheID]);
			}
			else {
				m_archetypes[newArcheID]->AddComponent<Component>(e, std::forward<Component>(component), nullptr);
			}

			this->m_entityToArche[e] = newArcheID;
		}

		template<typename Component>
		Component* GetComponent(Entity e) {
			auto archeID = m_entityToArche[e];
			return m_archetypes[archeID]->GetComponent<Component>(e);
		}
	};
}