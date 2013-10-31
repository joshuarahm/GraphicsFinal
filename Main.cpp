#include <cstdio>

/* Include the class SloxApplication which 
 * will be the base class for this */
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

using namespace slox;
using namespace glox;
using namespace std;

void exit( const SDL_QuitEvent& evt ) {
    (void) evt;
    exit( 0 );
}

class SonsOfSol : public SloxApplication, SloxRawEventHandler {
public:
    void reshape( int width, int height ) {
        m_viewport.setWidth( width );
        m_viewport.setHeight( height );

        m_viewport.render();
        m_perspective.setAspectRatio( m_viewport.getAspectRatio() );
        m_perspective.project();
    }

    void onEvent( const SDL_Event& event ) { SloxRawEventHandler::onEvent( event ); }

    bool initialize( int argc, char** argv ) {
        (void) argc;
        (void) argv;
        
        unsigned int tex = 0;

        SDL_Init( SDL_INIT_VIDEO );
        m_screen = SDL_SetVideoMode( 600, 600, 0, SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF );

        m_perspective.setPosition( GloxPoint<>( 0, 0, 10 ) );

        this->addQuitListener( new SloxFunctionQuitListener( exit ) );
            
        if( ! m_screen ) {
            /* If the screen could not be initialized, print
             * a useful video message */
            this->setError( "Cannot set SDL video mode\n" );
            return false;
        }

        SDL_WM_SetCaption( "Sons Of Sol", "" );
        reshape( m_screen->w, m_screen->h );
        
        if( SloxTextureFactory::readBitmapFile( "stars.bmp", &tex ) ) {
            /* Either there was a warning or an error, print out
             * either */
            fprintf( stderr, "%s\n", SloxTextureFactory::getMessage().c_str() );

            if( tex == 0 ) {
                /* If the texture hasn't changed, its an error, otherwise
                 * it's a warning, so continue */
                this->setError( "Aborting: unable to load texture" );
                return false;
            }
        }
        m_sky_tex.setId( tex );
        GloxTextureRepeat texrep( &m_sky_tex, 3, 3 );
        m_sky = new GloxTexturedCube( 900.0f, GloxColor( 255, 255, 255 ),
            texrep, texrep, texrep, texrep, texrep, texrep );

        m_quit = false;
        return true;
    }

    bool loop( uint32_t ticks ) {
        (void) ticks;
	    /* Set some parameters */
	    GloxState::clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	    GloxState::enable( GL_DEPTH_TEST );
	    GloxState::loadIdentity();
    
	    /* Translate to the this perspective */
	    m_perspective.render();
        m_sky->draw();

        GloxColor( 255,255,255 ).render();

        GloxWith( GL_QUADS, {
            GloxPoint<>( 0,0,0 ).plot();
            GloxPoint<>( 0,5,0 ).plot();
            GloxPoint<>( 5,5,0 ).plot();
            GloxPoint<>( 5,0,0 ).plot();
        } )
        
        glFlush();
        SDL_GL_SwapBuffers();
        /* Slow down a little */
        SDL_Delay( 5 );      
        return !m_quit;
    }


    ~SonsOfSol() {
    }
private:
    GloxViewport m_viewport;
    GloxLookAtPerspective m_perspective;
    GloxTexturedCube* m_sky;
    GloxTexture m_sky_tex;

    SDL_Surface* m_screen;

    bool m_quit;
};

int main ( int argc, char ** argv ) {
    SonsOfSol game;
    game.run( argc, argv );
}
