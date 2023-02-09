#include "Nameko/Entity.h"
#include "Nameko/IdGenerator.h"
#include "Nameko/Archtype.h"
#include "Nameko/ECS.h"

#include <bitset>

#include<chrono>

struct Transform {
public:
	Transform(float x, float y) : x(x), y(y) {
//		std::cout << "Transform Constructor : " << this->x << std::endl;
	}

	Transform(const Transform& other) {
		this->x = other.x;
		this->y = other.y;
//		std::cout << "Transform Copy Constructor" << std::endl;
	}

	Transform(Transform&& other) noexcept {
		this->x = other.x;
		this->y = other.y;
//		std::cout << "Transform Move Constructor" << std::endl;
	}

	Transform& operator=(const Transform& other) {
		this->x = other.x;
		this->y = other.y;
//		std::cout << "Transform Copy= Constructor" << std::endl;
		return *this;
	}

	Transform& operator=(Transform&& other) noexcept {
		this->x = other.x;
		this->y = other.y;
//		std::cout << "Transform Move= Constructor" << std::endl;
		return *this;
	}



	~Transform() {
//		std::cout << "Transform Destructor : " << this->x << std::endl;
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
	Transform t1(3, 3);
	Mesh m1(1);
	auto e = eManager.CreateEntity();

	auto arche = new Archetype;

	std::chrono::system_clock::time_point start, end;

	std::cout << "Initialize ECS" << std::endl;
	start = std::chrono::system_clock::now();
	
	for (int i = 0; i < 8192; ++i) {
		e = eManager.CreateEntity();
		arche->AddComponents(e, Transform(i / 100, i / 100), Mesh(1));
	}

	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	std::cout << "Initialize ECS" << std::endl;
	std::vector<Transform> tVec;
	std::vector<Mesh> mVec;
	tVec.reserve(8192);
	mVec.reserve(8192);
	start = std::chrono::system_clock::now();

	for (int i = 0; i < 8192; ++i) {
		tVec.emplace_back(Transform(i / 100, i / 100));
		mVec.emplace_back(Mesh(1));
	}

	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	float sum = 0.0f;

	std::cout << "Iterate ECS" << std::endl;
	start = std::chrono::system_clock::now();

	for (int i = 0; i < 10000; ++i) {
		arche->IterateAll<Transform, Mesh>([&sum](Transform& trans, Mesh& mesh) {
			sum += (trans.x + mesh.y) * 0.5f;
		});
	}

	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	std::cout << "sum : " << sum << std::endl;

	sum = 0.0f;

	std::cout << "Iterate Vector" << std::endl;
	start = std::chrono::system_clock::now();

	for (int i = 0; i < 10000; ++i) {
		for (int j = 0; j < 8192; ++j) {
			sum += (tVec[j].x + mVec[j].y) * 0.5f;
		}
	}

	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	std::cout << "sum : " << sum << std::endl;

	delete arche;

	return 0;
}
