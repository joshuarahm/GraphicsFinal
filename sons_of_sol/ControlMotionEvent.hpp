#ifndef CONTROLMOTIONEVENT_HPP_
#define CONTROLMOTIONEVENT_HPP_

/*
 * Author: jrahm
 * created: 2013/11/01
 * ControlMotionEvent.hpp: <description>
 */

/* This is the class used to 
 * signal an event from the controller.
 *
 * These values have been normalized */
class ControlMotionEvent {
public:
	enum MotionEventType {
		  NONE
		, PITCH
		, ROLL
		, YAW
		, THROTTLE
		, STRAFE_SIDE
		, STRAFE_UP
		, FIRE_PRIMARY
	};

	enum MotionOrigin {
		JOYSTICK,
		MOUSE,
		BUTTON
	};

	ControlMotionEvent( MotionEventType type, double mag, MotionOrigin origin ) :
		mag( mag ), type( type ), origin( origin ) {}

	inline double getMagnitude() const {
		return mag;
	}

	inline MotionEventType getType() const {
		return type;
	}

	inline MotionOrigin getOrigin() const {
		return origin;
	}

	inline void setMagnitude( double mag ) {
		this->mag = mag;
	}

	inline void setType( MotionEventType type ) {
		this->type = type;
	}

	inline void setOrigin( MotionOrigin origin ) {
		this->origin = origin;
	}
private:
	double mag;
	MotionEventType type;
	MotionOrigin origin;
};

#endif /* CONTROLMOTIONEVENT_HPP_ */
