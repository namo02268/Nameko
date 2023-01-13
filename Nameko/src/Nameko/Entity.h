#pragma once

#include<cstdint>

namespace Nameko {
	using Entity = uint64_t;	// Upper 32bit = CCID, Lower 32it = EID
	using EID = uint32_t;		// Entity ID
	using CIID = uint32_t;		// Component Instance ID

	static inline EID GetEID(const Entity e) {
		return e & 0xffffffffUL;
	}

	static inline CIID GetCIID(const Entity e) {
		return e >> 32;
	}

	static inline void SetEID(Entity& e, const EID id) {
		e = (e & ~0xFFFFFFFFUL) | id;
	}

	static inline void SetCIID(Entity& e, const CIID id) {
		e |= (static_cast<Entity>(id) << 32);
	}
}
