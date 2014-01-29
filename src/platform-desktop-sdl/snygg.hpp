#ifndef SNYGG_SNYGG_HPP
#define SNYGG_SNYGG_HPP

#include <memory>
#include <ymse/game.hpp>
#include <ymse/reshaper.hpp>
#include "../game/item_container.hpp"

namespace ymse {
	class sdl_core;
}

class scalable_skin;

class snygg : public ymse::game, ymse::reshaper, public item_container {
	struct impl;
	std::unique_ptr<impl> d;

	void set_skin_key(scalable_skin*);

	void reshape(int width, int height);

	void take_screenshot(const std::string& filename, unsigned, unsigned w, unsigned h);
	void screenshot_with_skin(const std::string& filename, scalable_skin*);
	void screenshot_key();

public:
	snygg(const std::string& board_filename);
	~snygg();

	void attach_to_core(ymse::sdl_core&);

	void render();
	void tick();

	void add_item(std::unique_ptr<item>&&);
	void add_renderable(std::unique_ptr<renderable>&&);
};

#endif

