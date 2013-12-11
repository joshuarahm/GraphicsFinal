#include "sons_of_sol/SonsOfSolApplication.hpp"

#include "glox/GloxLightSource.hpp"
#include "glox/GloxRotation.hpp"
#include "glox/GloxScopedRotation.hpp"

#include <iostream>

using namespace slox;
using namespace glox;
using namespace std;

bool SonsOfSolApplication::initialize( int argc, char** argv ) {
	(void) argc;
	(void) argv;
	/* Use our texture factory to load textures */
	m_object_loader.setTextureFactory( &m_texture_factory );
    m_is_paused = false ;

	setupControls();

    SDL_Init( SDL_INIT_VIDEO );
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();   
	m_screen_width = info->current_w;
	m_screen_height = info->current_h;
    m_screen = SDL_SetVideoMode( 600, 600, 0, SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF );

    m_perspective.setPosition( GloxPoint<>( 0, 0, 10 ) );
	m_perspective.setZFar ( FAR_DISTANCE_END );
	m_perspective.setZNear( FAR_DISTANCE_START );
	m_ship.setPerspective( m_perspective );
	m_ship.setPosition( GloxPointf( 0, 10, -3 ) );


    if( argv[1] == NULL || strcmp(argv[1],"nograb") ) {
	    SDL_ShowCursor( 0 );
	    SDL_WM_GrabInput(SDL_GRAB_ON);
    }

	if( glewInit() != GLEW_OK ) {
		cerr << "Unable to initialize GLEW" << endl ;
		exit( 122 ) ;
	}

	/* Add some listeners */
    this->addQuitListener( this );
	this->addKeyListener( this );

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
    m_sky = new GloxTexturedCube( FAR_DISTANCE_END - 100, GloxColor( 255, 255, 255 ),
        texrep, texrep, texrep, texrep, texrep, texrep );

    texrep = GloxTextureRepeat( &m_crate_tex, 1, 1 );
    m_crate = new GloxTexturedCube( 5.0f, GloxColor( 255, 255, 255 ),
        texrep, texrep, texrep, texrep, texrep, texrep );
	
	m_object_loader.addToPath( "objects" );
	m_object_loader.addToPath( "objects/material" );
	m_earth = new GloxTexturedSphere( FAR_DISTANCE_END / 4.0, 5.0f, m_earth_tex );
	m_moon = new GloxTexturedSphere( FAR_DISTANCE_END / 24.0, 5.0f, m_moon_tex );
    m_probe = m_object_loader.loadObjectFromFile( "probe.obj" );

    m_script_ttl = 130 ;
    if( ! m_probe ) {
        cerr << "Unable to load probe: " << m_object_loader.getReason() << endl;
        exit( 123 );
    }

    m_frigate_industrial = m_object_loader.loadObjectFromFile( "frigate_industrial.obj" );
    if( ! m_frigate_industrial ) {
        cerr << "Unable to load frigate_industrial\n" << m_object_loader.getReason() << endl;
        exit( 123 );
    }

    m_other_ship = m_object_loader.loadObjectFromFile( "ship.obj" );
    SloxModelObject* ship_med = m_object_loader.loadObjectFromFile( "ship_med.obj" ) ;
    SloxModelObject* ship_low = m_object_loader.loadObjectFromFile( "ship_low.obj" ) ;
    if( ! (m_other_ship && ship_med && ship_low) ) {
        cerr << "Unable to load other ship\n" << m_object_loader.getReason() << endl;
        exit( 123 );
    }

    m_test_cube = m_object_loader.loadObjectFromFile( "cube.obj" );
    if( ! m_test_cube ) {
        cerr << "Unable to load test cube\n" << m_object_loader.getReason() << endl;
        exit( 123 );
    }

    m_deathstar = m_object_loader.loadObjectFromFile( "deathstar.obj" ) ;
    if( ! m_deathstar ) {
        cerr << "Unable to load deathstar\n" << m_object_loader.getReason() << endl;
        exit( 123 );
    }

    m_acceleration_cannon = m_object_loader.loadObjectFromFile( "acceleration_cannon.obj" ) ;
    if( ! m_acceleration_cannon ) {
        cerr << "Unable to load acceleration cannon\n" << m_object_loader.getReason() << endl;
        exit( 123 );
    }

    AIShip::setModel( m_other_ship, ship_med, ship_low ) ;

    float xstart = 0.5*(- FAR_DISTANCE_END / 5.0f);
    float xend   = 0.5*(- xstart);
    float ystart = 0.3*(FAR_DISTANCE_END / 3.0f - FAR_DISTANCE_END / 2);
    float yend   = 0.3*(ystart + 2*(FAR_DISTANCE_END / 5.0f));
	/* Add the points for the sun */
	m_star.add( GloxPointNormalTexture(
		GloxPoint<>( xstart, ystart, -(FAR_DISTANCE_END) / 3 ), GloxNormal<>(0,0,0), GloxPoint<>( 0,0 ) ) );
	m_star.add( GloxPointNormalTexture(
		GloxPoint<>( xend, ystart, -(FAR_DISTANCE_END) / 3 ), GloxNormal<>(0,0,0), GloxPoint<>( 1,0 ) ) );
	m_star.add( GloxPointNormalTexture(
		GloxPoint<>( xend, yend, -(FAR_DISTANCE_END) / 3 ), GloxNormal<>(0,0,0), GloxPoint<>( 1,1 ) ) );
	m_star.add( GloxPointNormalTexture(
		GloxPoint<>( xstart, yend, -(FAR_DISTANCE_END) / 3 ), GloxNormal<>(0,0,0), GloxPoint<>( 0,1 ) ) );

	/* Move the light souce to the sun */
	GloxLightSource* source = m_light_manager.getLightSource( 0 );
	source->setPosition( GloxPointf( 0, 200, -695 ) ); 

    m_quit = false;

    /* Starting ships */
    for( int i = 0; i < 5; ++ i ) {
        AIShip::Updater* updater = new EllipseUpdater( GloxPointf(0,-10,0), GloxPointf(0,10,10), GloxPointf(10,i*5,0), 0.01 );
        m_updaters.push_back( updater );
        AIShip tmp( updater );
        tmp.setTimeOffset( 1000 * i ) ;
        m_aiships.push_back( tmp ) ;
    }

    /* Frigate ships */
    for ( int j = 0; j < 360; j += 72 ) {
        for( int i = 0; i < 10; ++ i ) {
            float pos = 10000 + 100 * i;
            GloxPointf center( pos, pos, pos );
            float cos = GloxCos(j) ;
            float sin = GloxSin(j) ;
            AIShip::Updater* updater =
                new EllipseUpdater( center, GloxPointf(60*cos,-100*sin,2) , GloxPointf(50*cos,10*sin,50) * 2, 0.002 );
            m_updaters.push_back( updater );
            AIShip tmp( updater );
            tmp.setTimeOffset( 100000 * i ) ;
            m_aiships.push_back( tmp ) ;
        }
    }

    /* Deathstar ships */
    for ( int j = 0; j < 360; j += 72 ) {
        for( int i = 0; i < 10; ++ i ) {
            float pos = 100 * i;
            GloxPointf center( 5000 + pos, -1000+pos, -20000+pos );
            float cos = GloxCos(j) ;
            float sin = GloxSin(j) ;
            AIShip::Updater* updater =
                new EllipseUpdater( center, GloxPointf(60*cos,-70*sin,2) , GloxPointf(50*cos,10*sin,40) * 2, 0.002 );
            m_updaters.push_back( updater );
            AIShip tmp( updater );
            tmp.setTimeOffset( 10000 * i ) ;
            m_aiships.push_back( tmp ) ;
        }
    }

	m_earth_prog = new GloxProgram() ;
	if( m_earth_prog->attachShaderFromFile( "shaders/earth.vp", GL_VERTEX_SHADER ) ||
		m_earth_prog->attachShaderFromFile( "shaders/earth.fp", GL_FRAGMENT_SHADER ) ||
		m_earth_prog->link() ) {
		cerr << "Warning Unable to load shaders: " <<
			GloxShader::getMessage() << endl;
	}
	m_earth_shader_light_pos = m_earth_prog->getUniformLocation( "lightPos" );
	m_earth_shader_camera_pos = m_earth_prog->getUniformLocation( "cameraPos" );

	cout << "Earth light  " << m_earth_shader_light_pos  << endl;
	cout << "Earth camera " << m_earth_shader_camera_pos << endl;

	m_moon_prog = new GloxProgram() ;
	if( m_moon_prog->attachShaderFromFile( "shaders/moon.vp", GL_VERTEX_SHADER ) ||
		m_moon_prog->attachShaderFromFile( "shaders/moon.fp", GL_FRAGMENT_SHADER ) ||
		m_moon_prog->link() ) {
		cerr << "Warning Unable to load shaders: " <<
			GloxShader::getMessage() << endl;
	}
	m_moon_shader_light_pos = m_moon_prog->getUniformLocation( "lightPos" );
	m_moon_shader_camera_pos = m_moon_prog->getUniformLocation( "cameraPos" );


	// start the starting light
	GloxLightSource* start = m_light_manager.getLightSource( 0 );
	start->setLightModelAttribute( GL_LIGHT_MODEL_AMBIENT, GloxColor(50,50,50) );
	start->setAmbient ( GloxColor(30,30,30)  );
	start->setDiffuse ( GloxColor(255,0,0)  );
	start->setPosition( GloxPoint<>( 0,0,0 ) );

	start->setEnabled( false );

	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5);

    return true;
}
