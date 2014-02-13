#ifndef SNYGG_BOARD_BOARD_PROVIDER_HPP
#define SNYGG_BOARD_BOARD_PROVIDER_HPP

#include <memory>
#include <vec_fwd.hpp>

class segment;

class board_provider {
public:
	virtual ~board_provider() = default;

	virtual std::unique_ptr<segment> create_board() = 0;
	virtual la::vec2f get_starting_position() = 0;
};

#endif
