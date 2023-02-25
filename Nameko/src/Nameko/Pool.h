#pragma once

#include <memory>
#include <cassert>
#include <iostream>

namespace Nameko {
	class BasePool {
	protected:
		std::vector<std::byte*> m_blocks;

		size_t m_elementSize;
		size_t m_chunkSize;
		size_t m_totalBytes;

		size_t m_size;

	public:
		BasePool(size_t elementSize, size_t chunkSize)
			: m_elementSize(elementSize)
			, m_chunkSize(chunkSize)
			, m_totalBytes(elementSize * chunkSize)
			, m_size(0)
		{}

		virtual ~BasePool() = default;

		size_t size() const { return m_size; }

		void* get(size_t n) {
			assert(m_size > n && "n must be smaller than size.");
			const size_t chunkSize = n / m_chunkSize;
			const size_t currentSize = n - chunkSize * m_chunkSize;
			return m_blocks[chunkSize] + m_elementSize * (currentSize);
		}

		virtual void add(void* ptr) = 0;
		virtual void replace(size_t n, void* ptr) = 0;
		virtual void destroy(size_t n) = 0;
		virtual BasePool* createPool() = 0;
	};

	template<typename T, size_t ChunkSize>
	class Pool : public BasePool {
	public:
		Pool() 
			: BasePool(sizeof(T), ChunkSize)
		{
			static_assert(std::is_standard_layout<T>::value == true, "template parameter T must be trivial");
		}

		~Pool() {
			for (size_t i = 0; i < m_size; i++) {
				static_cast<T*>(this->get(i))->~T();
			}

			for (auto ptr : m_blocks) {
				delete[] ptr;
				std::cout << "Free : " << m_totalBytes << "[bytes]" << std::endl;
			}
		}

		inline void add(T& t) {
			const size_t index = m_size++;
			const size_t chunkSize = index / m_chunkSize;

			if(chunkSize >= m_blocks.size()) {
				// ÉAÉâÉCÉÅÉìÉg
				// auto ptr = static_cast<std::byte*>(operator new(m_totalBytes, std::align_val_t(alignof(T))));
				// m_blocks.emplace_back(ptr);
				m_blocks.emplace_back(new std::byte[m_totalBytes]);
				std::cout << "Total Bytes : " << m_totalBytes << std::endl;
			}

			::new(this->get(index)) T(std::move(t));
		}

		inline void add(void* ptr) override {
			this->add(*static_cast<T*>(ptr));
		}

		void replace(size_t n, void* ptr) override {
			*static_cast<T*>(this->get(n)) = std::move(*static_cast<T*>(ptr));
		}

		inline void destroy(size_t n) override {
			const size_t last = m_size - 1;
			static_cast<T*>(this->get(n))->~T();
			if (n != last) {
				this->replace(n, this->get(last));
			}

			m_size--;
		}

		BasePool* createPool() override {
			return new Pool<T, ChunkSize>;
		}
	};
}