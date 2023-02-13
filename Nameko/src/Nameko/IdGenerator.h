#pragma once

#include <cstdint>
#include "Nameko/Config.h"

namespace Nameko {
	class IdGenerator {
	private:
		static inline FamilyID counter = 0;

		template<typename T>
		static inline const FamilyID familyCounter = counter++;

	public:
		template<typename T>
		static inline FamilyID GetFamily() {
			return 1 << familyCounter<T>;
		}

		template<typename... Types>
		static inline ArcheID GetArche() {
			return (GetFamily<Types>() | ...);
		}
	};

	inline EntityID GetEntityID() {
		static EntityID id = 1;
		return id++;
	}
}

/*
C++でテンプレートメタプログラミングを使って、
AddType<Mesh, 0>;
AddType<Transform, 1>;
GetType<0>::type; // type: Mesh
GetType<1>::type; // type: Transform
のようにコンパイル時に型と数字のペアを追加して、後から数字を使って型を取り出すことができるようにしてください
*/