#include <iostream>
#include <tuple>

struct Transform {
	Transform(float x, float y) : x(x), y(y) {
		std::cout << "Transform Constructor : " << x << std::endl;
	}

	// Copy Constructor
	Transform(const Transform& other) : x(other.x), y(other.y) {
		std::cout << "Transform Copy Constructor : " << x << std::endl;
	}

	// Move Constructor
	Transform(Transform&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)) {
		std::cout << "Transform Move Constructor : " << x << std::endl;
	}

	Transform& operator=(const Transform& other) {
		std::cout << "Transform Copy Constructor : " << x << std::endl;
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	~Transform() {
		std::cout << "Transform Destructor : " << x << std::endl;
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

template < class... >
struct tuple_add {};
template < class... Tuple, class... Args >
struct tuple_add < std::tuple < Tuple... >, Args... >
{
	using type = std::tuple < Tuple..., Args... >;
};

template<typename... Types>
class ArcheType {
	using my_types = std::tuple<Types...>;

	template<typename Type>
	using added_type = tuple_add<Types..., Type>;
	
public:
	ArcheType() = default;
	~ArcheType() = default;

	template<typename Type>
	ArcheType<Types..., Type>* Create(Type) {
		auto arche = new ArcheType<Types..., Type>();
		return arche;
	}
};


int main() {
	ArcheType<int, char> arche;
	arche.Create(1.0f);

	return 0;
}
