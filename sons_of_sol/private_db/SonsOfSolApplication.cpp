#include "sons_of_sol/SonsOfSolApplication.hpp"

#include "glox/GloxLightSource.hpp"
#include "glox/GloxRotation.hpp"
#include "glox/GloxScopedRotation.hpp"

#include <iostream>

using namespace slox;
using namespace glox;
using namespace std;

void SonsOfSolApplication::setupControls() {
	m_event_multiplexer.setKeyMapping( SDLK_w, ControlMotionEvent::THROTTLE, 5.0f );
	m_event_multiplexer.setKeyMapping( SDLK_s, ControlMotionEvent::THROTTLE, -5.0f );
	m_event_multiplexer.setKeyMapping( SDLK_d, ControlMotionEvent::STRAFE_SIDE );
	m_event_multiplexer.setKeyMapping( SDLK_a, ControlMotionEvent::STRAFE_SIDE, -1.0f );
	m_event_multiplexer.setKeyMapping( SDLK_z, ControlMotionEvent::STRAFE_UP, -1.0f );
	m_event_multiplexer.setKeyMapping( SDLK_x, ControlMotionEvent::STRAFE_UP, 0.3f );

	m_event_multiplexer.setKeyMapping( SDLK_q, ControlMotionEvent::ROLL, 2.0f );
	m_event_multiplexer.setKeyMapping( SDLK_e, ControlMotionEvent::ROLL, -2.0f );
	m_event_multiplexer.setKeyMapping( SDLK_k, ControlMotionEvent::PITCH );
	m_event_multiplexer.setKeyMapping( SDLK_j, ControlMotionEvent::PITCH, -1.0f );
	m_event_multiplexer.setKeyMapping( SDLK_l, ControlMotionEvent::YAW, -1.0f );
	m_event_multiplexer.setKeyMapping( SDLK_h, ControlMotionEvent::YAW );

	m_event_multiplexer.setJoyAxisMapping( 255, 0, ControlMotionEvent::YAW, -1.0f );
	m_event_multiplexer.setJoyAxisMapping( 255, 1, ControlMotionEvent::PITCH, -1.0f );
}

