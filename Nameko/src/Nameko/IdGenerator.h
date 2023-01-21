#pragma once

#include <cstdint>
#include "Nameko/Config.h"

namespace Nameko {
	class IdGenerator {
	private:
		static inline FamilyID counter = 1;

	public:
		template<typename T>
		static inline const FamilyID GetFamily = counter <<= 1;

		template<typename... Types>
		static inline const ArcheID GetArche = (GetFamily<Types> | ...);
	};

	inline EntityID GetEntityID() {
		static EntityID id = 1;
		return id++;
	}
}
