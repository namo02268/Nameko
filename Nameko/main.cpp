#include <iostream>

#include "Nameko/TypeInfo.h"

struct Transform {
	Transform(float x, float y) : x(x), y(y) {}
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

	std::cout << IdGenerator::GetFamily<Transform>() << std::endl;

	return 0;
}
