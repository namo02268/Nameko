#include <iostream>
#include "Nameko/ArcheType.h"
#include <tuple>
#include <vector>
#include "Nameko/Memory/Factory.h"

struct Transform {
	Transform(float x, float y) : x(x), y(y) {
		std::cout << "Transform Constructor : " << x << std::endl;
	}

	// Copy Constructor
	Transform(const Transform& other) : x(other.x), y(other.y) {
		std::cout << "Transform Copy Constructor : " << x << std::endl;
	}

	// Move Constructor
	Transform(Transform&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)) {
		std::cout << "Transform Move Constructor : " << x << std::endl;
	}

	Transform& operator=(const Transform& other) {
		std::cout << "Transform Copy Constructor : " << x << std::endl;
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	~Transform() {
		std::cout << "Transform Destructor : " << x << std::endl;
	}

	float x;
	float y;
};

struct Mesh {
	Mesh(float y) : y(y) {}
	float y;
};

struct Collider {
	Collider(float x) : x(x) {}
	float x;
};

struct Vertex {
	float vertices[512];
};

struct Model {
	Transform transform{ 1, 1 };
	Mesh mesh{ 1 };
	Vertex vertex;
};

int main() {
	using namespace Nameko;
//	ArcheType<int, float, char> arche;
//	arche.AllocatePool();

	MemoryBlock<int, Transform, char> block1(4);
	block1.Create(Transform(1, 1));
	block1.Create(Transform(2, 2));
	block1.Create(Transform(3, 3));

	MemoryBlock<Transform, Mesh> block2(4);

	Move<Transform>(block1, block2, 1);


	return 0;
}
