#pragma once

#include "IDGenerator.h"
#include "MemoryBlock.h"
#include <array>

namespace Nameko {
	template<typename... Types>
	class ArcheType {
	private:
//		ArcheID m_id;
		MemoryBlock<Types...> memoryBlock;
		size_t currentIndex = 0;

	public:
		ArcheType() {
			memoryBlock(5);
		}

		void AddComponents(Types... types) {
			
		}

		void FreePool() {

		}
	};

	class ArcheTypeManager {

	};
}
