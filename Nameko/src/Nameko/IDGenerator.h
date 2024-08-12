#pragma once

#include <cstdint>
#include "Nameko/Config.h"

namespace Nameko {
	class IDGenerator {
	private:
		static inline FamilyID counter = 0;

		template<typename T>
		static inline const FamilyID familyCounter = counter++;

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