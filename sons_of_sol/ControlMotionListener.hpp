#ifndef CONTROLMOTIONLISTENER_HPP_
#define CONTROLMOTIONLISTENER_HPP_

/*
 * Author: jrahm
 * created: 2013/11/01
 * ControlMotionListener.hpp: <description>
 */

#include "sons_of_sol/ControlMotionEvent.hpp"

class ControlMotionListener {
public:
	/* Called once there is a control motion */
	virtual inline void onControlMotion( const ControlMotionEvent& event ) {
		(void) event;
	}
};

#endif /* CONTROLMOTIONLISTENER_HPP_ */
