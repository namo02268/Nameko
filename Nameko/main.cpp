#include "Nameko/Chunk.h"
#include "Nameko/Entity.h"
#include "Nameko/IdGenerator.h"
#include "Nameko/Archtype.h"

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

template<size_t...>
struct seq{};

template<size_t N, size_t... S>
struct gens : gens<N -1, N - 1, S...> {};

template<size_t... S>
struct gens<0, S...> {
	typedef seq<S...> type;
};

template<size_t... S, typename... Args>
auto CallFunc(seq<S...>, std::tuple<Args...>&& params) {
	Nameko::Chunk<std::remove_reference_t<decltype(std::get<S>(params))>...> chunk;
}

template<typename... Args>
struct ChunkGenerator {
	ChunkGenerator(std::tuple<Args...> params) : params(params){}
	std::tuple<Args...> params;

	template<size_t... S>
	void callFunc(seq<S...>) {
		Nameko::Chunk<std::remove_reference_t<decltype(std::get<S>(params))>...> chunk;
	}

	void delayed_dispatch() {
		callFunc(typename gens<sizeof...(Args)>::type());
	}
};

int main() {
	using namespace Nameko;

	Archtype<Transform, Mesh> arche;
	Entity e1 = 1;
	Entity e2 = 2;
	Entity e3 = 3;
	Entity e4 = 4;
	arche.AddComponents(e1, Transform(1, 1), Mesh(1));
	arche.AddComponents(e2, Transform(2, 2), Mesh(2));
	arche.AddComponents(e3, Transform(3, 3), Mesh(3));
	arche.AddComponents(e4, Transform(4, 4), Mesh(4));

	std::cout << arche.GetComponent<Transform>(e1).x << std::endl;
	std::cout << arche.GetComponent<Transform>(e2).x << std::endl;
	std::cout << arche.GetComponent<Transform>(e3).x << std::endl;
	std::cout << arche.GetComponent<Transform>(e4).x << std::endl;

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
