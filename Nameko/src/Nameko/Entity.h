#pragma once

#include "Nameko/IdGenerator.h"

namespace Nameko {
	using Entity = EntityID;

	class EntityManager {
	public:
		~EntityManager() = default;

		Entity CreateEntity() { return Entity(GetEntityID()); }
		void DestroyEntity(Entity e) {
			// TODO : Recycle ID
		}
	};
}
