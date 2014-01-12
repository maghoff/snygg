#ifndef ITEM_CONTAINER_HPP
#define ITEM_CONTAINER_HPP

#include <memory>

class item;
class renderable;

typedef std::auto_ptr<item> item_ptr;
typedef std::auto_ptr<renderable> renderable_ptr;

class item_container {
public:
	virtual ~item_container();

	virtual void add_item(item_ptr) = 0;
	virtual void add_renderable(renderable_ptr) = 0;
};

#endif // ITEM_CONTAINER_HPP
