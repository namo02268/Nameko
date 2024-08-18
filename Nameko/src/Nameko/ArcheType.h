#pragma once

#include "IDGenerator.h"
#include <array>

namespace Nameko {
	template<typename... Types>
	class ArcheType {
	private:
//		ArcheID m_id;
		size_t currentIndex = 0;

	public:
		ArcheType() {
		}

		void AddComponents(Types... types) {
			
		}

		void FreePool() {

		}
	};

	class ArcheTypeManager {

	};
}
