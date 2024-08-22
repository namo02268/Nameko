
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

template <typename T, typename List>
using remove_type_t = typename TypeUtils::remove_type<T, List>::type;

using original_list = TypeUtils::list<int, float, char>;
using result_list = remove_type_t<float, original_list>;

int main() {
	using namespace Nameko;

	std::cout << std::is_same_v<TypeUtils::list<int, char>, result_list> << std::endl;

	ArcheTypeI<Mesh> src;
	ArcheTypeI<Mesh, Transform> dst;
	
	src.memoryBlocks[0]->Create(Mesh(1));
	src.memoryBlocks[0]->Create(Mesh(2));
	src.memoryBlocks[0]->Create(Mesh(3));

	dst.memoryBlocks[0]->Create(Transform(1, 1));
	dst.memoryBlocks[0]->Create(Transform(2, 2));
	dst.memoryBlocks[0]->Create(Mesh(1));
	dst.memoryBlocks[0]->Create(Mesh(2));

	src.AddComponent(&dst, Transform(1, 1));

	return 0;
}
