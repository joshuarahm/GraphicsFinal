Sons Of Sol
=============

Build Instructions
==================

Most of the time just run
$ make

The makefiles should be updated and working, but
in case they are not, you can run:

$ ./genmake.sh # generate bootstrap Makefile
$ make genmake # recursively generate other makefiles
$ make

(To use genmake, you must have gawk installed)

Defaultly, the project compiles with all debugging flags enabled
and no optimizations. To compile with optimizations run

$ OPTFLAGS=-O<Level> make

NOTES
====

This code has been verified to work on the CSEL machines
with the fallback shader (see below)

libglox and libslox are both projects written by me this year
for the purpose of aiding me with OpenGL and SDL

Story
============

The year is 2183 and the rebel cause is all but extinguished and an Imperial
victory seems almost all but inevitable. One lingering
hope remains with the frigate Industrial and its X25B fighter
escorts if they can destroy the Death Star. You are
a part of a quick fighter response team known as the Sons of Sol who have
been enlisted to protect the frigate from the Death Star and its fighter
as it makes a close approach to fire its main weapon and save the rebel cause
and ensuring the survival of liberty for years to come.

You are due for engagement now.

Controls
========

Movement 

W/S   - Forward/Back
Mouse - Pitch/Yaw
Q/E   - Roll
A/D   - Strafe Left/Right
Z/X   - Strafe Up/Down
Shift or Spacebar - Shoot

What to look for
================

Once you leave the initial launch tube, you will
have control of your ship. There are other ships that
orbit around the death star and they look like little
moving stars when far away.

Near the starting point, there are 5 moving ships that follow
ellipses around the starting point. To get there, just turn
around after leaving the acceleration tube and find your way back.

Likewise, there are ships around the frigate you can find much like
around the death star.

In addition, the Death Star has a few openings that you can fly into and
try to make your way to the Death Star's reactor core.


Cool graphics things
====================

There are 2 shaders, one for the moon and
one for the Earth. The earth shader is implemented
two different ways because I found that they work
differently depending on the version of glsl

If using the primary shader for the earth (glsl >= 1.3)
then the earth rotates as a small, but noticable speed.

The ships dynamically change the resolution of
the model the closer you get. That is how I am able
to have literaly a hundred of the little ships.

All 6 degrees of freedom actually work, although
strafing is intentionally slow.

The controls use a smooth averaging algorithm to produce
the smooth control effects.

The initial starting light is a directional light that
changes from red to green when it is time to accelerate.

The spaceship jitter is described as a function of the
acceleration and if the gun is firing

Lighting actually comes from the sun, and a secondary, smaller
light is emitted from the Earth.

The small ships employ complex alorithms to yaw, pitch and roll
with the line they follow to make it look more realistic (Follow one, it's quite cool)

The bullets from the gun are lines drawn along the vector of motion
that have an alpha gradient and their color decays over time.

Some Caveats
============

The Earth, Moon and Sun move with you, so don't
try to get to them because you can't.

If you go too far past the death star or the frigate
and look back, there will be some Z fighting because there
is no limit on how far you can go.

There is no collision or hit detection :-( but it is still
fun to pretend.

Textures could be better but this isn't for a photoshop class
so, I think they are good enough.

If you are using glsl < 1.3 (like the CSEL machines) then you
can't use the primary shader, but never fear! I included a screenshot
of it in the 'screenshots' directory. I'm proud of that shader.
