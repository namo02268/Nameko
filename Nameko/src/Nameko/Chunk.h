#pragma once

#include "Nameko/Config.h"

#include <memory>
#include <cassert>
#include <vector>
#include <tuple>

#include <iostream>

namespace Nameko {
	template<typename T, size_t blockSize = 1024>
	class Block {
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
			this->clean();
			free(this->m_ptr);
		}

		T& operator[](const int n) { return *this->get(n); }

		template<typename... Args>
		inline T* create(Args && ... args) {
			auto ptr = reinterpret_cast<T*>(this->m_ptr) + m_size;
			::new(ptr) T(std::forward<Args>(args) ...);
			this->m_size++;
			return ptr;
		}

		inline void destroy(size_t n) {
			this->at(n).~T();
			if (n != this->m_size - 1) {
				this->at(n) = this->at(this->m_size - 1);
			}
			this->m_size--;
		}

		inline void clean() {
			for (int i = 0; i < this->m_size; i++) {
				this->at(i).~T();
			}
			this->m_size = 0;
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
		using ComponentBlocks = std::tuple<Block<Components, MAX_COMPONENTS>...>;

	private:
		ComponentBlocks m_blocks;

		const size_t ComponentCount = sizeof...(Components);

	public:
		Chunk() = default;
		~Chunk() = default;

		template<>
		void AddComponent()

		template<typename Component>
		inline Block<Component, MAX_COMPONENTS>* get() {
			return &std::get<Block<Component, MAX_COMPONENTS>>(this->m_blocks);
		}
	};
}