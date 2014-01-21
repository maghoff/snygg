#ifndef YMSE_OPPOSITE_KEYS_LISTENER_HPP
#define YMSE_OPPOSITE_KEYS_LISTENER_HPP

namespace ymse {

class opposite_keys_listener {
public:
	opposite_keys_listener();
	virtual ~opposite_keys_listener();

	virtual void set_val(int) = 0;
};

} // namespace ymse

#endif // YMSE_OPPOSITE_KEYS_LISTENER_HPP
