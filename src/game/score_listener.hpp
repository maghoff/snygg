#ifndef SCORE_LISTENER_HPP
#define SCORE_LISTENER_HPP

class score_listener {
public:
	virtual ~score_listener();

	virtual void score_updated(int current_score) = 0;
	virtual void died(int final_score) = 0;
};

#endif
