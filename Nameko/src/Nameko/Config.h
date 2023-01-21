#pragma once

#include <cstdint>

namespace Nameko {
	constexpr size_t MAX_ENTITIES = 1024 * 16;
	constexpr size_t MAX_COMPONENTS = 1024 * 16;
	constexpr size_t CHUNK_SIZE = 1024;

	using EntityID = uint32_t;
	using FamilyID = uint32_t;
	using ArcheID = FamilyID;
}
