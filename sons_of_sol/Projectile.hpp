#ifndef PROJECTILE_HPP_
#define PROJECTILE_HPP_

/*
 * Author: jrahm
 * created: 2013/12/08
 * Projectile.hpp: <description>
 */

#include "glox/GloxPoint.hpp"
#include "glox/objects/GloxCube.hpp"
#include "glox/GloxState.hpp"
#include "glox/GloxCommon.hpp"

#include <iostream>

#define TTL 50

class Projectile {
public:
    Projectile( const glox::GloxPoint<>& position, const glox::GloxPoint<>& vector ) ;

    inline void update() {
        if( m_ttl == 0 || -- m_ttl == 0 ) {
            return ;
        }
        m_position += m_dpos;
    }

    inline bool isDead() { return m_ttl == 0; }

    void draw() ;

    static void loadModel() ;
private:
    uint8_t m_ttl ;
    float m_roty;
    float m_rotz;
    glox::GloxPoint<> m_position;
    glox::GloxPoint<> m_dpos;
    static glox::GloxCube* m_model ;

};

#endif /* PROJECTILE_HPP_ */
