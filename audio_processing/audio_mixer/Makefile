

OBJ_FILES := MixerApplication.o MixerAlgorithmSimpleAddWithClipping.o MixerAlgorithmSimpleAddWithNormalization.o MixerAlgorithmSimpleAddWithSmoothNormalization.o MixerAlgorithmRMSGainBeforeSum.o
CPPFLAGS += -Wall
LDFLAGS += -l sndfile
APP_BINARY := mixerapplication


all: $(OBJ_FILES)
	g++ $(CPPFLAGS) -o $(APP_BINARY) main.cpp $(OBJ_FILES) $(LDFLAGS)

clean:
	rm -f *.o
	rm -f $(APP_BINARY)

doc:
	cd doc; doxygen Doxyfile; cd -