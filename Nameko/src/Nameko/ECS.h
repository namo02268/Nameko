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

			auto componentTuple = std::make_tuple(std::forward<Components>(components)...);

			if (archeID != 0) {
//				this->cast_arche(componentTuple, archeID)->
			}

			if (!m_archetypes.contains(archeID)) {
				m_archetypes.emplace(archeID, this->make_arche_from_ref(componentTuple));
			}
			this->cast_arche_from_ref(componentTuple, archeID)->AddComponents(e, std::forward<Components>(components)...);
		}

		template<typename... Components>
		void AddComponentsPtr(Entity e, Components*... components) {
			auto entityArcheID = m_entityToArche[e];
			auto componentArcheID = IdGenerator::GetArche<Components...>();
			auto archeID = entityArcheID | componentArcheID;

			auto componentTuple = std::make_tuple(*components...);

			if (archeID != 0) {
				//				this->cast_arche(componentTuple, archeID)->
			}

			if (!m_archetypes.contains(archeID)) {
				m_archetypes.emplace(archeID, this->make_arche_from_ptr(componentTuple));
			}
			this->cast_arche_from_ptr(componentTuple, archeID)->AddComponentsPtr(e, components...);
		}

	private:
		template<typename Tuple, size_t... S>
		auto make_arche_from_ref_impl(Tuple&& t, std::index_sequence<S...>) {
			return new Archetype<std::remove_reference_t<decltype(std::get<S>(t))>...>;
		}

		template<typename Tuple>
		auto make_arche_from_ref(Tuple&& t) {
			return this->make_arche_from_ref_impl(std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
		}

		template<typename Tuple, size_t... S>
		auto cast_arche_from_ref_impl(Tuple&& t, ArcheID id, std::index_sequence<S...>) {
			return static_cast<Archetype<std::remove_reference_t<decltype(std::get<S>(t))>...>*>(this->m_archetypes[id]);
		}

		template<typename Tuple>
		auto cast_arche_from_ref(Tuple&& t, ArcheID id) {
			return this->cast_arche_from_ref_impl(std::forward<Tuple>(t), id, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
		}

		template<typename Tuple, size_t... S>
		auto make_arche_from_ptr_impl(Tuple&& t, std::index_sequence<S...>) {
			return new Archetype<std::remove_pointer_t<decltype(std::get<S>(t))>...>;
		}

		template<typename Tuple>
		auto make_arche_from_ptr(Tuple&& t) {
			return this->make_arche_from_ref_impl(std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
		}

		template<typename Tuple, size_t... S>
		auto cast_arche_from_ptr_impl(Tuple&& t, ArcheID id, std::index_sequence<S...>) {
			return static_cast<Archetype<std::remove_pointer_t<decltype(std::get<S>(t))>...>*>(this->m_archetypes[id]);
		}

		template<typename Tuple>
		auto cast_arche_from_ptr(Tuple&& t, ArcheID id) {
			return this->cast_arche_from_ref_impl(std::forward<Tuple>(t), id, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
		}

		template<typename Tuple, size_t... S>
		void add_impl(Entity e, Tuple&& t, ArcheID id, std::index_sequence<S...>) {
			if (!m_archetypes.contains(id)) {
				auto* archetype = new Archetype<std::remove_reference_t<decltype(std::get<S>(t))>...>;
				m_archetypes.emplace(id, archetype);
			}
			static_cast<Archetype<std::remove_reference_t<decltype(std::get<S>(t))>...>*>(this->m_archetypes[id])->AddComponents(e, std::get<S>(t)...);
		}

		template<typename Tuple>
		void add(Entity e, Tuple&& t, ArcheID id) {
			this->add_impl(e, std::forward<Tuple>(t), id, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
		}
	};
}