#pragma once

#include <vector>

#include "Archtype.h" 

namespace Nameko {
	class ECS {
	private:
		std::vector<BaseArchtype*> ArcheTypes;

	public:
		ECS() = default;
		~ECS() = default;


	};
}