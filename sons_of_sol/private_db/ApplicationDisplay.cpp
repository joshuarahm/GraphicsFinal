#include "sons_of_sol/SonsOfSolApplication.hpp"

#include "glox/GloxLightSource.hpp"
#include "glox/GloxRotation.hpp"
#include "glox/GloxScopedRotation.hpp"

#include <iostream>

using namespace glox;
using namespace slox;
using namespace std;

float randJitter() {
	return ((rand() & 0xFF) - 128.0) / (256.0 * 10) ;
}

void SonsOfSolApplication::display() {
    if ( m_script_ttl >= 0 ) {
		GloxLightSource* golight = m_light_manager.getLightSource( 0 );
		golight->setPosition( m_ship.getPosition() );

		if( m_script_ttl > 90 ) {
			m_ship.setPosition( m_ship.getPosition() - GloxPointf( randJitter(),0.25,randJitter() ) );
		} else if( m_script_ttl > 80 ) {
			float coef = 11 - (m_script_ttl - 80);
			m_ship.setPosition( GloxPointf( randJitter()/coef,randJitter()/coef,randJitter()/coef-3 ) );
		} else if( m_script_ttl == 80 ) {
			m_ship.setPosition( GloxPointf( 0,0,-3 ) );
		}

		golight->setEnabled( true );
        if ( m_script_ttl == 70 ) {
            m_ship.setDForwardTo( 30 );
			golight->setLightModelAttribute( GL_LIGHT_MODEL_AMBIENT, GloxColor(50,50,50) );
			golight->setAmbient ( GloxColor(50,50,50)  );
			golight->setDiffuse ( GloxColor(0,255,0)  );
        }
        if ( m_script_ttl == 0 ) {
	        this->addKeyListener( & m_event_multiplexer );
	        this->addMouseMotionListener( & m_event_multiplexer );
            m_ship.setDForwardTo( 20 ) ;
			enableLighting();
        }
        cout << m_script_ttl << endl ;
        m_script_ttl -= 1 ;
    }

    /* Translate to the this perspective */
	m_ship.drawHUD();

    m_ship.getPerspective().setZNear( FAR_DISTANCE_START );   
    m_ship.getPerspective().setZFar( FAR_DISTANCE_END );   
	m_ship.getPerspective().render();

	/* We don't want lighting for the skybox */
	GloxDisableFor( GL_LIGHTING, 
		/* Draw some stuff relative to the
		 * ship so the ship cannot break out
		 * of the skybox */
    	GloxWithTranslation( m_ship.getPosition(),
			/* Draw the background sky */
			m_sky->draw();
			/* Draw the star texture */
        	GloxEnableFor( GL_TEXTURE_2D,
				m_star_tex.bind();
				GloxWith( GL_QUADS, m_star.plot() );
			);
		)



		/* Draw the earth with its
		 * shaders */
		m_earth_prog->render() ;
		m_earth_prog->setUniformVector3( m_earth_shader_light_pos,
			m_light_manager.getLightSource(0)->getPosition() ) ;
        m_earth_prog->setUniformFloat( m_earth_shader_time, SDL_GetTicks() / 300000.0 ) ;

		//GloxPoint<> pos( GloxCos( ticks ) * FAR_DISTANCE_END/4.0, 0, GloxSin( ticks ) * FAR_DISTANCE_END/4.0 );
		GloxPoint<> pos( -250000, 0, 0 );
		m_earth_prog->setUniformVector3( m_earth_shader_camera_pos,
			pos );
		// cout << "Pos          : " << pos.toString() << " " << (pos.getZ() / pos.getX()) << endl;
		// cout << "Ship Position: " << m_ship.getPosition().toString() << (m_ship.getPosition().getZ() / m_ship.getPosition().getX()) << endl ;
		GloxWithTranslation( m_ship.getPosition() + GloxPoint<>( 700,-FAR_DISTANCE_END / 2,0 ),
			GloxRotation( -90, 0, 0, 1 ).render();
			GloxEnableFor( GL_BLEND, 
				m_earth->draw();
			)
		)
	
		/* Draw the moon with its shaders */
		m_moon_prog->render() ;
		m_moon_prog->setUniformVector3( m_moon_shader_light_pos,
			m_light_manager.getLightSource( 0 )->getPosition() ) ;
	
		m_moon_prog->setUniformVector3( m_moon_shader_camera_pos,
			m_ship.getPosition() ) ;
	
		GloxWithTranslation( m_ship.getPosition() + GloxPoint<>( 700,FAR_DISTANCE_END / 3.0f, 0 ),
			GloxRotation( -90, 0, 0, 1 ).render();
			m_moon->draw()
		)
	
		GloxProgram::unloadPrograms() ;
	);



	m_light_manager.getLightSource( 1 )->setEnabled( true );
	m_light_manager.render();

    GloxScale( 6,
        m_acceleration_cannon->draw() ) ;

    for( vector<AIShip>::iterator itr = m_aiships.begin() ; itr != m_aiships.end() ; ++ itr ) {
        // cout << itr->getPosition().toString() << endl ;
        itr->draw( (itr->getPosition() - m_ship.getPosition()).getMagnitude() );
    }

    glPushMatrix();

	// m_crate->draw();
    GloxWithTranslation( GloxPoint<>( 0,10,0 ),
        m_probe->draw() );
    GloxWithTranslation( GloxPoint<>( 0,-10,0 ),
        m_test_cube->draw() );

	GloxWithTranslation( GloxPoint<>( 10000,10000,10000 ),
	GloxScale( 1000, 
			glPushMatrix();
			glRotatef( 90, 1.2, 1.3, 1.4 );
    		m_frigate_industrial->draw();
			glPopMatrix();
		);
	);
    
	GloxWithTranslation( GloxPoint<>( 5000,-1000,-20000 ),
	    GloxScale( 100, 
			glPushMatrix();
			glRotatef( 90, 0.3, 1, 0 );
    		m_deathstar->draw();
			glPopMatrix();
		);
	);

	GloxDisableFor( GL_LIGHTING, 
		GloxEnableFor(GL_BLEND,
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    		m_ship.drawProjectiles();
		)
	)


    glPopMatrix();
}
