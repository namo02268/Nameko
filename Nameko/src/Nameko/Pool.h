#pragma once

#include <memory>

namespace Nameko {
	class BasePool {
	public:
		virtual ~BasePool() {};
	};

	template<typename Component, size_t PoolSize>
	class Pool : public BasePool {
	private:
		Component* m_castedPtr;
		size_t m_index = 0;

	public:
		Pool(void* ptr) {
			std::cout << "Pool Constructor" << std::endl;
			m_castedPtr = static_cast<Component*>(ptr);
		}

		~Pool() {
			std::cout << "Pool Destructor" << std::endl;
			for (size_t i = 0; i < m_index; ++i) {
				(m_castedPtr + i)->~Component();
			}
		}

		void push_back(const Component&& component) {
			::new(m_castedPtr + m_index) Component(std::move(component));
			++m_index;
		}
	};
}
