#include <ostream>
#include "console_score_listener.hpp"

console_score_listener::console_score_listener(std::ostream& out_, std::string name_) :
	out(out_),
	name(std::move(name_))
{
}

console_score_listener::~console_score_listener() { }

void console_score_listener::score_updated(int) { }

void console_score_listener::died(int final_score) {
	out << name << " scored " << final_score << std::endl;
}
