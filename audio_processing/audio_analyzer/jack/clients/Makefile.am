ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src
EXTRA_DIST = autogen.sh




.PHONY: fftw-3.3.4
fftw-3.3.4:
	rm -f fftw-3.3.4.tar.gz
	wget http://www.fftw.org/fftw-3.3.4.tar.gz
	tar xf fftw-3.3.4.tar.gz
	cd fftw-3.3.4; ./configure --prefix=$(INSTALL_DIR); make -j$(NR_OF_PARALLEL_OBJECTS) install; cd -
