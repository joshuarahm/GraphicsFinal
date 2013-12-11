#ifndef SHIP_HPP_
#define SHIP_HPP_

/*
 * Author: jrahm
 * created: 2013/10/31
 * Ship.hpp: <description>
 */

#include "glox/GloxPoint.hpp"
#include "glox/GloxVector3.hpp"
#include "glox/GloxLookAtPerspective.hpp"

#include "slox/events/SloxKeyListener.hpp"

#include "ControlMotionListener.hpp"
#include "sons_of_sol/Projectile.hpp"

#include <vector>
#include <queue>

#define GUN_RATE 1
#define N_LIVING_PROJECTILES (TTL / GUN_RATE)

/* This is a class
 * for the first person
 * ship */
class PlayerShip : public ControlMotionListener {
public:
	inline PlayerShip() :
		m_forward( 0, 0, -1 ), m_up( 0, 1, 0 ), m_right( 1, 0, 0 ),
		m_dpitch( 0 ), 
		m_dyaw( 0 ), 
		m_droll( 0 ), 
		m_dforward( 0 ), 
		m_dup( 0 ),
		m_dright( 0 ),
        m_jitter( 0 ),
        m_is_firing( false ){} 

	/* Draw the heads-up-display of the
	 * ship */
	void drawHUD();

	/* Render the perspective of
	 * this ship */
	void renderPerspective();

	/* Move the ship up and down */
	void pitch( float angle );

	/* Roll the ship */
	void roll( float angle );

	/* Rotate the ship side to side */
	void yaw( float angle );

	/* Update this object */
	void update();

	const glox::GloxStandardProjection& getPerspective( ) const {
		return m_perspective;
	}

	glox::GloxStandardProjection& getPerspective( )  {
		return m_perspective;
	}

	void setPerspective( const glox::GloxLookAtPerspective& perspective ) {
		m_perspective = perspective;
	}

	inline void setDPitch( float dpitch ) {
		this->m_dpitch = dpitch;
	}

	inline void setDRoll( float droll ) {
		this->m_droll = droll;
	}

	inline void setDYaw( float dyaw ) {
		this->m_dyaw = dyaw;
	}

	inline float getDPitch() const { 
		return m_dpitch;
	}

	inline float getDYaw() const  { 
		return m_dyaw;
	}

	inline float getDRoll() const { 
		return m_droll;
	}

    inline void setDForwardTo( float dforward ) {
        m_dforward_to = dforward ;
    }

	inline float getDForward() {
		return m_dforward;
	}

	virtual void onControlMotion( const ControlMotionEvent& evt );

	inline const glox::GloxPoint<> getPosition() const {
		return m_position;
	}

	inline void setPosition( const glox::GloxPoint<>& pos ) {
		m_position = pos;
	}

	inline void setJitter( float jitter ) {
		m_jitter = jitter ;
	}

	inline float getJitter( ) {
		return m_jitter ;
	}

    void drawProjectiles() ;
private:
	/* The position of the ship */
	glox::GloxPoint<> m_position;

	/* The orientation of the ship */
	glox::GloxVector3<> m_forward;
	glox::GloxVector3<> m_up;
	glox::GloxVector3<> m_right;

	/* The perspective of the ship */
	glox::GloxLookAtPerspective m_perspective;

	float m_dpitch;
	float m_dyaw;

	float m_droll;
	float m_droll_to;

	float m_dforward;
	float m_dforward_to;

	float m_dup;
	float m_dup_to;

	float m_dright;
	float m_dright_to;

    float m_jitter ;

    bool m_is_firing ;
    std::vector<Projectile> m_living_projectiles ; 
    std::queue<int> m_open_positions;
};

#endif /* SHIP_HPP_ */
