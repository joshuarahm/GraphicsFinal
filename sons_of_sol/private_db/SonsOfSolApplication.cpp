#include "sons_of_sol/SonsOfSolApplication.hpp"

#include "glox/GloxLightSource.hpp"
#include "glox/GloxRotation.hpp"
#include "glox/GloxScopedRotation.hpp"

#include <iostream>

using namespace slox;
using namespace glox;
using namespace std;

void SonsOfSolApplication::setupControls() {
	m_event_multiplexer.setKeyMapping( SDLK_w, ControlMotionEvent::THROTTLE, 30.0f );
	m_event_multiplexer.setKeyMapping( SDLK_s, ControlMotionEvent::THROTTLE, -20.0f );
	m_event_multiplexer.setKeyMapping( SDLK_d, ControlMotionEvent::STRAFE_SIDE );
	m_event_multiplexer.setKeyMapping( SDLK_a, ControlMotionEvent::STRAFE_SIDE, -1.0f );
	m_event_multiplexer.setKeyMapping( SDLK_z, ControlMotionEvent::STRAFE_UP, -1.0f );
	m_event_multiplexer.setKeyMapping( SDLK_x, ControlMotionEvent::STRAFE_UP, 0.3f );

	m_event_multiplexer.setKeyMapping( SDLK_q, ControlMotionEvent::ROLL, 5.0f );
	m_event_multiplexer.setKeyMapping( SDLK_e, ControlMotionEvent::ROLL, -5.0f );
	m_event_multiplexer.setKeyMapping( SDLK_k, ControlMotionEvent::PITCH );
	m_event_multiplexer.setKeyMapping( SDLK_j, ControlMotionEvent::PITCH, -1.0f );
	m_event_multiplexer.setKeyMapping( SDLK_l, ControlMotionEvent::YAW, -1.0f );
	m_event_multiplexer.setKeyMapping( SDLK_h, ControlMotionEvent::YAW );

	m_event_multiplexer.setKeyMapping( SDLK_SPACE, ControlMotionEvent::FIRE_PRIMARY );
	m_event_multiplexer.setKeyMapping( SDLK_LSHIFT, ControlMotionEvent::FIRE_PRIMARY );

	m_event_multiplexer.setJoyAxisMapping( 255, 0, ControlMotionEvent::YAW, -3.0f );
	m_event_multiplexer.setJoyAxisMapping( 255, 1, ControlMotionEvent::PITCH, -3.0f );
}

void SonsOfSolApplication::setFullScreen() {
	m_screen = SDL_SetVideoMode(m_screen_width,m_screen_height,0,
		SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	reshape( m_screen_width, m_screen_height );
}

void SonsOfSolApplication::pause() {
    if( m_is_paused ) {
        SDL_ShowCursor( 0 ) ;
	    SDL_WM_GrabInput(SDL_GRAB_ON);
    } else {
        SDL_ShowCursor( 1 ) ;
	    SDL_WM_GrabInput(SDL_GRAB_OFF);
    }
    m_is_paused = ! m_is_paused ;
}


#define NTEX 6
bool SonsOfSolApplication::loadTextures() {
	const char* files[NTEX] = {
		"stars.jpg",
		"leaf.jpg",
		"crate.jpg",
		"star.jpg",
		"earth.jpg",
        "moon.jpg"
	};

	GloxTexture* textures[NTEX] = {
		&m_sky_tex,
		&m_leaf_tex,
		&m_crate_tex,
		&m_star_tex,
		&m_earth_tex,
        &m_moon_tex
	};

	unsigned int tex = 0;

	m_texture_factory.addToPath( "objects/textures/" );
	m_texture_factory.addToPath( "objects/" );
	for ( int i = 0; i < NTEX; ++ i ) {
    	if( m_texture_factory.readImageFile( files[i], &tex ) ) {
        	/* Either there was a warning or an error, print out
         	* either */
        	fprintf( stderr, "Reading %s: %s\n", files[i], m_texture_factory.getMessage().c_str() );
	
        	if( tex == 0 ) {
            	/* If the texture hasn't changed, its an error, otherwise
             	* it's a warning, so continue */
            	this->setError( "Aborting: unable to load texture" );
            	return false;
        	}
    	}
    	textures[i]->setId( tex );
		tex = 0;
	}

	return true;
}

void SonsOfSolApplication::enableLighting() {
	/* There is very little ambient light
	 * in space */
	// int ambient_coef = 0.5;
	// int diffuse_coef  = 100;
	int specular_coef =   0;

	GloxLightSource* sun = m_light_manager.getLightSource( 0 );
	GloxLightSource* earth = m_light_manager.getLightSource( 1 );
	
	GloxColor base(2,2,0);
	sun->setLightModelAttribute( GL_LIGHT_MODEL_AMBIENT, GloxColor(100,100,90) );
	sun->setAmbient ( GloxColor( 25, 25, 25 )  );
	sun->setDiffuse ( GloxColor( 255, 235, 200 ) );
	sun->setSpecular( base * specular_coef );
	sun->setLightModelAttribute( GL_LIGHT_MODEL_LOCAL_VIEWER, 1 );
	sun->setEnabled( true );

	earth->setLightModelAttribute( GL_LIGHT_MODEL_AMBIENT, GloxColor(0,0,0) );
	earth->setAmbient ( GloxColor(0,0,0)  );
	earth->setDiffuse ( GloxColor(130,155,155)  );
	earth->setSpecular( base * specular_coef );
	earth->setLightModelAttribute( GL_LIGHT_MODEL_LOCAL_VIEWER, 1 );
	earth->setPosition( GloxPoint<>( 700,-1000,0 ) );
}

void SonsOfSolApplication::update( uint32_t ticks ) {
	(void) ticks;
	m_ship.update();

    for( vector<AIShip>::iterator itr = m_aiships.begin() ;
        itr != m_aiships.end() ; ++ itr ) {
        itr->update();
    }
}

void SonsOfSolApplication::renderFar() {
    // m_ship.getPerspective().setZNear( FAR_DISTANCE_START );   
    // m_ship.getPerspective().setZNear( FAR_DISTANCE_START );   
}


void SonsOfSolApplication::reshape( int width, int height ) {
	m_viewport.setWidth( width );
	m_viewport.setHeight( height );

	m_viewport.render();
	m_ship.getPerspective().setAspectRatio( m_viewport.getAspectRatio() );
	m_ship.getPerspective().project();
}
