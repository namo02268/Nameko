#pragma once

#include <cstdint>

namespace Nameko {
	// Maximum number of entities
	constexpr size_t MAX_ENTITIES = 1024 * 16;
	// Maximum number of component families
	constexpr size_t MAX_FAMILY = 32;
	// Maximum number of components
	constexpr size_t MAX_COMPONENTS = 1024 * 16;
	// Chunk size
	constexpr size_t CHUNK_SIZE = 1024;

	// Entity ID = 32bit
	using EntityID = uint32_t;
	// Entity ID = 32bit
	using ArcheTypeID = uint32_t;
	using ArcheIndex = uint32_t;
	using FamilyID = uint32_t;
	// Arche ID = ArcheTypeID(32bit) + ArcheIndex(32bit)
	using ArcheID = uint64_t;

	struct ArcheID
	{
		uint32_t ArcheTypeID;
		uint32_t PoolIndex;
		uint32_t ArcheIndex;
	};
}
