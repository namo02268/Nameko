#include "Nameko/Entity.h"
#include "Nameko/IdGenerator.h"
#include "Nameko/Archtype.h"
#include "Nameko/ECS.h"

#include <bitset>

#include<chrono>

struct Transform {
public:
	Transform(float x, float y) : x(x), y(y) {
		std::cout << "Transform Constructor" << std::endl;
	}

	Transform(const Transform& other) {
		this->x = other.x;
		this->y = other.y;
		std::cout << "Transform Copy Constructor" << std::endl;
	}

	Transform(Transform&& other) noexcept {
		this->x = other.x;
		this->y = other.y;
		std::cout << "Transform Move Constructor" << std::endl;
	}

	Transform& operator=(const Transform& other) {
		this->x = other.x;
		this->y = other.y;
		std::cout << "Transform Copy= Constructor" << std::endl;
		return *this;
	}

	Transform& operator=(Transform&& other) noexcept {
		this->x = other.x;
		this->y = other.y;
		std::cout << "Transform Move= Constructor" << std::endl;
		return *this;
	}



	~Transform() {
		std::cout << "Transform Destructor" << std::endl;
	}

	float x;
	float y;
};

struct Mesh {
public:
	Mesh(float y) : y(y) {}

	float y;
};

int main() {
	using namespace Nameko;
	EntityManager eManager;
//	Transform t1(3, 3);
//	Mesh m1(1);
	auto e = eManager.CreateEntity();

	auto arche = new Archetype;

	for (int i = 0; i < 3; i++) {
		arche->AddComponents(i, Transform(i, i), Mesh(i));
	}

	/*
	std::cout << "--------------------------------------" << std::endl;

	std::cout << arche->RetrieveInstance(0, IdGenerator::GetFamily<Transform>()) << std::endl;
	std::cout << arche->RetrieveInstance(1, IdGenerator::GetFamily<Transform>()) << std::endl;
	std::cout << arche->RetrieveInstance(2, IdGenerator::GetFamily<Transform>()) << std::endl;
	*/

	delete arche;

	return 0;
}
