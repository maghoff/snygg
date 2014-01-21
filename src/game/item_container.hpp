#ifndef ITEM_CONTAINER_HPP
#define ITEM_CONTAINER_HPP

#include <memory>

class item;
class renderable;

class item_container {
public:
	virtual ~item_container();

	virtual void add_item(std::unique_ptr<item>&&) = 0;
	virtual void add_renderable(std::unique_ptr<renderable>&&) = 0;
};

#endif // ITEM_CONTAINER_HPP
