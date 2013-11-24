#include "sons_of_sol/ControlMultiplexer.hpp"

ControlMultiplexer::ControlMultiplexer() {
	
}

void ControlMultiplexer::onKeyUp( const SDL_KeyboardEvent& evt ) {
	std::map< SDLKey, M_MapVal >::iterator itr;
	itr = m_keysym_map.find( evt.keysym.sym );

	if( itr != m_keysym_map.end() ) {
		ControlMotionEvent event( (*itr).second.type, 0, ControlMotionEvent::BUTTON );
		fireControlEvent( event );
	}
}

void ControlMultiplexer::onKeyDown( const SDL_KeyboardEvent& evt ) {
	std::map< SDLKey, M_MapVal >::iterator itr;
	itr = m_keysym_map.find( evt.keysym.sym );

	if( itr != m_keysym_map.end() ) {
		ControlMotionEvent event( (*itr).second.type, 1 * (*itr).second.multiplier, ControlMotionEvent::BUTTON );
		fireControlEvent( event );
	}
}

void ControlMultiplexer::onMouseMoved( const SDL_MouseMotionEvent& evt ) {
	std::map< uint16_t, M_MapVal >::iterator itr; 
	/* x direction on mousepad */
	itr = m_joy_axis_map.find( 0xFF00 );
	const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();   
	int screen_width = info->current_w;
	int screen_height = info->current_h;

	float x_norm = (evt.x - screen_width / 2) / ((float) screen_width / 2.0);
	if( itr != m_joy_axis_map.end() ) {
		ControlMotionEvent event( itr->second.type, x_norm * (*itr).second.multiplier,
			ControlMotionEvent::MOUSE );
		fireControlEvent( event );
	}

	/* y direction */
	float y_norm = (evt.y - screen_height / 2) / ((float) screen_height / 2.0);
	itr = m_joy_axis_map.find( 0xFF01 );
	if( itr != m_joy_axis_map.end() ) {
		ControlMotionEvent event( itr->second.type, y_norm * (*itr).second.multiplier,
			ControlMotionEvent::MOUSE );
		fireControlEvent( event );
	}
}

void ControlMultiplexer::fireControlEvent( const ControlMotionEvent& evt ) {
	std::vector< ControlMotionListener* >::iterator itr;

	for( itr = m_motion_listeners.begin(); itr != m_motion_listeners.end() ; ++ itr ) {
		(*itr)->onControlMotion( evt );
	}
}
