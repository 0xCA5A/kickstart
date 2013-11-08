

OBJ_FILES := MixerApplication.o MixerAlgorithmSimpleAddWithClipping.o MixerAlgorithmSimpleAddWithNormalization.o MixerAlgorithmSimpleAddWithSmoothNormalization.o MixerAlgorithmRMSGainBeforeSum.o MixerAlgorithmRMSGainBeforeAndAfterSum.o MixerAlgorithmRMSGainAfterSum.o
HEADER_FILES := $(wildcard *.hpp *.h)
CPPFLAGS += -Wall -O3
LDFLAGS += -l sndfile
APP_BINARY := mixerapplication



all: $(OBJ_FILES) $(HEADER_FILES)
	g++ $(CPPFLAGS) -o $(APP_BINARY) main.cpp $(OBJ_FILES) $(LDFLAGS)

%.o: %.cpp $(HEADER_FILES)
	g++ -c $(CPPFLAGS) -o $@ $<

clean:
	rm -f *.o
	rm -f $(APP_BINARY)

doc:
	cd doc; doxygen Doxyfile; cd -