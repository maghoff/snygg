#ifndef SNYGG_PLAYER_HPP
#define SNYGG_PLAYER_HPP

#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace ymse {
	class bindable_keyboard_handler;
}

class intersectable_with_circle;
class item_container;

class player : public boost::noncopyable {
	struct impl;
	boost::scoped_ptr<impl> d;

	void spawn_key(bool);

public:
	player(ymse::bindable_keyboard_handler&, item_container&);
	virtual ~player();

	void spawn();

	bool crashes_with(intersectable_with_circle&) const;
	void die();
	void score();
};

#endif
