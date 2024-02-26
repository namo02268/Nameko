#pragma once

class BasePool {
private:
	void* m_ptr;
	size_t m_size;
	size_t m_chunkSize;
	size_t m_elementSize;
	size_t m_totalSize;

public:
	BasePool(void* ptr, size_t chunkSize, size_t elementSize) {
		m_ptr = ptr;
		m_size = 0;
		m_chunkSize = chunkSize;
		m_elementSize = elementSize;
		m_totalSize = chunkSize * elementSize;
	}
	virtual ~BasePool() = default;

	virtual void swap(BasePool* other) = 0;
};

template<size_t ChunkSize, class Component>
class Pool : public BasePool {
private:
	Component* m_castedPtr;

public:
	Pool(void* ptr) : BasePool(ptr, ChunkSize, sizeof(Component)) {
		m_castedPtr = static_cast<Component*>(ptr);
	}

	Component& at(size_t n) {
		return m_castedPtr[n];
	}

	void remove(size_t n) {

	}

	void swap(BasePool* other) {

	}
};