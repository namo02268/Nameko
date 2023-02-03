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
	template<typename T, size_t MaxElement>
	class Block {
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
			std::cout << "Free Block" << std::endl;
			for (size_t i = 0; i < m_size; i++) {
				this->at(i).~T();
			}

			free(this->m_ptr);
		}

		T& operator[](const int n) { return *this->get(n); }
		size_t size() const { return this->m_size; }
		size_t getMaxElement() const { return this->m_maxElement; }
		size_t getTypeSize() const { return this->m_typeSize; }
		size_t getTotalSize() const { return this->m_totalSize; }

		template<typename T>
		inline T* create(T&& t) {
			auto ptr = static_cast<T*>(this->m_ptr) + m_size;
			::new(ptr) T(std::forward<T>(t));
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

		inline void replace(size_t n, T&& t) {
			this->at(n) = t;
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
	
	template<size_t Size, typename... Types>
	class Chunk {
		template<typename T>
		using BlockType = Block<T, Size>;
		using Blocks = std::tuple<BlockType<Types>...>;
		template<typename T, typename Tuple>
		struct has_type;
		template<typename T, typename... Ts>
		struct has_type<T, std::tuple<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

	private:
		Blocks m_blocks;
		size_t m_size = 0;
		const size_t TypeCount = sizeof...(Types);

	public:
		Chunk() = default;

		~Chunk() = default;

		void Add(Types&&... types) {
			(this->GetBlock<Types>()->create(std::forward<Types>(types)), ...);
			m_size++;
		}

		template<typename T>
		T& Get(size_t n) {
			return this->GetBlock<T>()->at(n);
		}

		auto GetAll(size_t n) -> std::tuple<Types...> {
			return { this->GetBlock<Types>()->at(n)... };
		}

		void Remove(size_t n) {
			(this->GetBlock<Types>()->destroy(n), ...);
			m_size--;
		}

		void Replace(size_t n, Types&&... types) {
			(this->GetBlock<Types>()->replace(n, std::forward<Types>(types)), ...);
		}

		template<typename... Targets>
		void IterateAll(const std::function<void(Targets&...)> lambda) {
			for (size_t i = 0; i < m_size; ++i) {
				lambda(this->Get<Targets>(i)...);
			}
		}
		
	private:
		template<typename T>
		BlockType<T>* GetBlock() {
			static_assert(has_type<BlockType<T>, std::tuple<BlockType<Types>...>>::value, "Component is not contained in this Chunk.");
			return &std::get<BlockType<T>>(this->m_blocks);
		}
	};
}