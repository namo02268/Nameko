#pragma once

#include <memory>
#include <cassert>
#include <tuple>
#include <iterator>
#include <functional>

#include "Nameko/Config.h"
#include "Nameko/Entity.h"

#include <iostream>

namespace Nameko {
	class IBlock {
	public:
		~IBlock() = default;
	};

	template<typename T, size_t MaxElement = 512>
	class Block : public IBlock {
	private:
		void* m_ptr;
		size_t m_maxElement;
		size_t m_typeSize;
		size_t m_totalSize;
		size_t m_size;

	public:
		Block() : m_typeSize(sizeof(T)), m_maxElement(MaxElement) {
			this->m_totalSize = m_typeSize * m_maxElement;
			this->m_size = 0;
			this->m_ptr = malloc(m_totalSize);
			std::cout << "Total Size : " << m_totalSize << "[bytes]" << std::endl;
		}
		Block(const Block&) = delete;
		Block& operator=(const Block&) = delete;
		~Block() {
			free(this->m_ptr);
		}

		T& operator[](const int n) { return *this->get(n); }
		size_t size() const { return this->m_size; }
		size_t getMaxElement() const { return this->m_maxElement; }
		size_t getTypeSize() const { return this->m_typeSize; }
		size_t getTotalSize() const { return this->m_totalSize; }

		template<typename Component>
		inline T* create(Component&& component) {
			auto ptr = reinterpret_cast<T*>(this->m_ptr) + m_size;
			::new(ptr) T(std::move(component));
			this->m_size++;
			return ptr;
		}

		inline void destroy(size_t n) {
			if (n != this->m_size - 1) {
				this->at(n) = this->at(this->m_size - 1);
			}
			this->m_size--;
		}

		inline T* data() {
			return this->get(0);
		}

		inline T& at(const size_t n) {
			return *this->get(n);
		}

		inline void swap(const size_t a, const size_t b) {
			T tmp = this->at(a);
			this->at(a) = this->at(b);
			this->at(b) = tmp;
		}

		T* get(size_t n) {
			assert(
				m_size > n &&
				m_maxElement > n &&
				"n must be smaller than the block size or the current size."
			);
			return static_cast<T*>(this->m_ptr) + n;
		}
	};

	template<typename... Components>
	class Chunk {
		template<typename T>
		using BlockType = Block<T, CHUNK_SIZE>;
		using ComponentBlocks = std::tuple<BlockType<Components>...>;

		template<typename T, typename Tuple>
		struct has_type;
		template<typename T, typename... Ts>
		struct has_type<T, std::tuple<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

	private:
		ComponentBlocks m_blocks;
		size_t m_size = 0;
		const size_t ComponentCount = sizeof...(Components);

	public:
		Chunk() = default;
		~Chunk() = default;

		inline void AddComponents(Components&... components) {
			(this->GetBlock<Components>()->create(std::move(components)), ...);
			m_size++;
		}

		template<typename Component>
		inline Component& GetComponent(size_t n) {
			return this->GetBlock<Component>()->at(n);
		}

		inline void RemoveComponents(size_t n) {
			(this->GetBlock<Components>()->destroy(n), ...);
			m_size--;
		}

		template<typename... Targets>
		void IterateAll(const std::function<void(Targets&...)> lambda) {
			for (size_t i = 0; i < m_size; ++i) {
				lambda(this->GetComponent<Targets>(i)...);
			}
		}
		
	private:
		template<typename Component>
		inline BlockType<Component>* GetBlock() {
			static_assert(has_type<BlockType<Component>, std::tuple<BlockType<Components>...>>::value, "Component is not contained in this Chunk.");
			return &std::get<BlockType<Component>>(this->m_blocks);
		}
	};

	/*
	template<typename... Components>
	class Chunk {
		template<typename T>
		using BlockType = Block<T, CHUNK_SIZE>;
		using EntityBlock = BlockType<Entity>;

	private:
		std::vector<std::unique_ptr<IBlock>> m_blocks;
		EntityBlock m_entities;
		const size_t ComponentCount = sizeof...(Components);

	public:
		Chunk() {
			(m_blocks.push_back(std::make_unique<BlockType<Components>>()), ...);
		}
		~Chunk() = default;

		inline void AddComponents(Components&... components) {
			std::array<std::reference_wrapper<Components>, sizeof...(Components)> components_array
				= { components... };
			for (size_t i = 0; i < ComponentCount; i++) {
				auto block = static_cast<BlockType<Components>*>(m_blocks[i].GetBlock());
				if (block) {
					block->create(components_array[i]);
				}
			}
		}
	};
	*/
}