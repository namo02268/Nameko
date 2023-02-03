#include "Nameko/Entity.h"
#include "Nameko/IdGenerator.h"
#include "Nameko/Archtype.h"

#include<chrono>

struct Transform {
public:
	Transform(float x, float y) : x(x), y(y) {
//		std::cout << "Transform Constructor" << std::endl;
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
//		std::cout << "Transform Destructor" << std::endl;
	}

	float x;
	float y;
};

struct Mesh {
public:
	Mesh(float y) : y(y) {}

	float y;
};

template<typename Tuple, size_t... S>
auto make_arche_impl(Tuple&& t, std::index_sequence<S...>) {
	return new Nameko::Archetype<std::remove_reference_t<decltype(std::get<S>(t))>...>;
}

template<typename Tuple>
auto make_arche(Tuple&& t) {
	return make_arche_impl(std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

int main() {
	using namespace Nameko;
	EntityManager eManager;

	Transform t1(1, 2);
	Mesh m1(1);

	auto arche = make_arche(std::tuple(t1, m1));

	auto e1 = eManager.CreateEntity();
	arche->AddComponents(e1, Transform(1, 1), Mesh(1));
	auto e2 = eManager.CreateEntity();
	arche->AddComponents(e2, Transform(2, 2), Mesh(2));
	auto e3 = eManager.CreateEntity();
	arche->AddComponents(e3, Transform(3, 3), Mesh(3));
	auto e4 = eManager.CreateEntity();
	arche->AddComponents(e4, Transform(4, 4), Mesh(4));
	auto e5 = eManager.CreateEntity();
	arche->AddComponents(e5, Transform(5, 5), Mesh(5));
	auto e6 = eManager.CreateEntity();
	arche->AddComponents(e6, Transform(6, 6), Mesh(6));
	auto e7 = eManager.CreateEntity();
	arche->AddComponents(e7, Transform(7, 7), Mesh(7));

	std::cout << arche->GetComponent<Transform>(e1).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e2).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e3).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e4).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e5).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e6).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e7).x << std::endl;

	std::cout << "------------------------------" << std::endl;

	arche->RemoveComponents(e1);

	std::cout << arche->GetComponent<Transform>(e2).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e3).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e4).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e5).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e6).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e7).x << std::endl;

	std::cout << "------------------------------" << std::endl;

	arche->RemoveComponents(e3);

	std::cout << arche->GetComponent<Transform>(e2).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e4).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e5).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e6).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e7).x << std::endl;

	std::cout << "------------------------------" << std::endl;

	arche->RemoveComponents(e7);

	std::cout << arche->GetComponent<Transform>(e2).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e4).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e5).x << std::endl;
	std::cout << arche->GetComponent<Transform>(e6).x << std::endl;


	delete arche;

	/*
	std::chrono::system_clock::time_point start, end;

	std::cout << "Initialize ECS" << std::endl;
	start = std::chrono::system_clock::now();
	chunk.AllocateBlock<Transform, Nameko::CHUNK_SIZE>();
	chunk.AllocateBlock<Mesh, Nameko::CHUNK_SIZE>();
	for (int i = 0; i < 1024; i++) {
		chunk.AddComponent(t1);
		chunk.AddComponent(m1);
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
		tVec.emplace_back(t1);
		mVec.emplace_back(m1);
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	float sum = 0.0f;

	std::cout << "Iterate ECS" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 10240; i++) {
		chunk.IterateAll<1024, Transform, Mesh>([&sum](Transform& trans, Mesh& mesh) {
			sum += (trans.x + mesh.y) * 0.5f;
			});
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	sum = 0.0f;

	std::cout << "Iterate Vector" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 10240; i++) {
		for (int j = 0; j < 1024; ++j) {
			sum += (tVec[j].x + mVec[j].y) * 0.5;
		}
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	*/

	return 0;
}
