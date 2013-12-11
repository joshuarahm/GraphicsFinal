#include "sons_of_sol/Projectile.hpp"
#include "glox/GloxColor.hpp"
#include "glox/GloxScopedRotation.hpp"

using namespace glox ;
using namespace std ;

GloxCube* Projectile::m_model;

void Projectile::loadModel( ) {
    if( ! m_model ) {
        m_model = new GloxCube( 10, 0.1, 0.1, GloxColor( 255, 255, 0 ) );
    }
}

inline float jitter() {
	return (2 * rand() & 0xFF) / 256.0 ;
}

void Projectile::draw() {
    GloxPoint<> next = m_position + (m_dpos) * 0.2 + GloxPointf(jitter(),jitter(),jitter());
    GloxScopedAttributes __glsa( GL_CURRENT_BIT ); 

    GloxColor(
        (uint8_t)(255.0f /     TTL    *     m_ttl   ),
        (uint8_t)(250.0f / pow(TTL,2) * pow(m_ttl,2)),
        (uint8_t)(200.0f / pow(TTL,4) * pow(m_ttl,4)),
		(uint8_t)(100.0)
		).render();

    glLineWidth( m_ttl / 10.0f );
    GloxWith( GL_LINES,
        m_position.plot();
    GloxColor(
        (uint8_t)(255.0f /     TTL    *    (m_ttl-1)  ),
        (uint8_t)(200.0f / pow(TTL,2) * pow(m_ttl-1,2)),
        (uint8_t)(100.0f / pow(TTL,4) * pow(m_ttl-1,4))).render();
        next.plot();
    );
}

Projectile::Projectile( const glox::GloxPoint<>& position, const glox::GloxPoint<>& vec ) {
    m_position = position;
    m_dpos = vec ;
    m_ttl = TTL;

    loadModel();

    /* Project the point onto the x,y plane */
    GloxPoint<> first( m_dpos.getX(), m_dpos.getY(), 0 );
    /* A point that represents the x axis */
    GloxPoint<> xaxis( 1, 0, 0 );

    /* Rotation around the z axis */
    float dot = first.dot( xaxis ) ;
    float rot = GloxToDegrees(acos( dot )) ;

    if( first.getY() < 0 ) {
        rot = - rot;
    }
    rot = rot + 180 ;
    m_roty = rot;
    /* x axis now follows *first* vector */
    /* Rotate around y axis to get x to follow *dpos* vector */
    dot = first.dot( m_dpos );
    rot = acos( dot );
    rot = GloxToDegrees( rot );

    if( m_dpos.getZ() < 0 ) {
        rot = - rot;
    }
    m_rotz = rot;
}
