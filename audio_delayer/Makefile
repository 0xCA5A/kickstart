

OBJ_FILES := RIFFWaveFileShifter.o
CPPFLAGS += -Wall
APP_BINARY := riffwavefileshifter


all: $(OBJ_FILES)
	g++ $(CPPFLAGS) -o $(APP_BINARY) main.cpp $(OBJ_FILES) -lsndfile

clean:
	rm -f *.o
	rm -f $(APP_BINARY)

