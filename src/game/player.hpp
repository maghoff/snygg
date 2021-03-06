#ifndef SNYGG_PLAYER_HPP
#define SNYGG_PLAYER_HPP

#include <memory>

namespace game {
	class bindable_keyboard_handler;
}

class intersectable_with_circle;
class item_container;
class board;
class score_listener;

class player {
	struct impl;
	std::unique_ptr<impl> d;

public:
	player(
		game::bindable_keyboard_handler&, item_container&, board&, score_listener& score_reporter,
		int left, int right, int spawn
	);
	virtual ~player();

	void spawn();

	bool crashes_with(const intersectable_with_circle&) const;
	void die();
	void score();
};

#endif
