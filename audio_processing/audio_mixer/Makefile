

OBJ_FILES := MixerApplication.o MixerAlgorithmSimpleAddWithClipping.o MixerAlgorithmSimpleAddWithNormalization.o
CPPFLAGS += -Wall
APP_BINARY := mixerapplication


all: $(OBJ_FILES)
	g++ $(CPPFLAGS) -o $(APP_BINARY) main.cpp $(OBJ_FILES)

clean:
	rm -f *.o
	rm -f $(APP_BINARY)
