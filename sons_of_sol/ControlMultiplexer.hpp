#ifndef CONTROLMULTIPLEXER_HPP_
#define CONTROLMULTIPLEXER_HPP_

/*
 * Author: jrahm
 * created: 2013/11/01
 * ControlMultiplexer.hpp:
 *
 * Listens to multiple events and multiplexes them
 * into a single movement event for the ship
 */

#include "slox/events/SloxKeyListener.hpp"
#include "slox/events/SloxMouseMotionListener.hpp"

#include "sons_of_sol/ControlMotionEvent.hpp"
#include "sons_of_sol/ControlMotionListener.hpp"

#include <map>
#include <vector>

class ControlMultiplexer :
	public slox::SloxKeyListener,
	public slox::SloxMouseMotionListener {
public:
	/* Construct a new control multiplexer */
	ControlMultiplexer();

	/* Adds a listener to this ControlMultiplexer */
	inline void addControlMotionListener( ControlMotionListener* listener ) {
		m_motion_listeners.push_back( listener );
	}

	/* Called when a key is released. This event will be converted
	 * into ControlMotionEvent and sent */
	virtual void onKeyUp( const SDL_KeyboardEvent& evt ) ;

	/* Called when there was a key press. This event may be converted
	 * into ControlMotionEvent which is then sent off to all
	 * the listeners. If there is no mapping then the motion
	 * event is not sent */
	virtual void onKeyDown( const SDL_KeyboardEvent& evt ) ;

	/* Called when the mouse is moved. This
	 * is interpreted as a joystick event */
	virtual void onMouseMoved( const SDL_MouseMotionEvent& evt );

	/* Called when there is an event that is called */
	void fireControlEvent( const ControlMotionEvent& evt );

	/* Adds a mapping from a key to a MotionEventType. Keys are represented
	 * as a 1 for pressed and 0 for not pressed, so some normalization must
	 * be done with a joystick in order to received the desired results */
	inline void setKeyMapping( SDLKey key, ControlMotionEvent::MotionEventType to, float mult=1.0f ) {
		m_keysym_map[ key ] = M_MapVal( to, mult );;
	}

	/* Removes a mapping from this multiplexer, such that
	 * a key of this type pressed will no longer fire that
	 * event */
	inline void removeKeyMapping( SDLKey key ) {
		m_keysym_map.erase( key );
	}

	/* Sets a joystick axis mapping. If the joystick_index = 255 then it 
	 * is interpreted as the mouse */
	inline void setJoyAxisMapping( uint8_t joystick_index, uint8_t axis_index,
		ControlMotionEvent::MotionEventType to, float mult=1.0f ) {
		m_joy_axis_map[ joystick_index << 8 | axis_index ] = M_MapVal( to, mult );
	}

	inline void removeJoyAxisMapping( uint8_t joystick_index, uint8_t axis_index ) {
		m_joy_axis_map.erase( joystick_index << axis_index );
	}

private:
	struct M_MapVal {
		inline M_MapVal( ControlMotionEvent::MotionEventType typ, float mult ) :
			type( typ ), multiplier( mult ) {}
		inline M_MapVal( ) : type( ControlMotionEvent::NONE ), multiplier( 0.0f ) {}
		ControlMotionEvent::MotionEventType type;
		float multiplier;
	};
	/* The listeners on this multiplexer */
	std::vector< ControlMotionListener* > m_motion_listeners;

	/* The keysym map */
	std::map< SDLKey, M_MapVal > m_keysym_map;

	/* Mapping of a bit oring of joystick index and axis index to
	 * an event type */
	std::map< uint16_t, M_MapVal > m_joy_axis_map;
};

#endif /* CONTROLMULTIPLEXER_HPP_ */
