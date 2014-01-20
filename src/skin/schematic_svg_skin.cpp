#include <algorithm>
#include <cmath>
#include <sstream>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "complex_polygon.hpp"
#include "schematic_svg_skin.hpp"

namespace {
	const float ARROW_LENGTH = 4.;
	const float ARROW_WIDTH = 1.2;

	const char css[] =
		".backdrop {"
			"fill: black;"
		"}"

		"path.floor {"
			"fill: rgba(158, 158, 158, 0.3);"
			"stroke: rgba(158, 158, 158, 0.3);"
			"stroke-width: 0.25;"
		"}"

		"circle.eats {"
			"stroke-width: 0.25;"
			"stroke: #ffffff;"
			"fill: rgba(255, 255, 255, 0.3);"
		"}"
		"circle.blood-stain {"
			"stroke-width: 0.25;"
			"stroke: #ff0000;"
			"fill: rgba(255, 0, 0, 0.3);"
		"}"

		"path.line {"
			"stroke-width: 0.25;"
			"stroke: #b3b3ff;"
			"fill: rgba(178, 178, 255, 0.3);"
		"}"
		".line>.arrow {"
			"stroke-width: 0.25;"
			"stroke: #b3b3ff;"
		"}"
		"path.arc {"
			"stroke-width: 0.25;"
			"stroke: #b3ffb3;"
			"fill: rgba(178, 255, 178, 0.3);"
		"}"
		".arc>.arrow {"
			"stroke-width: 0.25;"
			"stroke: #b3ffb3;"
		"}"
		"path.cap {"
			"stroke-width: 0.25;"
			"stroke: #b3ffff;"
			"fill: rgba(178, 255, 255, 0.3);"
		"}"
	;

	static std::ostream& operator << (std::ostream& out, const ymse::vec2f& v) {
		return out << v.x() << ',' << v.y();
	}
}

schematic_svg_skin::schematic_svg_skin(std::ostream& out_, const ymse::rectf& bounding_box) :
	out(out_)
{
	float zoom_factor = 4.;
	float padding = 5.;
	float width = (bounding_box.width() + padding*2.) * zoom_factor;
	float height = (bounding_box.height() + padding*2.) * zoom_factor;

	out <<
		"<svg xmlns='http://www.w3.org/2000/svg' version='1.1' width='" << width << "' height='" << height << "'>"
		"<style type='text/css'><![CDATA[" << css << "]]></style>"
		"<g transform='"
			"scale(" << zoom_factor << ", " << -zoom_factor << ") "
			"translate(" << -bounding_box.left() + padding << ", " << -bounding_box.bottom() - padding << ")"
		"'>"
		"<path class='backdrop' d='"
			"M" << bounding_box.left() - padding << "," << bounding_box.top() - padding << " "
			"L" << bounding_box.right() + padding << "," << bounding_box.top() - padding << " "
			"L" << bounding_box.right() + padding << "," << bounding_box.bottom() + padding << " "
			"L" << bounding_box.left() - padding << "," << bounding_box.bottom() + padding << " "
		"Z'/>"
	;
}

schematic_svg_skin::~schematic_svg_skin() {
	for (; !output_stack.empty(); output_stack.pop()) {
		out << output_stack.top();
	}
	out << "</g></svg>";
}

void schematic_svg_skin::circle(ymse::vec2f p, float r) {
	std::stringstream ss;
	ss << "<circle class='eats' cx='" << p.x() << "' cy='" << p.y() << "' r='" << r << "'/>";
	output_stack.push(ss.str());
}

void schematic_svg_skin::blood(ymse::vec2f p, float r) {
	std::stringstream ss;
	ss << "<circle class='blood-stain' cx='" << p.x() << "' cy='" << p.y() << "' r='" << r << "'/>";
	output_stack.push(ss.str());
}

