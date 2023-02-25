#pragma once

#include <unordered_map>
#include <array>

#include "Nameko/Entity.h"
#include "Nameko/Archtype.h" 
#include "Nameko/IdGenerator.h"
#include "Nameko/System.h"

namespace Nameko {
	class ECS {
	private:
		EntityManager* m_entityManager;
		std::unordered_map<ArcheID, Archetype*>  m_archetypes;
		std::array<ArcheID, MAX_ENTITIES> m_entityToArche;
		std::vector<std::unique_ptr<System>> m_systems;

	public:
		ECS() {
			m_entityManager = new EntityManager;
		}

		~ECS() {
			delete m_entityManager;
			for (auto& pair : m_archetypes) {
				delete pair.second;
			}
		}

		void AddSystem(std::unique_ptr<System> system) {
			system->SetECS(this);
			m_systems.emplace_back(std::move(system));
		}

		void Init() {
			for (auto& system : m_systems) {
				system->Init();
			}
		}

		void Update(float dt) {
			for (auto& system : m_systems) {
				system->Update(dt);
			}
		}

		void Draw() {
			for (auto& system : m_systems) {
				system->Draw();
			}
		}

		Entity CreateEntity() {
			return m_entityManager->CreateEntity();
		}

		void DestoryEntity(Entity e) {
			m_entityManager->DestroyEntity(e);
			m_archetypes[m_entityToArche[e]]->RemoveComponents(e);
		}

		template<typename Component>
		void AddComponent(Entity e, Component&& component) {
			auto oldArcheID = m_entityToArche[e];
			auto newArcheID = oldArcheID | IdGenerator::GetArche<Component>();

			if (this->HasComponent<Component>(e)) {
				std::cout << "Entity already has the Component." << std::endl;
			}
			else {
				if (!m_archetypes.contains(newArcheID)) {
					m_archetypes[newArcheID] = new Archetype;
				}
				m_archetypes[newArcheID]->AddComponent<Component>(e, std::forward<Component>(component), m_archetypes[oldArcheID]);
				m_entityToArche[e] = newArcheID;
			}
		}

		template<typename Component>
		Component* GetComponent(Entity e) {
			if (this->HasComponent<Component>(e)) {
				auto archeID = m_entityToArche[e];
				return m_archetypes[archeID]->GetComponent<Component>(e);
			}
			else {
				std::cout << "Entity doesn't have the Component." << std::endl;
				return nullptr;
			}
		}

		template<typename Component>
		inline bool HasComponent(Entity e) {
			return m_entityToArche[e] & IdGenerator::GetFamily<Component>();
		}

		template<typename Component>
		void RemoveComponent(Entity e) {
			if (this->HasComponent<Component>(e)) {
				auto oldArcheID = m_entityToArche[e];
				auto newArcheID = oldArcheID & ~IdGenerator::GetArche<Component>();

				if (newArcheID != 0) {
					if (!m_archetypes.contains(newArcheID)) {
						m_archetypes[newArcheID] = new Archetype;
					}
					m_archetypes[oldArcheID]->RemoveComponent<Component>(e, m_archetypes[newArcheID]);
				}
				else {
					m_archetypes[oldArcheID]->RemoveComponents(e);
				}
				m_entityToArche[e] = newArcheID;
			}
			else {
				std::cout << "Entity doesn't have the Component." << std::endl;
			}
		}

		template<typename... Components>
		void EachComponent(const std::function<void(Components&...)>&& lambda) {
			auto mask = IdGenerator::GetArche<Components...>();
			for (const auto& [archeID, archetype] : this->m_archetypes) {
				if ((archeID & mask) == mask) {
					archetype->EachComponent<Components...>(std::forward<const std::function<void(Components&...)>>(lambda));
				}
			}
		}

		template<typename... Components>
		void EachEntity(const std::function<void(Entity&)>&& lambda) {
			auto mask = IdGenerator::GetArche<Components...>();
			for (const auto& [archeID, archetype] : this->m_archetypes) {
				if ((archeID & mask) == mask) {
					archetype->EachEntity(lambda);
				}
			}
		}

		Entity DeplicateEntity(Entity src) {
			Entity dst = this->CreateEntity();
			auto mask = m_entityToArche[src];

			m_entityToArche[dst] = mask;
			m_archetypes[mask]->DeplicateEntity(src, dst);

			return dst;
		}
	};
}