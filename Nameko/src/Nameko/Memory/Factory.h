#pragma once

#include "MemoryBlock.h"

namespace Nameko {
	template<typename... Types>
	class Factory {
	private:
		std::vector<MemoryBlock<Types...>*> memoryBlocks;
		size_t index = 0;

	public:
		Factory() {
			memoryBlocks.push_back(new MemoryBlock<Types...>(5));
		}
		~Factory() = default;

		/*
		template<typename... Args>
		void AddComponents(Args&&... args) {
			(AddComponent<Types>(std::forward<Args>(args)), ...);
			nextIndex++;
		}
		*/

		template<typename Type>
		void Create(Type&& component) {
			// placement new : あらかじめ確保された領域内にオブジェクトを構築
			new (memoryBlocks[0]->Get<Type>(index)) Type(std::forward<Type>(component));
		}

		template<typename Type>
		Type* Get(size_t index) {
		}

		template<typename Type>
		void Destroy() {

		}

	private:
	};
}
