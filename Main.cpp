#include <cstdio>

/* Include the class SloxApplication which 
 * will be the base class for this */
#include "glox/GloxCommon.hpp"
#include "slox/SloxApplication.hpp"
#include "slox/SloxTextureFactory.hpp"
#include "slox/SloxRawEventHandler.hpp"
#include "slox/events/SloxFunctionQuitListener.hpp"

#include "glox/GloxViewport.hpp"
#include "glox/GloxFirstPersonPerspective.hpp"
#include "glox/objects/GloxTexturedCube.hpp"
#include "glox/GloxColor.hpp"
#include "glox/GloxLookAtPerspective.hpp"

#include "sons_of_sol/SonsOfSolApplication.hpp"

using namespace slox;
using namespace glox;
using namespace std;

void exit( const SDL_QuitEvent& evt ) {
    (void) evt;
    exit( 0 );
}

int main ( int argc, char ** argv ) {
    SonsOfSolApplication game;
    game.run( argc, argv );
    return 0;
}
