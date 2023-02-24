#pragma once

#include <utility>

#include "Nameko/Config.h"

namespace Nameko {
    class IdGenerator {
    private:
        template<typename T, FamilyID counter>
        struct FamilyCounter {
            static constexpr FamilyID value = counter;
        };

        template<typename T, FamilyID... counters>
        struct FindFamilyCounter;

        template<typename T, FamilyID counter, FamilyID... counters>
        struct FindFamilyCounter<T, counter, counters...> {
            static constexpr FamilyID value = std::conditional_t<std::is_same_v<T, std::tuple_element_t<counter, std::tuple<Types...>>>, FamilyCounter<T, counter>, FindFamilyCounter<T, counters...>>::value;
        };

        template<typename T, FamilyID... counters>
        static constexpr FamilyID getFamilyCounter() {
            return FindFamilyCounter<T, counters...>::value;
        }

        template<typename... Ts, FamilyID... counters>
        static constexpr ArcheID getArcheCounter(std::index_sequence<counters...>) {
            return (GetFamily<Ts>() | ...) << 1;
        }

    public:
        template<typename T>
        static constexpr FamilyID GetFamily() {
            static_assert(std::disjunction_v<std::is_same<T, Types>...>, "Type not found in component list.");
            return 1 << getFamilyCounter<T, 0, (sizeof...(Types) - 1)>();
        }

        template<typename... Types>
        static constexpr ArcheID GetArche() {
            static_assert(sizeof...(Types) <= MAX_COMPONENTS, "Too many components in archetype.");
            return getArcheCounter<Types..., 0, (sizeof...(Types) - 1)>(std::make_index_sequence<sizeof...(Types)>());
        }
    };
}