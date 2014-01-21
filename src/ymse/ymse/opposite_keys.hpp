#ifndef YMSE_OPPOSITE_KEYS_HPP
#define YMSE_OPPOSITE_KEYS_HPP

#include <boost/noncopyable.hpp>

namespace ymse {

class bindable_keyboard_handler;

class opposite_keys : public boost::noncopyable {
	bindable_keyboard_handler& kbd;
	int neg_key, pos_key;
	bool neg, pos;

protected:
	virtual void set(bool*, bool);

public:
	opposite_keys(bindable_keyboard_handler&, int neg, int pos);
	virtual ~opposite_keys();

	int val() const;
};

}

#endif
