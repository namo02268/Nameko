#pragma once

#include "IDGenerator.h"]
#include "ComponentManager.h"
#include <array>

namespace Nameko {
	template<typename... Types>
	class ArcheType {
	private:
		static std::array<BaseComponentManager*, MAX_FAMILY> m_cManager;
		ArcheID m_id;
		size_t currentIndex = 0;

	public:
		ArcheType() {
		}

		void AddComponent() {

		}

		void AllocatePool() {
			size_t m_typeSizes[sizeof...(Types)]{ sizeof(Types)... };

			size_t index = 0;
			((m_familyToPool[IDGenerator::GetFamilyCount<Types>()] = index++), ...);


			std::cout << "Sizes: ";
			for (size_t size : m_typeSizes) {
				std::cout << size << " ";
			}
			std::cout << std::endl;
		}

		void FreePool() {

		}

	};
}
