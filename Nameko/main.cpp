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
auto make_chunk_impl(Tuple&& t, std::index_sequence<S...>) {
	return new Nameko::Chunk<Nameko::CHUNK_SIZE, std::remove_reference_t<decltype(std::get<S>(t))>...>;
}

template<typename Tuple>
auto make_chunk(Tuple&& t) {
	return make_chunk_impl(std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

int main() {
	using namespace Nameko;
	EntityManager eManager;

	Transform t1(1, 2);
	Mesh m1(1);

	auto test = std::make_tuple(t1, m1);

	auto chunk = make_chunk(test);

	chunk->Add(std::move(t1), std::move(m1));

	delete chunk;

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
