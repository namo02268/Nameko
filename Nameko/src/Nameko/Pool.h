#pragma once

#include <memory>
#include <cassert>
#include <iostream>

namespace Nameko {
	class PoolBase {
	protected:
		size_t m_size;

	private:
		char* m_ptr;
		size_t m_elementSize;
		size_t m_maxElement;
		size_t m_totalSize;

	public:
		PoolBase(size_t elementSize, size_t maxElement) : m_elementSize(elementSize), m_maxElement(maxElement) {
			this->m_totalSize = m_elementSize * m_maxElement;
			this->m_ptr = new char[m_totalSize];
			this->m_size = 0;
			std::cout << "Total Size : " << m_totalSize << "[bytes]" << std::endl;
		}

		PoolBase(const PoolBase&) = delete;
		PoolBase& operator=(const PoolBase&) = delete;
		virtual ~PoolBase() {
			delete[] m_ptr;
		}

		inline void setSize(const size_t size) { this->m_size = size; }

		size_t size() const { return this->m_size; }
		size_t elementSize() const { return this->m_elementSize; }
		size_t maxElement() const { return this->m_maxElement; }
		size_t totalSize() const { return this->m_totalSize; }

		inline void* get(size_t n) {
			return this->m_ptr + n * this->m_elementSize;
		}

		virtual void destroy(size_t n) = 0;
	};

	template<typename Component>
	class Pool : public PoolBase {
	public:
		Pool(size_t elementSize, size_t maxElement) : PoolBase(elementSize, maxElement) {}
		~Pool() {
			for (size_t i = 0; i < m_size; ++i) {
				this->destroy(i);
			}
		}

		inline Component& at(size_t n) {
			static_cast<Component>(this->get(n));
		}

		void destroy(size_t n) override {
			static_cast<Component*>(this->get(n))->~Component();
		}
	};
}