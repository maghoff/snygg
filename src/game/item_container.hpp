#ifndef ITEM_CONTAINER_HPP
#define ITEM_CONTAINER_HPP

#include <memory>

class renderable;
class crashable;
class movable;

class item_container {
public:
	virtual ~item_container();

	virtual void add_renderable(std::unique_ptr<renderable>&&) = 0;
	virtual void add_crashable(std::unique_ptr<crashable>&&) = 0;
	virtual void add_movable(std::unique_ptr<movable>&&) = 0;
};

#endif // ITEM_CONTAINER_HPP