void schematic_svg_skin::fat_arc(ymse::vec2f p, float r, float t, float begin_in, float end_in, float b_begin, float b_end) {
	float r1 = r-t, r2 = r+t;

	float begin = begin_in, end = end_in;
	if (begin > end) std::swap(begin, end);

	float &x(p[0]), &y(p[1]);

	int large_arc_flag = (end - begin) > M_PI ? 1 : 0;

	std::stringstream ss;
	ss << "<g class='arc'>";
	ss << "<path class='arc' d='"
		"M" << x + r1 * cos(begin) << "," << y + r1 * sin(begin) << " "
		"A" << r1 << "," << r1 << " 0 " << large_arc_flag << ",1 " << x + r1 * cos(end) << "," << y + r1 * sin(end) << " "
		"L" << x + r2 * cos(end) << "," << y + r2 * sin(end) << " "
		"A" << r2 << "," << r2 << " 0 " << large_arc_flag << ",0 " << x + r2 * cos(begin) << "," << y + r2 * sin(begin) << " "
		"Z'/>";

	if ((end - begin) * r > ARROW_LENGTH * 1.25) {
		const float dir = begin_in < end_in ? 1. : -1.;
		const float hrl = dir * (ARROW_LENGTH / 2. / r);
		const float center = (begin + end) / 2.;
		const float arrow_begin = center - hrl;
		const float arrow_end = center + hrl;
		const float arrowhead_end = center + hrl - dir * ARROW_WIDTH / r;

		int sweep_flag = begin_in < end_in ? 1 : 0;
		ss << "<path class='arrow' d='"
			"M" << x + r * cos(arrow_begin) << "," << y + r * sin(arrow_begin) << " "
			"A" <<
				r << ',' << r << ' ' <<
				"0 0 " << sweep_flag << " " <<
				x + r * cos(arrow_end) << "," << y + r * sin(arrow_end) << " "
			"M" << x + r * cos(arrow_end) << "," << y + r * sin(arrow_end) << " "
			"L" << x + (r - ARROW_WIDTH) * cos(arrowhead_end) << "," << y + (r - ARROW_WIDTH) * sin(arrowhead_end) << " "
			"M" << x + r * cos(arrow_end) << "," << y + r * sin(arrow_end) << " "
			"L" << x + (r + ARROW_WIDTH) * cos(arrowhead_end) << "," << y + (r + ARROW_WIDTH) * sin(arrowhead_end) << " "
			"'/>";
	}
	ss << "</g>";
	output_stack.push(ss.str());
}

void schematic_svg_skin::fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end) {
	float &x1(p[0]), &y1(p[1]), &dx(d[0]), &dy(d[1]);

	// Calculate normal * thickness:
	float nx = dy * t, ny = -dx * t;
	float x2 = x1 + dx*len, y2 = y1 + dy * len;

	std::stringstream ss;
	ss << "<g class='line'>";

	ss << "<path class='line' d='"
		"M" << x1+nx << "," << y1+ny << " "
		"L" << x2+nx << "," << y2+ny << " "
		"L" << x2-nx << "," << y2-ny << " "
		"L" << x1-nx << "," << y1-ny << " "
		"Z'/>";

	if (len > ARROW_LENGTH * 1.25) {
		const float hll = ARROW_LENGTH / 2.;
		const ymse::vec2f center = p + d * (len / 2.f);
		const ymse::vec2f long_normal = ymse::vec2f(nx, ny);
		const ymse::vec2f normal = long_normal * (1.f / long_normal.length());

		ss << "<path class='arrow' d='"
			"M" << center - hll * d << " "
			"L" << center + hll * d << " "
			"M" << center + hll * d << " "
			"L" << center + (hll - ARROW_WIDTH) * d + ARROW_WIDTH * normal << " "
			"M" << center + hll * d << " "
			"L" << center + (hll - ARROW_WIDTH) * d - ARROW_WIDTH * normal <<
			"'/>"
		;
	}
	ss << "</g>";
	output_stack.push(ss.str());
}

void schematic_svg_skin::cap(ymse::vec2f p, float snake_direction_in, float cap_direction_in, float) {
	const float r = 2.5;
	float snake_dir = snake_direction_in - M_PI*0.5;
	float cap_dir = cap_direction_in - M_PI*0.5;

	std::stringstream ss;
	ss << "<path class='cap' d='"
		"M" << p[0] + r*cos(snake_dir + cap_dir) << "," << p[1] + r*sin(snake_dir + cap_dir) << " "
		"A" << r << "," << r << " "
			"0 0 1" <<
			p[0] + r*cos(snake_dir + cap_dir + M_PI) << "," << p[1] + r*sin(snake_dir + cap_dir + M_PI) << " "
		"Z'/>";
	output_stack.push(ss.str());
}

void schematic_svg_skin::floor(const complex_polygon& floor_poly) {
	std::stringstream ss;
	ss << "<g class='floor'>";

	const std::vector<ymse::vec2f>& p = floor_poly.points;
	const std::vector<int>& t = floor_poly.triangles;

	for (unsigned i = 0; i < t.size(); i += 3) {
		ss << "<path class='floor' d='"
			"M" << p[t[i + 0]].x() << "," << p[t[i + 0]].y() << " "
			"L" << p[t[i + 1]].x() << "," << p[t[i + 1]].y() << " "
			"L" << p[t[i + 2]].x() << "," << p[t[i + 2]].y() << " "
			"Z'/>";
	}
	ss << "</g>";
	output_stack.push(ss.str());
}
