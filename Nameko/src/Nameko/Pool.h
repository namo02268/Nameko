#pragma once

#include <memory>
#include <cassert>
#include <iostream>
#include <vector>
#include <array>
#include <tuple>

namespace Nameko {
	class BasePool {
	public:
		BasePool() = default;
		BasePool(const BasePool&) = delete;
		BasePool& operator=(const BasePool&) = delete;
		BasePool(BasePool&&) = delete;
		BasePool& operator=(BasePool&&) = delete;
		virtual ~BasePool() = default;
	};

	template<size_t ChunkSize, class... T>
	class Pool {
	private:
		std::vector<std::tuple<T*...>> m_blocks;
		std::array<size_t, sizeof...(T)> m_typeSizes;
		size_t m_chunkSize;
		size_t m_totalSize;
		size_t m_size;

	public:
		Pool() : m_chunkSize(ChunkSize), m_size(0), m_totalSize(0) {
			size_t index = 0;
			((m_typeSizes[index++] = sizeof(T)), ...);
			for (size_t size : m_typeSizes) {
				m_totalSize += size;
			}
			m_totalSize *= m_chunkSize;
		}

		~Pool() {
			for (auto block : m_blocks) {
				for (size_t i = 0; i < m_size; i++) {
					(this->at<T>(i).~T(), ...);
				}
				free(static_cast<void*>(std::get<0>(block)));
			}
		}

		/*
		std::tuple<T&...> at(const size_t n) {
			if (n >= m_size) {
				throw std::out_of_range("Index out of range");
			}
			const size_t blockIndex = n / m_chunkSize;
			const size_t index = n - blockIndex * m_chunkSize;
			return std::tie(std::get<T*>(m_blocks[blockIndex])[index]...);
		}
		*/

		template<class S>
		S& at(const size_t n) {
			if (n >= m_size) {
				throw std::out_of_range("Index out of range");
			}
			const size_t blockIndex = n / m_chunkSize;
			const size_t index = n - blockIndex * m_chunkSize;
			return std::get<S*>(m_blocks[blockIndex])[index];
		}

		void push_back(T&&... t) {
			const size_t index = m_size++;
			const size_t blockIndex = index / m_chunkSize;

			if (blockIndex >= m_blocks.size()) {
				char* ptr = static_cast<char*>(malloc(m_totalSize));
				size_t offset = 0;
				m_blocks.emplace_back(std::make_tuple(static_cast<T*>(nullptr)...));
				makeBlock<0, T...>(ptr, 0, blockIndex);
			}

			((::new(&this->at<T>(index)) T(std::move(t))), ...);
		}

		void remove(const size_t n) {
			const size_t last = m_size - 1;
			if (n != last) {
				((this->at<T>(n) = std::move(this->at<T>(last))),...);
			}
			m_size--;
		}

		void clear() {
			for (auto block : m_blocks) {
				for (size_t i = 0; i < m_size; i++) {
					(this->at<T>(i).~T(), ...);
				}
				free(static_cast<void*>(std::get<0>(block)));
			}

			m_blocks.clear();
			m_size = 0;
		}

	private:
		template <int N, typename T>
		void makeBlock(char* ptr, size_t offset, const size_t blockIndex) {
			std::get<N>(m_blocks[blockIndex]) = static_cast<T*>(static_cast<void*>(ptr + offset));
		}

		template <int N, typename T0, typename T1, typename ... Tn>
		void makeBlock(char* ptr, size_t offset, const size_t blockIndex) {
			std::get<N>(m_blocks[blockIndex]) = static_cast<T0*>(static_cast<void*>(ptr + offset));
			offset += sizeof(T0) * m_chunkSize;
			makeBlock<N + 1, T1, Tn...>(ptr, offset, blockIndex);
		}

		/*
		struct Iterator {
		private:
			Pool* pool_;
			size_t index_;

		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			Iterator(Pool* pool, size_t index) : pool_(pool), index_(index) {}

			reference operator*() const { return (*pool_)[index_]; }
			pointer operator->() { return &(*pool_)[index_]; }
			Iterator& operator++() { ++index_; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
			friend bool operator==(const Iterator& a, const Iterator& b) { return a.index_ == b.index_; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.index_ != b.index_; }
		};

		Iterator begin() { return Iterator(this, 0); }
		Iterator end() { return Iterator(this, m_size); }
		*/
	};
}
