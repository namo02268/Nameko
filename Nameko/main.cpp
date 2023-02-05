#include "Nameko/Entity.h"
#include "Nameko/IdGenerator.h"
#include "Nameko/Archtype.h"
#include "Nameko/ECS.h"

#include <bitset>

#include<chrono>

struct Transform {
public:
	Transform(float x, float y) : x(x), y(y) {
		std::cout << "Transform Constructor" << std::endl;
	}

	Transform(const Transform& other) {
		this->x = other.x;
		this->y = other.y;
		std::cout << "Transform Copy Constructor" << std::endl;
	}

	Transform(Transform&& other) noexcept {
		this->x = other.x;
		this->y = other.y;
		std::cout << "Transform Move Constructor" << std::endl;
	}

	Transform& operator=(const Transform& other) {
		this->x = other.x;
		this->y = other.y;
		std::cout << "Transform Copy= Constructor" << std::endl;
		return *this;
	}

	Transform& operator=(Transform&& other) noexcept {
		this->x = other.x;
		this->y = other.y;
		std::cout << "Transform Move= Constructor" << std::endl;
		return *this;
	}



	~Transform() {
		std::cout << "Transform Destructor" << std::endl;
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
//	Transform t1(3, 3);
//	Mesh m1(1);
	auto e = eManager.CreateEntity();

	ECS* ecs = new ECS;
	ecs->AddComponents(e, Transform(3, 3), Mesh(1));
//	ecs->AddComponentsPtr(e, &t1, &m1);
	delete ecs;

	/*
	auto arche = make_arche(std::tuple(t1, m1));

	arche->AddComponents(e, Transform(3, 3), Mesh(3));
	std::cout << std::get<Transform&>(arche->GetComponents(e)).x << std::endl;
	std::get<Transform&>(arche->GetComponents(e)).x = 5;
	std::cout << std::get<Transform&>(arche->GetComponents(e)).x << std::endl;

	delete arche;
	*/
	/*
	std::chrono::system_clock::time_point start, end;

	std::cout << "Initialize ECS" << std::endl;
	start = std::chrono::system_clock::now();


	for (int i = 0; i < 4096; i++) {
		arche->AddComponents(e, Transform(3, 3), Mesh(1));
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	std::cout << "Initialize Vector" << std::endl;
	start = std::chrono::system_clock::now();
	std::vector<Transform> tVec;
	std::vector<Mesh> mVec;
	tVec.reserve(4096);
	mVec.reserve(4096);
	for (int i = 0; i < 4096; i++) {
		tVec.emplace_back(t1);
		mVec.emplace_back(m1);
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	float sum = 0.0f;

	std::cout << "Iterate ECS" << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < 10240; i++) {
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
	for (int i = 0; i < 10240; i++) {
		for (int j = 0; j < 4096; ++j) {
			sum += (tVec[j].x + mVec[j].y) * 0.5;
		}
	}
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	std::cout << "sum : " << sum << std::endl;

	*/
	return 0;
}
