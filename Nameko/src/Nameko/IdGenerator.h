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
C++�Ńe���v���[�g���^�v���O���~���O���g���āA
AddType<Mesh, 0>;
AddType<Transform, 1>;
GetType<0>::type; // type: Mesh
GetType<1>::type; // type: Transform
�̂悤�ɃR���p�C�����Ɍ^�Ɛ����̃y�A��ǉ����āA�ォ�琔�����g���Č^�����o�����Ƃ��ł���悤�ɂ��Ă�������
*/