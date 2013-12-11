#ifndef AISHIP_HPP_
#define AISHIP_HPP_

/*
 * Author: jrahm
 * created: 2013/12/07
 * AIShip.hpp: <description>
 */

#include "slox/loader/SloxModelObject.hpp"
#include "glox/GloxCommon.hpp"
#include "glox/GloxState.hpp"
#include "glox/objects/GloxCube.hpp"

#include "slox/SloxCommon.hpp"
#include "glox/GloxPoint.hpp"

class AIShip {
public:
    class Updater {
    public: 
        virtual glox::GloxPoint<> update( uint32_t ) = 0;
    };
    inline AIShip( Updater* update = NULL ) :
        update_func( update ), m_roll( 0 ), m_roll_to( 0 ) {}

    void update() ;

    void draw( float dist ) ;

    void setTimeOffset( uint32_t off ) ;

    inline void setUpdateFunction( Updater* updater  ) ;

    static void setModel( slox::SloxModelObject* high, slox::SloxModelObject* med, slox::SloxModelObject* low ) ;

    inline const glox::GloxPoint<>& getPosition() { return m_position ; }

private:
    Updater* update_func;

    void calculate_roll_to();

    float m_ptich ;
    float m_yaw ;
    float m_roll ;

    float m_roll_to ;
    float m_weight ;
    uint32_t m_time_offset ;

    glox::GloxPoint<> m_position ;
    glox::GloxPoint<> m_last_position ;
    glox::GloxPoint<> m_future_position ;

    static slox::SloxModelObject* s_high_model ;
    static slox::SloxModelObject* s_med_model ;
    static slox::SloxModelObject* s_low_model ;

    static glox::GloxCube* s_really_far_away_model;
} ;



#endif /* AISHIP_HPP_ */
