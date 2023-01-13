#pragma once

#include "Nameko/Config.h"
#include "Nameko/Entity.h"

#include <memory>
#include <cassert>
#include <vector>
#include <tuple>

#include <iostream>

namespace Nameko {
	class IBlock {
	public:
		~IBlock() = default;
	};

	template<typename T, size_t blockSize = 512>
	class Block : public IBlock {
	private:
		void* m_ptr;
		size_t m_blockSize;
		size_t m_elementSize;
		size_t m_totalSize;
		size_t m_size;

	public:
		Block() : m_elementSize(sizeof(T)), m_blockSize(blockSize) {
			this->m_totalSize = m_elementSize * m_blockSize;
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
			assert(m_size > n && "n must be smaller than block size.");
			return reinterpret_cast<T*>(this->m_ptr) + n;
		}
	};


	template<typename... Components>
	class Chunk {
		template<typename T>
		using BlockType = Block<T, CHUNK_SIZE>;
		using ComponentBlocks = std::tuple<BlockType<Components>...>;
		using EntityBlock = BlockType<Entity>;

		template<typename T, typename Tuple>
		struct has_type;
		template<typename T, typename... Ts>
		struct has_type<T, std::tuple<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

	private:
		ComponentBlocks m_blocks;
		EntityBlock m_entities;
		const size_t ComponentCount = sizeof...(Components);

	public:
		Chunk() = default;
		~Chunk() = default;

		template<typename Component>
		inline void AddComponent(Component& component) {
			this->get<Component>()->create(std::move(component));
		}

		inline void AddComponents(Components&... components) {
			(this->AddComponent(components), ...);
		}

		template<typename Component>
		inline BlockType<Component>* get() {
			static_assert(has_type<BlockType<Component>, std::tuple<BlockType<Components>...>>::value, "Component is not contained in this Chunk.");
			return &std::get<BlockType<Component>>(this->m_blocks);
		}

		/*
		template<size_t i>
		inline typename std::tuple_element<i, ComponentBlocks>::type* get() {
			return &std::get<i>(this->m_blocks);
		}

		inline void AddComponents(Components&... components) {
			for (size_t i = 0; i < ComponentCount; i++) {
				(this->get<i>()->create(components), ...);
			}
		}
		*/

	};
}