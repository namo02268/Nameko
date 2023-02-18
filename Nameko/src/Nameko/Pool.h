#pragma once

#include <memory>
#include <cassert>
#include <iostream>

namespace Nameko {
	class BasePool {
	protected:
		std::vector<char*> m_blocks;

		size_t m_elementSize;
		size_t m_chunkSize;
		size_t m_totalBytes;

		size_t m_size;

	public:
		BasePool(size_t elementSize, size_t chunkSize) {
			this->m_elementSize = elementSize;
			this->m_chunkSize = chunkSize;
			this->m_totalBytes = elementSize * chunkSize;
			this->m_size = 0;
		}

		virtual ~BasePool() {
			for (auto ptr : m_blocks) {
				delete[] ptr;
				std::cout << "Free : " << m_totalBytes << "[bytes]" << std::endl;
			}
		}

		size_t size() const { return this->m_size; }

		void* get(size_t n) {
			assert(m_size > n && "n must be smaller than size.");
			auto currentSize = n % m_chunkSize;
			auto chunkSize = n / m_chunkSize;
			return this->m_blocks[chunkSize] + m_elementSize * (currentSize);
		}

		inline void add(void* ptr) {
			auto currentSize = m_size % m_chunkSize;
			auto chunkSize = m_size / m_chunkSize;

			if (m_blocks.size() < chunkSize + 1) {
				auto ptr = new char[this->m_totalBytes];
				this->m_blocks.emplace_back(ptr);
				std::cout << "Total Bytes : " << m_totalBytes << std::endl;
			}

			this->m_size++;
			std::memcmp(this->get(this->m_size - 1), ptr, m_elementSize);
		}

		void replace(size_t n, void* ptr) {
			std::memcpy(this->get(n), ptr, m_elementSize);
		}

		virtual void destroy(size_t n) = 0;
		virtual BasePool* createPool() = 0;
	};

	template<typename T, size_t ChunkSize>
	class Pool : public BasePool {
	public:
		Pool() : BasePool(sizeof(T), ChunkSize) {}
		~Pool() {
			for (int i = 0; i < m_size; i++) {
				static_cast<T*>(this->get(i))->~T();
			}
		}

		inline void add(T& t) {
			auto currentSize = m_size % m_chunkSize;
			auto chunkSize = m_size / m_chunkSize;

			if (m_blocks.size() < chunkSize + 1) {
				auto ptr = new char[this->m_totalBytes];
				this->m_blocks.emplace_back(ptr);
				std::cout << "Total Bytes : " << m_totalBytes << std::endl;
			}

			this->m_size++;

			auto ptr = static_cast<T*>(this->get(m_size - 1));
			::new(ptr) T(std::forward<T>(t));
		}

		inline void destroy(size_t n) override {
			static_cast<T*>(this->get(n))->~T();
			this->replace(n, this->get(m_size - 1));

			this->m_size--;
		}

		BasePool* createPool() override {
			return new Pool<T, ChunkSize>;
		}
	};
}