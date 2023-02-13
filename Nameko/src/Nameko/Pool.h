#pragma once

#include <memory>
#include <cassert>
#include <iostream>

namespace Nameko {
	class BasePool {
	private:

	public:
		BasePool(size_t element_)
		virtual ~BasePool() = default;
	};

	template<typename T, size_t ChunkSize>
	class Pool : public BasePool {
	private:
		std::vector<T*> m_blocks;
		size_t m_size = 0;

	public:
		Pool() = default;
		~Pool() = default;

		size_t Size() const { return this->m_size; }

		inline void Add(T&& t) {
			auto currentSize = m_size % ChunkSize;
			auto chunkSize = m_size / ChunkSize;

			if (m_blocks.size() < chunkSize + 1) {
				std::cout << "Add Chunk" << std::endl;
				T* ptr = new T[ChunkSize];
				m_blocks.push_back(ptr);
			}

//			this->m_blocks[chunkSize]->at(currentSize) = t;

			m_size++;
		}

		inline T& Get(size_t n) {
		}
	};
}