#pragma once

#include <memory>
#include <cassert>
#include <iostream>

#include <typeinfo>
namespace Nameko {
	class Pool {
	private:
		char* m_ptr;
		size_t m_elementSize;
		size_t m_maxElement;
		size_t m_totalSize;

	public:
		Pool(size_t elementSize, size_t maxElement) : m_elementSize(elementSize), m_maxElement(maxElement) {
			this->m_totalSize = m_elementSize * m_maxElement;
			this->m_ptr = new char[m_totalSize];
			std::cout << "Total Size : " << m_totalSize << "[bytes]" << std::endl;
		}

		Pool(const Pool&) = delete;
		Pool& operator=(const Pool&) = delete;
		~Pool() {
			delete[] m_ptr;
		}

		size_t elementSize() const { return this->m_elementSize; }
		size_t maxElement() const { return this->m_maxElement; }
		size_t totalSize() const { return this->m_totalSize; }

		inline void* get(size_t n) {
			return this->m_ptr + n * this->m_elementSize; 
		}
	};
}