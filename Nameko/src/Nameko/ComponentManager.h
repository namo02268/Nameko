#pragma once

class BaseComponentManager {
public:
	virtual ~BaseComponentManager() {}
};

template<typename Component>
class ComponentManager
{
public:
	ComponentManager() = default;
	~ComponentManager() = default;

private:

};
