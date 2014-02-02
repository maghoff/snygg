#ifndef SNYGG_SNYGG_HPP
#define SNYGG_SNYGG_HPP

#include <memory>
#include <tick_handler.hpp>
#include <reshaper.hpp>
#include "../game/item_container.hpp"

namespace game {
	class sdl_core;
}

class scalable_skin;

class snygg : public game::tick_handler, game::reshaper, public item_container {
	struct impl;
	std::unique_ptr<impl> d;

	void set_skin_key(scalable_skin*);

	void reshape(int width, int height) override;

	void take_screenshot(const std::string& filename, unsigned, unsigned w, unsigned h);
	void screenshot_with_skin(const std::string& filename, scalable_skin*);
	void screenshot_key();

public:
	snygg(const std::string& board_filename);
	~snygg();

	void attach_to_core(game::sdl_core&);

	void render() override;
	void tick_10ms() override;

	void add_item(std::unique_ptr<item>&&);
	void add_renderable(std::unique_ptr<renderable>&&);
};

#endif

