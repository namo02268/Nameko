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
	
	Entity e = 0;

	SetEID(e, 8);
	SetCIID(e, 72);


	std::cout << GetEID(e) << std::endl;
	std::cout << GetCIID(e) << std::endl;

	return 0;
}