#pragma once

#include <unordered_map>
#include <array>

#include "Nameko/Archtype.h" 
#include "Nameko/IdGenerator.h"

namespace Nameko {
	class ECS {
	private:
		std::unordered_map<ArcheID, BaseArchtype*> m_archetypes;
		std::array<ArcheID, MAX_ENTITIES> m_entityToArche;

	public:
		ECS() = default;
		~ECS() {
			for (auto& itr : m_archetypes) {
				delete itr.second;
			}
		}

		template<typename... Components>
		void AddComponents(Entity e, Components&&... components) {
			auto entityArcheID = m_entityToArche[e];
			auto componentArcheID = IdGenerator::GetArche<Components...>();
			auto archeID = entityArcheID | componentArcheID;
			
			std::tuple<Components&...> componentTuple{ components... };

			if (entityArcheID != 0) {

			}

			if (!m_archetypes.contains(archeID)) {
				m_archetypes.emplace(archeID, this->make_arche(componentTuple));
			}
			this->cast_arche(componentTuple, archeID)->AddComponents(e, std::forward<Components>(components)...);
		}

	private:
		template<typename Tuple, size_t... S>
		auto make_arche_impl(Tuple&& t, std::index_sequence<S...>) {
			return new Archetype<std::remove_reference_t<decltype(std::get<S>(t))>...>;
		}

		template<typename Tuple>
		auto make_arche(Tuple&& t) {
			return this->make_arche_impl(std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
		}

		template<typename Tuple, size_t... S>
		auto cast_arche_impl(Tuple&& t, ArcheID id, std::index_sequence<S...>) {
			return static_cast<Archetype<std::remove_reference_t<decltype(std::get<S>(t))>...>*>(this->m_archetypes[id]);
		}

		template<typename Tuple>
		auto cast_arche(Tuple&& t, ArcheID id) {
			return this->cast_arche_impl(std::forward<Tuple>(t), id, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
		}
	};
}