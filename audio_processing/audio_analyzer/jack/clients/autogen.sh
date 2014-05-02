#!/bin/sh
if [ ! -d m4 ]; then
	mkdir m4
fi
if [ ! -d build ]; then
	mkdir build
fi
autoreconf --force --install -I config -I m4
