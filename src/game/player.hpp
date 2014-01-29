#ifndef SNYGG_PLAYER_HPP
#define SNYGG_PLAYER_HPP

#include <memory>

namespace ymse {
	class bindable_keyboard_handler;
}

class intersectable_with_circle;
class item_container;
class board;

class player {
	struct impl;
	std::unique_ptr<impl> d;

public:
	player(ymse::bindable_keyboard_handler&, item_container&, board&, int left, int right, int spawn);
	virtual ~player();

	void spawn();

	bool crashes_with(intersectable_with_circle&) const;
	void die();
	void score();
};

#endif
