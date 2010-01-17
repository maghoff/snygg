#ifndef TEXTURED_SKIN_HPP
#define TEXTURED_SKIN_HPP

#include <string>
#include <boost/scoped_ptr.hpp>
#include "scalable_skin.hpp"

class textured_skin : public scalable_skin {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	textured_skin(const std::string& path);
	~textured_skin();

	void circle(ymse::vec2f p, float r);
	void blood(ymse::vec2f p, float r);

	void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end);

	void cap_front(ymse::vec2f p, float direction, float b_coord);
	void cap_back(ymse::vec2f p, float direction, float b_coord);

	void finish_frame(ymse::rectf bounding_box);
};

#endif // TEXTURED_SKIN_HPP
