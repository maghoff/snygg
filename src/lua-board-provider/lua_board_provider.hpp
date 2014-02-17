#ifndef SNYGG_BOARD_LUA_BOARD_PROVIDER_HPP
#define SNYGG_BOARD_LUA_BOARD_PROVIDER_HPP

#include <iosfwd>
#include <string>
#include "board_provider.hpp"

class file_loader;

class lua_board_provider : public board_provider {
	struct impl;
	std::unique_ptr<impl> d;

public:
	lua_board_provider(file_loader&, const std::string& name);
	~lua_board_provider();

	std::unique_ptr<segment> create_board() override;
	la::vec2f get_starting_position() override;
};

#endif
