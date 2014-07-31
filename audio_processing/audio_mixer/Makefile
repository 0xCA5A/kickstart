

OBJ_FILES := main.o MixerApplication.o MixerAlgorithmSimpleAddWithClipping.o MixerAlgorithmSimpleAddWithNormalization.o MixerAlgorithmFancyAddWithNormalization.o MixerAlgorithmRMSGainBeforeSum.o MixerAlgorithmRMSGainBeforeAndAfterSum.o MixerAlgorithmRMSGainAfterSum.o
HEADER_FILES := $(wildcard *.hpp *.h)
CPPFLAGS += -Wall -O3
LDFLAGS += -l sndfile
APP_BINARY := mixerapplication



all: $(OBJ_FILES) $(HEADER_FILES)
	g++ $(CPPFLAGS) -o $(APP_BINARY) $(OBJ_FILES) $(LDFLAGS)

%.o: %.cpp $(HEADER_FILES)
	g++ -c $(CPPFLAGS) -o $@ $<

clean:
	rm -f *.o
	rm -f *.*~
	rm -f $(APP_BINARY)
	rm doc/html -rf
	
doxygen:
	cd doc; doxygen Doxyfile; cd -
