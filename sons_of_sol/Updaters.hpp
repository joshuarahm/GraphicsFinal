#ifndef UPDATERS_HPP_
#define UPDATERS_HPP_

/*
 * Author: jrahm
 * created: 2013/12/08
 * Updaters.hpp: <description>
 */

#include "sons_of_sol/AIShip.hpp"
#include "glox/GloxPoint.hpp"
#include "glox/GloxCommon.hpp"

class EllipseUpdater : public AIShip::Updater {
public:
    inline EllipseUpdater( const glox::GloxPoint<>& center,
        const glox::GloxPoint<>& major, const glox::GloxPoint<>& minor,
        float speed_factor ) :
    m_center( center ),
    m_major( major ),
    m_minor( minor ),
    m_speed( speed_factor ) { 
        m_a = major.getMagnitude();
        m_b = minor.getMagnitude();
    }

    inline glox::GloxPoint<> update( uint32_t time ) {
        float t = m_speed * time ;
        return m_center + ( m_major * m_a * glox::GloxCos( t ) +
            m_minor * m_b * glox::GloxSin( t ) );
    }
    
private:
    glox::GloxPoint<> m_center;
    glox::GloxPoint<> m_major;
    glox::GloxPoint<> m_minor;

    float m_a;
    float m_b;
    float m_speed ;
};

#endif /* UPDATERS_HPP_ */
