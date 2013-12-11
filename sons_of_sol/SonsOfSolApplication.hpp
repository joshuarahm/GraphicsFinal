#ifndef SONSOFSOLAPPLICATION_HPP_
#define SONSOFSOLAPPLICATION_HPP_

/*
 * Author: jrahm
 * created: 2013/10/31
 * SonsOfSolApplication.hpp: <description>
 */
#include "glox/GloxCommon.hpp"

#include "slox/SloxApplication.hpp"
#include "slox/SloxTextureFactory.hpp"
#include "slox/SloxRawEventHandler.hpp"
#include "slox/events/SloxFunctionQuitListener.hpp"
#include "slox/loader/SloxModelObject.hpp"
#include "slox/loader/SloxObjectLoader.hpp"

#include "glox/GloxViewport.hpp"
#include "glox/GloxFirstPersonPerspective.hpp"
#include "glox/objects/GloxTexturedCube.hpp"
#include "glox/objects/GloxTexturedSphere.hpp"
#include "glox/GloxColor.hpp"
#include "glox/GloxLookAtPerspective.hpp"
#include "glox/GloxLightSourceManager.hpp"
#include "glox/objects/GloxCube.hpp"
#include "glox/GloxProgram.hpp"

#include "sons_of_sol/PlayerShip.hpp"
#include "sons_of_sol/ControlMultiplexer.hpp"
#include "sons_of_sol/AIShip.hpp"
#include "sons_of_sol/Updaters.hpp"

#define FAR_DISTANCE_START 1
#define FAR_DISTANCE_END   1000000

#define MID_DISTANCE_START 10
#define MID_DISTANCE_END   100000

class SonsOfSolApplication : public
	slox::SloxApplication,
	slox::SloxRawEventHandler,
	slox::SloxResizeListener,
	slox::SloxKeyListener,
	slox::SloxQuitListener {
public:
	/* Process an event */
	virtual inline void onEvent( const SDL_Event& event ) { 
        SloxRawEventHandler::onEvent( event );
    }

	virtual inline void onKeyDown( const SDL_KeyboardEvent& evt ) {
		if( evt.keysym.sym == SDLK_ESCAPE ) exit( 0 );
		if( evt.keysym.sym == SDLK_f ) {
			setFullScreen();
		};
		if( evt.keysym.sym == SDLK_p ) {
			pause();
		};
	}

	/* Initializes this application;
	 * returns true if initialization
	 * succeeded, false otherwise */
	virtual bool initialize( int argc, char** argv );

	/* The main loop of the application */
	virtual bool loop( uint32_t ticks ) {
		/* Update things that we might
		 * want to */
		update( ticks );

		/* Some boilder OpenGL stuff */
	    glox::GloxState::clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	    glox::GloxState::loadIdentity();
    	glox::GloxState::enable( GL_DEPTH_TEST );

		/* Display some stuff */
		display();

		/* Flush the buffers and
		 * do the appropriate delay */
        glFlush();
        SDL_GL_SwapBuffers();
        SDL_Delay( 5 );      

        return !m_quit;
	}

	/* What happens when a quit event is
	 * intercepted */
	virtual inline void onQuit( const SDL_QuitEvent& evt ) {
		(void) evt;
		m_quit = true;
	}

	inline virtual ~SonsOfSolApplication() {

	}

	/* Resizes the perspective for OpenGL */
	inline void onResize( const SDL_ResizeEvent& evt ) {
		m_screen = SDL_SetVideoMode(evt.w,evt.h,0,SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF);
		reshape( evt.w, evt.h );
	}

    void pause() ;
private:
    /* Render the background. (This is the Earth and skybox) */
    void renderFar();
    /* Render whats in the middle */
    void renderMid();

	void setupControls() ;
	void reshape( int width, int height );

	/* Sets up the lights to be used */
	void enableLighting( );
	void update( uint32_t ticks );
	void display( ) ;
	bool loadTextures();
	void setFullScreen();

	/* perspective stuff */
	glox::GloxViewport m_viewport;
	glox::GloxLookAtPerspective m_perspective;

	/* The sky */
	glox::GloxTexturedCube* m_sky;
	glox::GloxTexturedCube* m_crate;
	glox::GloxTexturedSphere* m_earth;
    glox::GloxTexturedSphere* m_moon;

	/* Textures */
	glox::GloxTexture m_sky_tex;
	glox::GloxTexture m_leaf_tex;
	glox::GloxTexture m_crate_tex;
	glox::GloxTexture m_star_tex;
	glox::GloxTexture m_earth_tex;
	glox::GloxTexture m_moon_tex;

	/* The sun */
	glox::GloxPointCollection<glox::GloxPointNormalTexture> m_star;

    std::vector<AIShip> m_aiships ;
    std::vector<AIShip::Updater*> m_updaters;

    /* The little test object */
    slox::SloxModelObject* m_probe;
    slox::SloxModelObject* m_frigate_industrial;
    slox::SloxModelObject* m_other_ship;
    slox::SloxModelObject* m_test_cube;
    slox::SloxModelObject* m_deathstar;
    slox::SloxModelObject* m_acceleration_cannon ;

	slox::SloxTextureFactory m_texture_factory;
	slox::SloxObjectLoader   m_object_loader;
	/* A boolean stating if it is time
	 * to quit */
	bool m_quit;

	glox::GloxLightSourceManager m_light_manager;
	PlayerShip m_ship;

	SDL_Surface* m_screen;
	ControlMultiplexer m_event_multiplexer;

	int m_screen_width;
	int m_screen_height;

    int m_script_ttl ;

    bool m_is_paused;

	glox::GloxProgram* m_earth_prog ;
	glox::GloxProgram* m_moon_prog ;
	int m_earth_shader_light_pos ;
	int m_earth_shader_camera_pos ;
	int m_moon_shader_light_pos ;
	int m_moon_shader_camera_pos ;
};

#endif /* SONSOFSOLAPPLICATION_HPP_ */
