#ifndef ITEM_CONTAINER_HPP
#define ITEM_CONTAINER_HPP

#include <memory>

class item;

class item_container {
public:
	virtual ~item_container();

	virtual void add_item(std::auto_ptr<item>) = 0;
};

#endif // ITEM_CONTAINER_HPP
