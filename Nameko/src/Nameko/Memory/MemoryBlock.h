#pragma once

#include <tuple>
#include <iostream>
#include <stdexcept>
#include <type_traits>

namespace Nameko {

    template<typename... Types>
    struct has_duplicates_impl;

    template<typename T>
    struct has_duplicates_impl<T> : std::false_type {};

    template<typename T, typename U, typename... Rest>
    struct has_duplicates_impl<T, U, Rest...>
        : std::conditional_t<
        std::is_same_v<T, U> || has_duplicates_impl<T, Rest...>::value,
        std::true_type,
        has_duplicates_impl<U, Rest...>
        >
    {};

    template<typename... Types>
    constexpr bool has_duplicates = has_duplicates_impl<Types...>::value;

    template<typename... Types>
    class MemoryBlock
    {
    private:
        size_t totalSize;
        size_t typeSizes[sizeof...(Types)];
        size_t typeAlignments[sizeof...(Types)];
        size_t offsets[sizeof...(Types)];
        size_t arraySize;
        size_t nextIndices[sizeof...(Types)] = { 0 };
        std::tuple<Types*...> typePointers;
        std::byte* ptr;

    public:
        MemoryBlock(size_t size) : arraySize(size) {
            size_t sizes[] = { sizeof(Types)... };
            size_t alignments[] = { alignof(Types)... };

            totalSize = 0;
            size_t offset = 0;

            for (size_t i = 0; i < sizeof...(Types); ++i) {
                typeSizes[i] = sizes[i];
                typeAlignments[i] = alignments[i];
                offset = AlignUp(offset, typeAlignments[i]);
                offsets[i] = offset;
                totalSize = offset + typeSizes[i] * arraySize;
                offset += typeSizes[i] * arraySize;
            }

            ptr = new(std::nothrow) std::byte[totalSize];
            if (!ptr) {
                throw std::runtime_error("Memory allocation failed");
            }

            InitializeTypePointers(std::make_index_sequence<sizeof...(Types)>{});

#ifdef _DEBUG
            std::cout << "Type sizes: ";
            for (size_t size : sizes) {
                std::cout << size << " ";
            }
            std::cout << "\nTotal size: " << totalSize << std::endl;
#endif
        }

        ~MemoryBlock() {
            DestroyAllElements(std::make_index_sequence<sizeof...(Types)>{});
            delete[] ptr;
        }

        size_t GetBlockSize() const {
            return totalSize;
        }

        template<typename T>
        T* Data() {
            static_assert(((std::is_same<T, Types>::value) || ...), "Type T is not part of the MemoryBlock");
            return std::get<T*>(typePointers);
        }

        template<typename T>
        T* Get(std::size_t idx) {
            static_assert(((std::is_same<T, Types>::value) || ...), "Type T is not part of the MemoryBlock");
#ifdef _DEBUG
            size_t typeIndex = GetTypeIndex<T>();
            if (idx >= nextIndices[typeIndex]) {
                throw std::out_of_range("Index out of range");
            }
#endif
            return std::get<T*>(typePointers) + idx;
        }

        template<typename T>
        void Create(T&& t) {
            size_t typeIndex = GetTypeIndex<T>();
#ifdef _DEBUG
            if (nextIndices[typeIndex] >= arraySize) {
                throw std::runtime_error("MemoryBlock overflow");
            }
#endif
            new (std::get<T*>(typePointers) + nextIndices[typeIndex]) T(std::forward<T>(t));
            ++nextIndices[typeIndex];
        }

        template<typename T>
        void Destroy(size_t idx) {
            static_assert(((std::is_same<T, Types>::value) || ...), "Type T is not part of the MemoryBlock");
            size_t typeIndex = GetTypeIndex<T>();
#ifdef _DEBUG
            if (idx >= nextIndices[typeIndex]) {
                throw std::out_of_range("Index out of range");
            }
#endif
            T* start = std::get<T*>(typePointers);
            T* element = start + idx;
            element->~T();

            if (idx != nextIndices[typeIndex] - 1) {
                T* lastElement = start + (nextIndices[typeIndex] - 1);
                new (element) T(std::move(*lastElement));
                lastElement->~T();
            }
            --nextIndices[typeIndex];
        }

    private:
        template<size_t... Index>
        void InitializeTypePointers(std::index_sequence<Index...>) {
            ((std::get<Index>(typePointers) = reinterpret_cast<Types*>(ptr + offsets[Index])), ...);
        }

        size_t AlignUp(size_t offset, size_t alignment) const {
            return (offset + alignment - 1) & ~(alignment - 1);
        }

        template<typename T>
        size_t GetTypeIndex() const {
            return GetTypeIndexImpl<T, 0, Types...>();
        }

        template<typename T, size_t Index, typename First, typename... Rest>
        size_t GetTypeIndexImpl() const {
            if constexpr (std::is_same_v<T, First>) {
                return Index;
            }
            else {
                return GetTypeIndexImpl<T, Index + 1, Rest...>();
            }
        }

        template<size_t... Index>
        void DestroyAllElements(std::index_sequence<Index...>) {
            ((DestroyElementsForType<Index, Types>()), ...);
        }

        template<size_t Index, typename T>
        void DestroyElementsForType() {
            T* start = std::get<Index>(typePointers);
            for (size_t i = 0; i < nextIndices[Index]; ++i) {
                (start + i)->~T();
            }
        }

        static_assert(!has_duplicates<Types...>, "Duplicate types are not allowed in MemoryBlock");
    };

    template<typename T, typename... SrcTypes, typename... DstTypes>
    static void Move(MemoryBlock<SrcTypes...>* src, MemoryBlock<DstTypes...>* dst, size_t srcIdx) {
        T* sourceElement = src->Get<T>(srcIdx);
        dst->Create<T>(std::move(*sourceElement));
        src->Destroy<T>(srcIdx);
    }
}