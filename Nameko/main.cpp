#include "Nameko/Entity.h"
#include "Nameko/IdGenerator.h"
#include "Nameko/Archtype.h"
#include "Nameko/ECS.h"

#include<chrono>

struct Transform {
	Transform(float x, float y) : x(x), y(y) {
//		std::cout << "Transform Constructor : " << this->x << std::endl;
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

class Updator : public Nameko::System {
public:
	Updator() { std::cout << "Updator Constructor" << std::endl; }
	~Updator() { std::cout << "Updator Destructor" << std::endl; }

	void Init() override {
		std::cout << "Init Updator" << std::endl;
	}

	void Update(float dt) override {
		std::cout << "Update Updator" << std::endl;
		m_ecs->EachComponent<Transform>([](Transform& trans) {
			trans.x += 10;
		});
	}

	void Draw() override {
		std::cout << "Draw Updator" << std::endl;
	}
};

class MyECS : public Nameko::ECS {
public:
	MyECS() = default;
	~MyECS() = default;

	Nameko::Entity CreateEntity() override {
		std::cout << "Create Entity" << std::endl;
		return m_entityManager->CreateEntity();
	}
};

int main() {
	using namespace Nameko;

	auto ecs = new MyECS;
	ecs->AddSystem(std::make_unique<Updator>());

	auto e1 = ecs->CreateEntity();
	ecs->AddComponent<Transform>(e1, Transform(1, 1));
	ecs->AddComponent<Mesh>(e1, Mesh(1));

	auto e2 = ecs->CreateEntity();
	ecs->AddComponent<Transform>(e2, Transform(2, 2));
	ecs->AddComponent<Mesh>(e2, Mesh(2));

	auto e3 = ecs->CreateEntity();
	ecs->AddComponent<Transform>(e3, Transform(3, 3));
	ecs->AddComponent<Mesh>(e3, Mesh(3));

	auto e4 = ecs->CreateEntity();
	ecs->AddComponent<Transform>(e4, Transform(4, 4));
	ecs->AddComponent<Mesh>(e4, Mesh(4));

	auto e5 = ecs->CreateEntity();
	ecs->AddComponent<Transform>(e5, Transform(5, 5));
	ecs->AddComponent<Mesh>(e5, Mesh(5));

	ecs->DestoryEntity(e1);

	ecs->EachEntity<Transform, Mesh>([](Entity& entity){
		std::cout << entity << std::endl;
	});

	delete ecs;

	return 0;
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
	std::cout << "IterateComponent ECS" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 10000; i++) {
		ecs->EachComponent<Transform, Mesh>([&sum](Transform& trans, Mesh& mesh) {
			sum += (trans.x + mesh.y) * 0.00005f;
		});
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	std::cout << "sum : " << sum << std::endl;

	sum = 0.0f;
	std::cout << "IterateComponent Vector" << std::endl;
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