#include <cmath>
#include "arc.hpp"
#include "line.hpp"
#include "board.hpp"

namespace {

void box(boost::ptr_vector<segment>& b, float x1, float y1, float w, float h, float r) {
	float x2 = x1 + w, y2 = y1 + h;

	b.push_back(new line(x1 + r, y1    ,  1,  0, w - 2*r));
	b.push_back(new arc (x2 - r, y1 + r,  r,  M_PI * -0.5, M_PI *  0.0, 1));
	b.push_back(new line(x2    , y1 + r,  0,  1, h - 2*r));
	b.push_back(new arc (x2 - r, y2 - r,  r,  M_PI *  0.0, M_PI *  0.5, 1));
	b.push_back(new line(x2 - r, y2    , -1,  0, w - 2*r));
	b.push_back(new arc (x1 + r, y2 - r,  r,  M_PI *  0.5, M_PI *  1.0, 1));
	b.push_back(new line(x1    , y2 - r,  0, -1, h - 2*r));
	b.push_back(new arc (x1 + r, y1 + r,  r,  M_PI * -1.0, M_PI * -0.5, 1));
}

}


board::board() :
	b(8)
{
	box(b, -200, -50, 400, 100, 10);
}

board::~board() {
}

void board::render(skin& s) const {
	typedef boost::ptr_vector<segment>::const_iterator it;
	for (it i = b.begin(); i != b.end(); ++i) i->render(s);
}
