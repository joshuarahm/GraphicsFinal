#include "sons_of_sol/PlayerShip.hpp"

#include "glox/GloxCommon.hpp"
#include "glox/GloxColor.hpp"

using namespace glox;


void PlayerShip::pitch( float angle ) {
	/* Update the forward vector */
	m_forward *= (float)GloxCos( angle );
	m_forward += ( m_up * (float)GloxSin( angle ) );
	m_forward.normalize();

	m_up = m_right.cross( m_forward );
}

void PlayerShip::roll( float angle ) {
	m_right *= (float)GloxCos( angle );
	m_right += m_up * GloxSin( angle );
	m_right.normalize();

	m_up = m_right.cross( m_forward );
}

void PlayerShip::yaw( float angle ) {
	m_right *= (float)GloxCos( angle );
	m_right += m_forward * GloxSin( angle );
	m_right.normalize();

	m_forward = m_up.cross( m_right );
}

void PlayerShip::renderPerspective( ) {
	m_perspective.setLookAtPoint( m_position + m_forward.toPoint() );
	m_perspective.setPosition( m_position + (m_forward.toPoint() * 0.2f) );
	m_perspective.setUpVector( m_up.toPoint() );

	GloxColor( 255,255,255 ).render();
	GloxWith( GL_LINES,
		GloxPoint<>( 0,0,0 ).plot();
		m_position.plot() );
}

void PlayerShip::drawHUD() {

}

static inline float update_val( float to, float x, int inertia ) {
	return ( to + (inertia - 1) * x ) / inertia;
}

void PlayerShip::onControlMotion( const ControlMotionEvent& evt ) {
	//printf( "Event Mag: %f\n", evt.getMagnitude() );

	if( evt.getType() == ControlMotionEvent::PITCH ) {
		m_dpitch = update_val( evt.getMagnitude(), m_dpitch, 10 );
	}

	if( evt.getType() == ControlMotionEvent::THROTTLE ) {
		m_dforward_to = evt.getMagnitude();
	}

	if( evt.getType() == ControlMotionEvent::YAW ) {
		m_dyaw = update_val( evt.getMagnitude(), m_dyaw, 10 );
	}

	if( evt.getType() == ControlMotionEvent::ROLL ) {
		m_droll_to = evt.getMagnitude();
	}

	if( evt.getType() == ControlMotionEvent::STRAFE_SIDE ) {
		m_dright_to = evt.getMagnitude();
	}

	if( evt.getType() == ControlMotionEvent::STRAFE_UP ) {
		m_dup_to = evt.getMagnitude();
	}
}

void PlayerShip::update() {
	// printf( "dpitch: %f, droll: %f, dyaw: %f, dforward: %f, dright: %f, dup: %f\n",
	// 	m_dpitch, m_droll, m_dyaw, m_dforward, m_dright, m_dup );

	pitch( m_dpitch );
	roll( m_droll );
	yaw( m_dyaw );

	m_position += (m_forward * m_dforward).toPoint();
	m_position += (m_right * m_dright).toPoint();
	m_position += (m_up * m_dup).toPoint();

	m_droll = update_val( m_droll_to, m_droll, 20 );
	m_dup = update_val( m_dup_to, m_dup, 20 );
	m_dright = update_val( m_dright_to, m_dright, 20 );
	m_dforward = update_val( m_dforward_to, m_dforward, 200 );

	// This is only the case if we are using a mouse
	m_dpitch = update_val( 0, m_dpitch, 100 );
	m_dyaw = update_val( 0, m_dyaw, 100 );
	// m_droll = (m_droll_to * ( 1 - exp( - m_droll_count ) ) + m_droll * ( 1 - exp( - m_droll_count ) ) )/ 2.0;
	renderPerspective();
}
