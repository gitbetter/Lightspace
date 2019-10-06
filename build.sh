#!/bin/bash

dev=false
xcode=false

while test $# -gt 0; do
	case "$1" in
		-d|--development) dev=true; shift;;
		-x|--xcode) xcode=true; shift;;
		*) break;;
	esac
done

if [ ! -d ./build ]; then
	mkdir build;
fi

cd build; rm -rf *; 

if [ "$xcode" = true ]; then
	flags="-G Xcode"
fi

if [ "$dev" = true ]; then
	flags=" $flags -DDEVELOPMENT=ON"
fi

cmake $flags ..;

if [ "$xcode" = false ]; then
	make;
else
	open .;
fi

cd ..;