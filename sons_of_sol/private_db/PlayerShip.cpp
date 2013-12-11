#include "sons_of_sol/PlayerShip.hpp"

#include "glox/GloxCommon.hpp"
#include "glox/GloxColor.hpp"

using namespace glox;
using namespace std;


float randomJitter( float jitter ) {
    return (( rand() % 2000 - 1000 ) / 1000.0) * jitter ;
}

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

    GloxPoint<> noise( randomJitter( m_jitter ), randomJitter( m_jitter ), randomJitter( m_jitter ) ) ;
    if( m_is_firing ) {
        noise += GloxPointf( randomJitter(0.01),randomJitter(0.01),randomJitter(0.01) );
    }
	m_perspective.setLookAtPoint( m_position + m_forward.toPoint() + noise );
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

	else if( evt.getType() == ControlMotionEvent::THROTTLE ) {
		m_dforward_to = evt.getMagnitude();
	}

	else if( evt.getType() == ControlMotionEvent::YAW ) {
		m_dyaw = update_val( evt.getMagnitude(), m_dyaw, 10 );
	}

	else if( evt.getType() == ControlMotionEvent::ROLL ) {
		m_droll_to = evt.getMagnitude();
	}

	else if( evt.getType() == ControlMotionEvent::STRAFE_SIDE ) {
		m_dright_to = evt.getMagnitude();
	}

	else if( evt.getType() == ControlMotionEvent::STRAFE_UP ) {
		m_dup_to = evt.getMagnitude();
	}

    else if( evt.getType() == ControlMotionEvent::FIRE_PRIMARY ) {
        m_is_firing = evt.getMagnitude() != 0 ;
    }
}

void PlayerShip::update() {
    static const float projectile_speed = 150;
	// printf( "dpitch: %f, droll: %f, dyaw: %f, dforward: %f, dright: %f, dup: %f\n",
	// 	m_dpitch, m_droll, m_dyaw, m_dforward, m_dright, m_dup );

	pitch( m_dpitch );
	roll( m_droll );
	yaw( m_dyaw );

    int i = 0;
    for( vector<Projectile>::iterator itr = m_living_projectiles.begin();
        itr != m_living_projectiles.end() ; ++ itr ) {
        if( ! (*itr).isDead() ) {
            (*itr).update();
            if( (*itr).isDead() ) {
                m_open_positions.push( i );
            }
        }
        ++ i;
    }

	m_position += (m_forward * m_dforward).toPoint();
	m_position += (m_right * m_dright).toPoint();
	m_position += (m_up * m_dup).toPoint();

	m_droll = update_val( m_droll_to, m_droll, 20 );
	m_dup = update_val( m_dup_to, m_dup, 20 );
	m_dright = update_val( m_dright_to, m_dright, 20 );

    float last_dforward = m_dforward ;
	m_dforward = update_val( m_dforward_to, m_dforward, 200 );

    float accel = m_dforward - last_dforward ;
    if( accel > 0 ) {
        m_jitter = accel / 40.0 ;
    } else {
        m_jitter = 0.0 ;
    }

	// This is only the case if we are using a mouse
	m_dpitch = update_val( 0, m_dpitch, 100 );
	m_dyaw = update_val( 0, m_dyaw, 100 );
	// m_droll = (m_droll_to * ( 1 - exp( - m_droll_count ) ) + m_droll * ( 1 - exp( - m_droll_count ) ) )/ 2.0;
    if( m_is_firing ) {
        GloxPointf spread( randomJitter(0.03), randomJitter(0.03), randomJitter(0.03) );
        Projectile proj( (m_position - m_up.toPoint() - m_right.toPoint()), (m_forward.toPoint() + spread) * projectile_speed ) ;

        if( m_open_positions.empty() ) {
            m_living_projectiles.push_back( proj );
        } else {
            int idx = m_open_positions.front();
            m_open_positions.pop();
            m_living_projectiles[idx] = proj;
        }
    }

	renderPerspective();
}

void PlayerShip::drawProjectiles() {
    for( vector<Projectile>::iterator itr = m_living_projectiles.begin();
        itr != m_living_projectiles.end() ; ++ itr ) {
        if( ! (*itr).isDead() ) {
            itr->draw();
        }
    }
}
