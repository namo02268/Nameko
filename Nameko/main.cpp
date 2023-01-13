#include "Nameko/Chunk.h"
#include "Nameko/Entity.h"

struct Transform {
public:
	Transform(float x) : x(x) {
		std::cout << "Trans Constructor" << std::endl;
	}
	~Transform() { std::cout << "Trans Destructor" << std::endl; }

	float x;
};

struct Mesh {
public:
	Mesh(float y) : y(y) {
		std::cout << "Mesh Constructor" << std::endl;
	}

	~Mesh() { std::cout << "Mesh Destructor" << std::endl; }

	float y;
};

int main() {
	using namespace Nameko;
	
	Chunk<Transform, Mesh> chunk;
	Transform t1(10);
	Mesh m1(10);

	chunk.AddComponents(t1, m1);

	return 0;
}