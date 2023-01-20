#pragma once

namespace Nameko {
	constexpr size_t MAX_ENTITIES = 1024 * 16;
	constexpr size_t MAX_COMPONENTS = 1024 * 16;
	constexpr size_t CHUNK_SIZE = 1024;

	using FamilyID = uint32_t;
	using ArcheID = FamilyID;
}
