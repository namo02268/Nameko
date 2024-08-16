#pragma once

#include <tuple>
#include <cstring>
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
        std::tuple<Types*...> typePointers;
        std::byte* ptr;

    public:
        MemoryBlock(size_t size) : arraySize(size) {
            size_t sizes[] = { sizeof(Types)... };
            size_t alignments[] = { alignof(Types)... };

            totalSize = 0;
            size_t offset = 0;

            // Calculate offsets and total size
            for (size_t i = 0; i < sizeof...(Types); ++i) {
                typeSizes[i] = sizes[i];
                typeAlignments[i] = alignments[i];
                offset = alignUp(offset, typeAlignments[i]);
                offsets[i] = offset;
                totalSize = offset + typeSizes[i] * arraySize;
                offset += typeSizes[i] * arraySize;
            }

            ptr = new(std::nothrow) std::byte[totalSize];
            if (!ptr) {
                throw std::runtime_error("Memory allocation failed");
            }
            std::memset(ptr, 0, totalSize);

            initializeTypePointers(std::make_index_sequence<sizeof...(Types)>{});

            // Debug output
            std::cout << "Type sizes: ";
            for (size_t size : sizes) {
                std::cout << size << " ";
            }
            std::cout << "\nTotal size: " << totalSize << std::endl;
        }

        ~MemoryBlock() {
            delete[] ptr;
        }

        size_t getBlockSize() const {
            return totalSize;
        }

        template<typename T>
        T* data() {
            static_assert(((std::is_same<T, Types>::value) || ...), "Type T is not part of the MemoryBlock");
            return std::get<T*>(typePointers);
        }

        template<typename T>
        T* at(std::size_t index) {
            static_assert(((std::is_same<T, Types>::value) || ...), "Type T is not part of the MemoryBlock");
            if (index >= arraySize) {
                throw std::out_of_range("Index out of range");
            }
            return std::get<T*>(typePointers) + index;
        }

    private:
        template<size_t... Index>
        void initializeTypePointers(std::index_sequence<Index...>) {
            ((std::get<Index>(typePointers) = reinterpret_cast<Types*>(ptr + offsets[Index])), ...);
        }

        size_t alignUp(size_t offset, size_t alignment) const {
            return (offset + alignment - 1) & ~(alignment - 1);
        }

        static_assert(!has_duplicates<Types...>, "Duplicate types are not allowed in MemoryBlock");
    };
}
