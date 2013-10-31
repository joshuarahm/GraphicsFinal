#!/bin/bash

# Makefile for the GL Object eXtensions library

# This bash file is used to generate the makefile
# to produce the library for the graphics library

BINARY_NAME="SonsOfSol"

obs=()
src=()
libs=()

function generate_depends {
	# get the dependencies of the dependencies
	if [ ! -f $1 ] ; then
		return ;
	fi

	next=$(cat $1 | gawk 'match($0, /#include "(.*)"/, m) { print m[1] }')
	deps=$1
	for i in $next ; do
		for canidate in $(generate_depends $i) ; do
			if [[ ! $deps =~ $i ]] ; then
				deps="$deps \\$(echo -ne '\n                            ')$i"
			fi
		done
		if [[ $? -ne 0 ]] ; then
			echo "Failed to generate depends">&2
			return 1;
		fi
	done
	echo "$deps"
}

# Iterate through and find the
# c++ source files 
for i in "$(find Main.cpp sons_of_sol/ | egrep '.*\.c(pp|xx)?$')" ; do
	# add this file to the list of
	# sources
    echo "Generating from source file: $i"
	deps="$(generate_depends $i)"

	# add the resulting object file to
	# the objects
	src[$cnt]=$deps
	obs+=("obs/`basename $i | sed 's/\.c\(pp\|xx\)\?$/.o/g'`")
	cnt=$[cnt + 1]
done

# remove the Makefile if it exists
rm -f Makefile || true

# open Makefile
exec 3<> Makefile

# some commonly used files to generate
echo 'CPPC?=g++'>&3
echo 'AR?=ar'>&3
echo "CFLAGS=$CFLAGS -Wall -Wextra -I. -g3 -ggdb -D DEBUG_LEVEL_TRACE -Islox -Iglox">&3
echo "LDFLAGS=$LDFLAGS -Lslox -Lglox -lslox -lglox -lGL -lGLU -lSDL -lm">&3
echo 'OBJECTS='${obs[@]}>&3
echo 'BINARY='$BINARY_NAME>&3

# Add all, setup and clean rules
echo -e \
'all: submodules setup $(OBJECTS)
	$(CPPC) -o $(BINARY) $(OBJECTS) $(LDFLAGS)
'>&3

echo -e 'genmake:\n\tfind . -name genmake.sh -exec {} \;\n'>&3
echo -e 'setup:\n\tmkdir -p obs/\n'>&3

echo -e \
'submodules:
	for i in $$(find */ -name Makefile) ; do \
		pushd $$(dirname $$i) && make && popd; \
	done
'>&3

echo -e \
'clean:
	- rm -rf obs $(BINARY)
	for i in $$(find */ -name Makefile) ; do \
		pushd $$(dirname $$i) && make clean && popd; \
	done'>&3

# iterate through all of the objects and 
# add a rule for the binary
for ((i=0;i<${#obs[@]};i++)) ; do
    echo "Object file: ${obs[$i]}"
    # add a rule for the new binary
	echo -e "${obs[$i]}: ${src[$i]}\n\t"'$(CPPC) $(CFLAGS) -o $@ -c $<\n'>&3
done

# close Makefile
exec 3>&-
