#ifndef SNYGG_RENDERABLE_HPP
#define SNYGG_RENDERABLE_HPP

class skin;

class renderable {
public:
	virtual ~renderable();

	virtual void render(skin&) const = 0;
};

#endif
