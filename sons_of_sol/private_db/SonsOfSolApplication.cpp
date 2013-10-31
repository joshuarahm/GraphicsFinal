#include "sons_of_sol/SonsOfSolApplication.hpp"

#include "glox/GloxLightSource.hpp"

using namespace slox;
using namespace glox;

bool SonsOfSolApplication::initialize( int argc, char** argv ) {
    SDL_Init( SDL_INIT_VIDEO );
    m_screen = SDL_SetVideoMode( 600, 600, 0, SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF );

    m_perspective.setPosition( GloxPoint<>( 0, 0, 10 ) );

    this->addQuitListener( this );
        
    if( ! m_screen ) {
        /* If the screen could not be initialized, print
         * a useful video message */
        this->setError( "Cannot set SDL video mode\n" );
        return false;
    }

    SDL_WM_SetCaption( "Sons Of Sol", "" );
    reshape( m_screen->w, m_screen->h );

	/* Load the textures into memory */
	if( ! loadTextures() )	return false;
    
    GloxTextureRepeat texrep( &m_sky_tex, 3, 3 );
    m_sky = new GloxTexturedCube( 900.0f, GloxColor( 255, 255, 255 ),
        texrep, texrep, texrep, texrep, texrep, texrep );

    m_quit = false;
    return true;
}

#define NTEX 3
bool SonsOfSolApplication::loadTextures() {
	const char* files[NTEX] = {
		"stars.bmp",
		"leaf.bmp",
		"crate.bmp"
	};

	GloxTexture* textures[NTEX] = {
		&m_sky_tex,
		&m_leaf_tex,
		&m_cube_tex
	};

	unsigned int tex;

	for ( int i = 0; i < NTEX; ++ i ) {
    	if( SloxTextureFactory::readBitmapFile( files[i], &tex ) ) {
        	/* Either there was a warning or an error, print out
         	* either */
        	fprintf( stderr, "Reading %s: %s\n", files[i], SloxTextureFactory::getMessage().c_str() );
	
        	if( tex == 0 ) {
            	/* If the texture hasn't changed, its an error, otherwise
             	* it's a warning, so continue */
            	this->setError( "Aborting: unable to load texture" );
            	return false;
        	}
    	}
    	textures[i]->setId( tex );
	}

	return true;
}

void SonsOfSolApplication::enableLighting() {
	int ambient_coef = 30;
	int diffuse_coef  = 100;
	int specular_coef =   0;

	GloxLightSource* source = m_light_manager.getLightSource( 0 );
	
	GloxColor base(2,2,2);
	source->setAmbient ( base * ambient_coef  );
	source->setDiffuse ( base * diffuse_coef  );
	source->setSpecular( base * specular_coef );
	source->setLightModelAttribute( GL_LIGHT_MODEL_LOCAL_VIEWER, 1 );
	GloxTrace( "init", "Source=%p\n", source );
	source->setEnabled( true );
}

void SonsOfSolApplication::update( uint32_t ticks ) {
	
}

void SonsOfSolApplication::display() {
    /* Translate to the this perspective */
    m_perspective.render();
    m_sky->draw();

    GloxColor( 255,255,255 ).render();

    GloxWith( GL_QUADS, {
        GloxPoint<>( 0,0,0 ).plot();
        GloxPoint<>( 0,5,0 ).plot();
        GloxPoint<>( 5,5,0 ).plot();
        GloxPoint<>( 5,0,0 ).plot();
    } );
}

void SonsOfSolApplication::reshape( int width, int height ) {
	m_viewport.setWidth( width );
	m_viewport.setHeight( height );

	m_viewport.render();
	m_perspective.setAspectRatio( m_viewport.getAspectRatio() );
	m_perspective.project();
}
