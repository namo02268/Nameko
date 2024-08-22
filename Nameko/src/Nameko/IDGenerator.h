#pragma once

#include <cstdint>
#include "Nameko/Config.h"

namespace Nameko {
	class IDGenerator {
	private:
		static inline size_t counter = 0;

		template<typename T>
		static inline const size_t familyCounter = counter++;

	public:
		template<typename T>
		static inline FamilyID GetFamilyID() {
			return 1 << familyCounter<T>;
		}

		template<typename T>
		static inline FamilyID GetFamilyCount() {
			return familyCounter<T>;
		}

		template<typename... Types>
		static inline ArcheID GetArcheID() {
			return (GetFamily<Types>() | ...);
		}
	};

	inline EntityID GetEntityID() {
		static EntityID id = 1;
		return id++;
	}
}