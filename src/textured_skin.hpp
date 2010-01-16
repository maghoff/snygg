#ifndef TEXTURED_SKIN_HPP
#define TEXTURED_SKIN_HPP

#include <string>
#include <boost/scoped_ptr.hpp>
#include "scalable_skin.hpp"

class textured_skin : public scalable_skin {
	struct impl;
	boost::scoped_ptr<impl> d;

	void cap_test(ymse::vec2f, float);
	void beautiful_cap_test(float base_ang);
	void half_cap_test(ymse::vec2f p, float base_ang);

	void stick_test(float base_ang, ymse::vec2f c);

public:
	textured_skin(const std::string& path);
	~textured_skin();

	void circle(ymse::vec2f p, float r);
	void blood(ymse::vec2f p, float r);

	void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end);

	void finish_frame(ymse::rectf bb);
};

#endif // TEXTURED_SKIN_HPP
