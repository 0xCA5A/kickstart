ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src
EXTRA_DIST = autogen.sh

INSTALL_DIR = $(shell pwd)/build
NR_OF_PARALLEL_OBJECTS = 3

.PHONY: fftw-3.3.4
fftw-3.3.4:
	if [ ! -f fftw-3.3.4.tar.gz ]; \
	then \ 
		wget http://www.fftw.org/fftw-3.3.4.tar.gz ; \ 
	fi
	rm -rf fftw-3.3.4
	tar xf fftw-3.3.4.tar.gz
	cd ./fftw-3.3.4/; ./configure --prefix=$(INSTALL_DIR); cd -
	make -j$(NR_OF_PARALLEL_OBJECTS) -C ./fftw-3.3.4 install
