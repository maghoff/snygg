#ifndef SNYGG_GROWTH_POLICY_HPP
#define SNYGG_GROWTH_POLICY_HPP

#include <memory>

namespace ymse {
	template <int Size, typename Type> class vec;
	typedef vec<2, float> vec2f;
}

class segment;
class segment_sequence;

typedef std::auto_ptr<segment> segment_ptr;

class growth_policy {
public:
	growth_policy();
	virtual ~growth_policy();

	virtual segment_ptr growth_segment(ymse::vec2f pos, ymse::vec2f dir, float length) = 0;
	virtual void grow(segment_sequence& body, float length) = 0;
};

#endif // SNYGG_GROWTH_POLICY_HPP
