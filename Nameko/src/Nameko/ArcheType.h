#pragma once

#include "IDGenerator.h"
#include "Memory/MemoryBlock.h"
#include "Nameko/Type.h"
#include <array>
#include <vector>

namespace Nameko {
	template<typename... Types>
	class ArcheType;

	template<typename... Types>
	using ArcheTypeI = TypeUtils::instantiate_t<ArcheType, TypeUtils::sorted_list_t<TypeUtils::list<Types...>>>;

	class ArcheTypeBase {
	public:
		ArcheTypeBase() = default;
		virtual ~ArcheTypeBase() = default;
	};

	template<typename... Types>
	class ArcheType : public ArcheTypeBase {
	public:
		std::vector<MemoryBlock<Types...>*> memoryBlocks;
		ArcheID m_id;

	public:
		ArcheType() {
			memoryBlocks.push_back(new MemoryBlock<Types...>(4));
		}

		~ArcheType() {
			delete memoryBlocks[0];
		}

		template<typename T>
		void AddComponent(ArcheTypeBase* other, T&& t) {
			auto* srcMem = this->memoryBlocks[0];
			auto* dstMem = reinterpret_cast<ArcheTypeI<T, Types...>*>(other)->memoryBlocks[0];
			dstMem->Create(std::forward<T>(t));
			(Move<Types>(srcMem, dstMem, 2), ...);
		}

		template<typename T>
		void DestroyComponent(ArcheTypeBase* other) {

		}
	};

	class ArcheManeger {
	public:
		template<typename T, typename... Types>
		static void AddComponent(ArcheTypeBase* src, ArcheTypeBase* dst, T&& t) {
			auto* srcMemoryBlock = reinterpret_cast<ArcheType<Types...>*>(src)->memoryBlocks[0];
			auto* dstMemoryBlock = reinterpret_cast<ArcheType<T, Types...>*>(dst)->memoryBlocks[0];
			dstMemoryBlock->Create(std::forward<T>(t));
			(Move<Types>(srcMemoryBlock, dstMemoryBlock), ...);
		}

		template<typename T>
		static void RemoveComponent() {

		}
	};
}
