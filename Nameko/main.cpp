#include "Nameko/Chunk.h"
#include "Nameko/Entity.h"

#include <chrono>

struct Transform {
public:
	Transform(float x, float y) : x(x), y(y) {
//		std::cout << "Trans Constructor" << std::endl;
	}
//	~Transform() { std::cout << "Trans Destructor" << std::endl; }

	float x;
	float y;
};

struct Mesh {
public:
	Mesh(float y) : y(y) {
//		std::cout << "Mesh Constructor" << std::endl;
	}

//	~Mesh() { std::cout << "Mesh Destructor" << std::endl; }

	float y;
};

int main() {
	using namespace Nameko;
	
	Chunk<Transform, Mesh> chunk;
	Transform t1(1, 1);
	Transform t2(2, 1);
	Transform t3(3, 1);
	Transform t4(4, 1);
	Mesh m1(1);
	Mesh m2(6);
	Mesh m3(7);
	Mesh m4(8);

	std::chrono::system_clock::time_point start, end;

	std::cout << "Initialize ECS" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 1024; i++) {
		chunk.AddComponents(t1, m1);
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	std::cout << "Initialize Vector" << std::endl;
	start = std::chrono::system_clock::now();
	std::vector<Transform> tVec;
	std::vector<Mesh> mVec;
	tVec.reserve(1024);
	mVec.reserve(1024);
	for (int i = 0; i < 1024; i++) {
		tVec.push_back(t1);
		mVec.push_back(m1);
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	float sum = 0;

	std::cout << "Iterate ECS" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 10240; i++) {
		chunk.IterateAll<Transform, Mesh>([&sum](Transform& trans, Mesh& mesh) {
			sum += (trans.x + mesh.y) * 0.5f;
			});
	}
	end = std::chrono::system_clock::now();
	std::cout << sum << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	sum = 0;

	std::cout << "Iterate Vector" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 10240; i++) {
		for (int j = 0; j < 1024; j++) {
			sum += (tVec[j].x + mVec[j].y) * 0.5;
		}
	}
	end = std::chrono::system_clock::now();
	std::cout << sum << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	/*
	chunk.IterateAll<Transform, Mesh>([](Transform& trans, Mesh& mesh){
		std::cout << trans.x << std::endl;
	});

	chunk.IterateAll<Mesh>([](Mesh& mesh) {
		std::cout << mesh.y << std::endl;
	});
	*/

	return 0;
}