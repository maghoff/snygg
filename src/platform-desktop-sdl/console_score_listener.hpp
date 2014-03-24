#ifndef CONSOLE_SCORE_LISTENER_HPP
#define CONSOLE_SCORE_LISTENER_HPP

#include <string>
#include <iosfwd>
#include <score_listener.hpp>

class console_score_listener : public score_listener {
	std::ostream& out;
	std::string name;

public:
	console_score_listener(std::ostream&, std::string);
    ~console_score_listener() override;

	void score_updated(int) override;
	void died(int) override;
};

#endif
