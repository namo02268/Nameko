#pragma once

#include <unordered_map>
#include <array>

#include "Nameko/Archtype.h" 
#include "Nameko/IdGenerator.h"

namespace Nameko {
	class ECS {
	private:
		std::unordered_map<ArcheID, BaseArchtype*> m_archeTypes;
		std::array<ArcheID, MAX_ENTITIES> m_entityToArche;

	public:
		ECS() = default;
		~ECS() = default;

		template<typename... Components>
		void AddComponents(Entity e, Components&&... components) {
			auto entityArcheID = m_entityToArche(e);
			auto componentArcheID = IdGenerator::GetArche<components...>();

			if (entityArcheID != 0) {

			}
			else {
				std::cout << entityArcheID << std::endl;
				std::cout << componentArcheID << std::endl;
			}
		}
	};
}