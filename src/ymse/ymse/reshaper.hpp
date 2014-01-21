#ifndef YMSE_RESHAPER_HPP
#define YMSE_RESHAPER_HPP

namespace ymse {

class reshaper {
public:
	reshaper();
	virtual ~reshaper();

	virtual void reshape(int width, int height) = 0;
};

}

#endif

