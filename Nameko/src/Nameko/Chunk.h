#pragma once

#include <memory>
#include <cassert>
#include <iostream>
#include <iterator>
#include <functional>

#include "Nameko/Config.h"
#include "Nameko/Entity.h"
#include "Nameko/IdGenerator.h"


#include <typeinfo>
namespace Nameko {
	class Block {
	private:
		char* m_ptr;
		size_t m_size;
		size_t m_elementSize;
		size_t m_maxElement;
		size_t m_totalSize;

	public:
		Block(size_t elementSize, size_t maxElement) : m_elementSize(elementSize), m_maxElement(maxElement) {
			this->m_totalSize = m_elementSize * m_maxElement;
			this->m_size = 0;
			this->m_ptr = new char[m_totalSize];
			std::cout << "Total Size : " << m_totalSize << "[bytes]" << std::endl;
		}

		Block(const Block&) = delete;
		Block& operator=(const Block&) = delete;
		~Block() {
			delete[] m_ptr;
		}

		size_t size() const { return this->m_size; }
		size_t elementSize() const { return this->m_elementSize; }
		size_t maxElement() const { return this->m_maxElement; }
		size_t totalSize() const { return this->m_totalSize; }

		inline void* get(size_t n) {
			assert(
				n < this->m_size &&
				n < this->m_maxElement &&
				"n must be smaller than the block size or the current size."
			);
			return this->m_ptr + n * this->m_elementSize; 
		}
	};
}