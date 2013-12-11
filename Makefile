CPPC?=g++
AR?=ar
OPTFLAGS?=-g3 -ggdb
CFLAGS= -Wall -Wextra -I. $(OPTFLAGS) -D DEBUG_LEVEL_TRACE -Islox -Iglox
LDFLAGS= -Lslox -Lglox -lslox -lglox -lGL -lGLU -lSDL -lm -ljpeg -lSDL_image -lGLEW
OBJECTS=obs/Main.o obs/AIShip.o obs/ControlMultiplexer.o obs/PlayerShip.o obs/ApplicationInit.o obs/SonsOfSolApplication.o obs/Projectile.o obs/ApplicationDisplay.o
BINARY=SonsOfSol
all: submodules setup $(OBJECTS)
	$(CPPC) -o $(BINARY) $(OBJECTS) $(LDFLAGS)

genmake:
	find . -name genmake.sh -exec {} \;

setup:
	mkdir -p obs/

submodules:
	for i in $$(find */ -name Makefile) ; do \
		cd $$(dirname $$i) && make && cd ..; \
	done

clean:
	- rm -rf obs $(BINARY)
	for i in $$(find */ -name Makefile) ; do \
		cd $$(dirname $$i) && make clean && cd ..; \
	done
obs/Main.o: Main.cpp \
                            sons_of_sol/SonsOfSolApplication.hpp
	$(CPPC) $(CFLAGS) -o $@ -c $<

obs/AIShip.o: sons_of_sol/private_db/AIShip.cpp \
                            sons_of_sol/AIShip.hpp
	$(CPPC) $(CFLAGS) -o $@ -c $<

obs/ControlMultiplexer.o: sons_of_sol/private_db/ControlMultiplexer.cpp \
                            sons_of_sol/ControlMultiplexer.hpp
	$(CPPC) $(CFLAGS) -o $@ -c $<

obs/PlayerShip.o: sons_of_sol/private_db/PlayerShip.cpp \
                            sons_of_sol/PlayerShip.hpp
	$(CPPC) $(CFLAGS) -o $@ -c $<

obs/ApplicationInit.o: sons_of_sol/private_db/ApplicationInit.cpp \
                            sons_of_sol/SonsOfSolApplication.hpp
	$(CPPC) $(CFLAGS) -o $@ -c $<

obs/SonsOfSolApplication.o: sons_of_sol/private_db/SonsOfSolApplication.cpp \
                            sons_of_sol/SonsOfSolApplication.hpp
	$(CPPC) $(CFLAGS) -o $@ -c $<

obs/Projectile.o: sons_of_sol/private_db/Projectile.cpp \
                            sons_of_sol/Projectile.hpp
	$(CPPC) $(CFLAGS) -o $@ -c $<

obs/ApplicationDisplay.o: sons_of_sol/private_db/ApplicationDisplay.cpp \
                            sons_of_sol/SonsOfSolApplication.hpp
	$(CPPC) $(CFLAGS) -o $@ -c $<

