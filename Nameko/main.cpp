#include "Nameko/Entity.h"
#include "Nameko/IdGenerator.h"
#include "Nameko/Archtype.h"
#include "Nameko/ECS.h"

#include <bitset>

#include<chrono>

struct Transform {
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
	Mesh(float y) : y(y) {}
	float y;
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
//	Transform t1(3, 3);
//	Mesh m1(1);
	EntityManager eManager;

	auto ecs = new ECS;

	auto e = eManager.CreateEntity();
	ecs->AddComponents(e, Transform(1, 1), Mesh(1));
	ecs->AddComponents(e, Vertex());

	delete ecs;


	/*
	auto arche = new Archetype;

	for (int i = 0; i < 8192; ++i) {
		e = eManager.CreateEntity();
		Vertex vertex{};
		arche->AddComponents(e, Transform(1, 1), Mesh(1), std::move(vertex));
	}

	std::vector<Transform> tVec;
	std::vector<Mesh> mVec;
	std::vector<Model> modelVec;
	tVec.reserve(8192);
	mVec.reserve(8192);
	modelVec.reserve(8192);

	for (int i = 0; i < 8192; ++i) {
		tVec.emplace_back(Transform(1, 1));
		mVec.emplace_back(Mesh(1));
		modelVec.emplace_back(Model());
	}

	std::chrono::system_clock::time_point start, end;

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
	std::cout << "Iterate AoS" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 10000; ++i) {
		for (int j = 0; j < 8192; ++j) {
			sum += (modelVec[j].transform.x + modelVec[j].mesh.y) * 0.5f;
		}
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	std::cout << "sum : " << sum << std::endl;


	sum = 0.0f;
	std::cout << "Iterate SoA" << std::endl;
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
	*/

	return 0;
}
