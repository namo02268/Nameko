#include <iostream>

#include "Nameko/TypeInfo.h"

struct Transform {
	Transform(float x, float y) : x(x), y(y) {
		std::cout << "Transform Constructor : " << x << std::endl;
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

	Transform t(1, 1);
	std::cout << "------------------" << std::endl;
	Transform s(std::move(t));
	s.x = 2, s.y = 2;
	std::cout << "------------------" << std::endl;

	return 0;
}
