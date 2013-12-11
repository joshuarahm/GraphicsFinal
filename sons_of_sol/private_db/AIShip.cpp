#include "sons_of_sol/AIShip.hpp"

#include <cmath>
#include <iostream>

#include "glox/GloxScopedRotation.hpp"
#include "glox/GloxScopedTranslation.hpp"
#include "glox/GloxColor.hpp"

using namespace slox;
using namespace glox;
using namespace std;


SloxModelObject* AIShip::s_high_model ;
SloxModelObject* AIShip::s_med_model ;
SloxModelObject* AIShip::s_low_model ;
GloxCube* AIShip::s_really_far_away_model ;

void AIShip::setModel( slox::SloxModelObject* high, slox::SloxModelObject* med, slox::SloxModelObject* low ) {
    s_high_model = high;
    s_med_model = med;
    s_low_model = low;
    s_really_far_away_model = new glox::GloxCube( 4, 0.5, 8,  GloxColor(245,245,220) );
}
void AIShip::setUpdateFunction( Updater* updater ){
        update_func = update_func;

        /* Update 3 times so we have a full set
         * of points to start with */
        update();
        update();
        update();
}

void AIShip::calculate_roll_to() {
    GloxPoint<> total_change = m_future_position - m_last_position ;
    GloxPoint<> current_change = m_position - m_last_position ;

    float scalar_projection = ( current_change.dot( total_change ) / total_change.dot(total_change) ) ;
    total_change *= scalar_projection ;
    GloxPoint<> to_vector = current_change - total_change ;
    to_vector.normalize();

    float ang = to_vector.dot( GloxPointf(0,1,0) ) ;
    float tmp = GloxToDegrees( acos( ang ) );

    if( tmp == tmp ) {
        /* Cut out NANs */
        m_roll_to = tmp;
    }
//    cout << "m_roll_to " << m_roll_to << endl;
}

void AIShip::setTimeOffset( uint32_t off ) {
    m_time_offset = off ;
}

void AIShip::update( ) {
    m_last_position = m_position;
    m_position = m_future_position ;
    m_future_position = update_func->update( SDL_GetTicks() + m_time_offset ) ;
}

void AIShip::draw( float dist ) {
    GloxScopedTranslation __gstr1( m_position );
    /* The the change in the vector, this is where
     * the front of the ship goes */
    GloxPoint<> dpos = m_position - m_last_position  ;
    
    /* Project the point onto the x,y plane */
    GloxPoint<> first( dpos.getX(), dpos.getY(), 0 );
    /* A point that represents the x axis */
    GloxPoint<> xaxis( 1, 0, 0 );

    /* Rotation around the z axis */
    float dot = first.dot( xaxis ) ;
    float rot = GloxToDegrees(acos( dot )) ;

    if( first.getY() < 0 ) {
        rot = - rot;
    }
    rot = rot + 180 ;
    // if( rot != rot ) {
    //     cout << "Non existant Z rotation!" << endl ;
    // }
    GloxScopedRotation __gscr1( rot, 0, 0, 1 );
    /* x axis now follows *first* vector */
    /* Rotate around y axis to get x to follow *dpos* vector */
    dot = first.dot( dpos );

    rot = acos( dot );
    rot = GloxToDegrees( rot );

    if( dpos.getZ() < 0 ) {
        rot = - rot;
    }

    // if( rot != rot ) {
    //     cout << "Non existant Y rotation!" << endl ;
    // }
    GloxScopedRotation __glcr2( rot, 0, 1, 0 );

    calculate_roll_to();
    if( m_roll != m_roll ) {
        m_roll = 0.0f;
    }
    m_roll = ( m_roll_to + (5 - 1) * m_roll) / 5.0f;
    
    float roll = m_roll;
    // if( dpos.getX() < 0 ) {
    //     roll += 180 ;
    // }
    
    GloxScopedRotation __glco3( roll, 1, 0, 0 );
    GloxScale( 2, 
    if( dist < 20 )
        s_high_model->draw();
    else if ( dist < 50 )
        s_med_model->draw();
    else if ( dist < 1000 )
        s_low_model->draw();
    else
        s_really_far_away_model->draw();
    );
}