void SonsOfSolApplication::setFullScreen() {
	m_screen = SDL_SetVideoMode(m_screen_width,m_screen_height,0,
		SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	reshape( m_screen_width, m_screen_height );
}

bool SonsOfSolApplication::initialize( int argc, char** argv ) {
	(void) argc;
	(void) argv;

	/* Use our texture factory to load textures */
	m_object_loader.setTextureFactory( &m_texture_factory );

	setupControls();

    SDL_Init( SDL_INIT_VIDEO );
	const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();   
	m_screen_width = info->current_w;
	m_screen_height = info->current_h;
    m_screen = SDL_SetVideoMode( 600, 600, 0, SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF );

    m_perspective.setPosition( GloxPoint<>( 0, 0, 10 ) );
	m_perspective.setZFar( 10000.0 );
	m_ship.setPerspective( m_perspective );


    if( argv[1] == NULL || strcmp(argv[1],"nograb") ) {
	    SDL_ShowCursor( 0 );
	    SDL_WM_GrabInput(SDL_GRAB_ON);
    }

	/* Add some listeners */
    this->addQuitListener( this );
	this->addKeyListener( this );

	this->addKeyListener( & m_event_multiplexer );
	this->addMouseMotionListener( & m_event_multiplexer );
	this->addResizeListener( this );

	m_event_multiplexer.addControlMotionListener( & m_ship );
        
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
    
    GloxTextureRepeat texrep( &m_sky_tex, 1.8, 1.8 );
    m_sky = new GloxTexturedCube( 9900.0f, GloxColor( 255, 255, 255 ),
        texrep, texrep, texrep, texrep, texrep, texrep );

    texrep = GloxTextureRepeat( &m_crate_tex, 1, 1 );
    m_crate = new GloxTexturedCube( 5.0f, GloxColor( 255, 255, 255 ),
        texrep, texrep, texrep, texrep, texrep, texrep );
	
	m_object_loader.addToPath( "objects" );
	m_object_loader.addToPath( "objects/material" );
	m_earth = new GloxTexturedSphere( 900.0f, 5.0f, m_earth_tex );
    m_probe = m_object_loader.loadObjectFromFile( "probe.obj" );

    if( ! m_probe ) {
        cerr << "Unable to load probe: " << m_object_loader.getReason() << endl;
        exit( 123 );
    }

    m_frigate_industrial = m_object_loader.loadObjectFromFile( "frigate_industrial.obj" );
    if( ! m_frigate_industrial ) {
        cerr << "Unable to load frigate_industrial\n" << m_object_loader.getReason() << endl;
        exit( 123 );
    }

	/* Add the points for the sun */
	m_star.add( GloxPointNormalTexture(
		GloxPoint<>( -100, 100, -4500 ), GloxNormal<>(0,0,0), GloxPoint<>( 0,0 ) ) );
	m_star.add( GloxPointNormalTexture(
		GloxPoint<>( 100, 100, -4500 ), GloxNormal<>(0,0,0), GloxPoint<>( 1,0 ) ) );
	m_star.add( GloxPointNormalTexture(
		GloxPoint<>( 100, 300, -4500 ), GloxNormal<>(0,0,0), GloxPoint<>( 1,1 ) ) );
	m_star.add( GloxPointNormalTexture(
		GloxPoint<>( -100, 300, -4500 ), GloxNormal<>(0,0,0), GloxPoint<>( 0,1 ) ) );

	/* Move the light souce to the sun */
	GloxLightSource* source = m_light_manager.getLightSource( 0 );
	source->setPosition( GloxPointf( 0, 200, -695 ) ); 

    m_quit = false;

	this->enableLighting();
    return true;
}

#define NTEX 5
bool SonsOfSolApplication::loadTextures() {
	const char* files[NTEX] = {
		"stars.bmp",
		"leaf.bmp",
		"crate.bmp",
		"star.bmp",
		"earth.bmp"
	};

	GloxTexture* textures[NTEX] = {
		&m_sky_tex,
		&m_leaf_tex,
		&m_crate_tex,
		&m_star_tex,
		&m_earth_tex
	};

	unsigned int tex = 0;

	m_texture_factory.addToPath( "objects/textures/" );
	m_texture_factory.addToPath( "objects/" );
	for ( int i = 0; i < NTEX; ++ i ) {
    	if( m_texture_factory.readBitmapFile( files[i], &tex ) ) {
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
	sun->setAmbient ( GloxColor( 0, 0, 0 )  );
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
}

void SonsOfSolApplication::display() {
    /* Translate to the this perspective */
	m_ship.drawHUD();
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
	);

	GloxWithTranslation( m_ship.getPosition() + GloxPoint<>( 700,-1000,0 ),
		GloxRotation( -90, 0, 0, 1 ).render();
		m_earth->draw()
	)


	m_light_manager.getLightSource( 1 )->setEnabled( true );
	m_light_manager.render();

	m_crate->draw();
    GloxWithTranslation( GloxPoint<>( 0,10,0 ),
        m_probe->draw() );

		GloxScale( 100, 
    		GloxWithTranslation( GloxPoint<>( 10,10,10 ),
				glPushMatrix();
				glRotatef( 90, 1.2, 1.3, 1.4 );
        		m_frigate_industrial->draw();
				glPopMatrix();
			)
		);

    GloxColor( 255,255,255 ).render();
}

void SonsOfSolApplication::reshape( int width, int height ) {
	m_viewport.setWidth( width );
	m_viewport.setHeight( height );

	m_viewport.render();
	m_ship.getPerspective().setAspectRatio( m_viewport.getAspectRatio() );
	m_ship.getPerspective().project();
}
