#ifndef SONSOFSOLAPPLICATION_HPP_
#define SONSOFSOLAPPLICATION_HPP_

/*
 * Author: jrahm
 * created: 2013/10/31
 * SonsOfSolApplication.hpp: <description>
 */

#include "slox/SloxApplication.hpp"
#include "slox/SloxTextureFactory.hpp"
#include "slox/SloxRawEventHandler.hpp"
#include "slox/events/SloxFunctionQuitListener.hpp"

#include "glox/GloxViewport.hpp"
#include "glox/GloxCommon.hpp"
#include "glox/GloxFirstPersonPerspective.hpp"
#include "glox/objects/GloxTexturedCube.hpp"
#include "glox/GloxColor.hpp"
#include "glox/GloxLookAtPerspective.hpp"
#include "glox/GloxLightSourceManager.hpp"

class SonsOfSolApplication : public
	slox::SloxApplication,
	slox::SloxRawEventHandler,
	slox::SloxQuitListener {
public:
	/* Process an event */
	inline void onEvent( const SDL_Event& event )
		{ SloxRawEventHandler::onEvent( event ); }

	/* Initializes this application;
	 * returns true if initialization
	 * succeeded, false otherwise */
	bool initialize( int argc, char** argv );

	/* The main loop of the application */
	bool loop( uint32_t ticks ) {
		/* Update things that we might
		 * want to */
		update( ticks );

		/* Some boilder OpenGL stuff */
	    glox::GloxState::clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	    glox::GloxState::enable( GL_DEPTH_TEST );
	    glox::GloxState::loadIdentity();

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
	inline void onQuit( const SDL_QuitEvent& evt ) {
		(void) evt;
		m_quit = true;
	}

	inline virtual ~SonsOfSolApplication() {

	}

private:

	/* Resizes the perspective for OpenGL */
	void reshape( int width, int height );

	/* Sets up the lights to be used */
	void enableLighting( );

	void update( uint32_t ticks );

	void display( ) ;

	bool loadTextures();

	/* perspective stuff */
	glox::GloxViewport m_viewport;
	glox::GloxLookAtPerspective m_perspective;

	/* The sky */
	glox::GloxTexturedCube* m_sky;

	/* Textures */
	glox::GloxTexture m_sky_tex;
	glox::GloxTexture m_leaf_tex;
	glox::GloxTexture m_cube_tex;

	/* A boolean stating if it is time
	 * to quit */
	bool m_quit;

	glox::GloxLightSourceManager m_light_manager;

	SDL_Surface* m_screen;
};

#endif /* SONSOFSOLAPPLICATION_HPP_ */
