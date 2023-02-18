#include "Nameko/Entity.h"
#include "Nameko/IdGenerator.h"
#include "Nameko/Archtype.h"
#include "Nameko/ECS.h"

#include <bitset>

#include<chrono>

#include <map>
#include <typeinfo>
#include <typeindex>

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
	
	auto ecs = new ECS;
	auto e1 = ecs->CreateEntity();
	auto e2 = ecs->CreateEntity();
	auto e3 = ecs->CreateEntity();
	auto e4 = ecs->CreateEntity();
	auto e5 = ecs->CreateEntity();

	ecs->AddComponent<Transform>(e1, Transform(1, 1));
	ecs->AddComponent<Mesh>(e1, Mesh(1));

	ecs->AddComponent<Transform>(e2, Transform(2, 2));
	ecs->AddComponent<Mesh>(e2, Mesh(2));

	ecs->AddComponent<Transform>(e3, Transform(3, 3));
	ecs->AddComponent<Mesh>(e3, Mesh(3));

	ecs->AddComponent<Transform>(e4, Transform(4, 4));
	ecs->AddComponent<Mesh>(e4, Mesh(4));
	ecs->RemoveComponent<Transform>(e4);
	ecs->AddComponent<Collider>(e4, Collider(4));

	ecs->AddComponent<Transform>(e5, Transform(5, 5));
	ecs->AddComponent<Mesh>(e5, Mesh(5));
	ecs->AddComponent<Collider>(e5, Collider(5));

	float sum = 0.0f;
	ecs->Each<Transform, Mesh>([&sum](Transform& trans, Mesh& mesh) {
		sum += (trans.x + mesh.y) * 0.5;
	});
	std::cout << "sum : " << sum << std::endl;

	delete ecs;
}


/*
int main() {
	using namespace Nameko;

	auto ecs = new ECS;
	EntityManager eManager;

	std::chrono::system_clock::time_point start, end;

	std::cout << "Initialize ECS" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 9000; i++) {
		auto e = eManager.CreateEntity();
		ecs->AddComponent<Transform>(e, Transform(i, i));
		ecs->AddComponent<Mesh>(e, Mesh(i));
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	std::cout << "Initialize Vector" << std::endl;
	start = std::chrono::system_clock::now();
	std::vector<Transform> tVec;
	std::vector<Mesh> mVec;
	tVec.reserve(9000);
	mVec.reserve(9000);
	for (int i = 0; i < 9000; i++) {
		tVec.emplace_back(Transform(i, i));
		mVec.emplace_back(Mesh(i));
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;


	float sum = 0.0f;
	std::cout << "Iterate ECS" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 10000; i++) {
		ecs->Each<Transform, Mesh>([&sum](Transform& trans, Mesh& mesh) {
			sum += (trans.x + mesh.y) * 0.00005f;
		});
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	std::cout << "sum : " << sum << std::endl;

	sum = 0.0f;
	std::cout << "Iterate Vector" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 9000; j++) {
			sum += (tVec[j].x + mVec[j].y) * 0.00005f;
		}
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	std::cout << "sum : " << sum << std::endl;

	delete ecs;

	return 0;
}
*/