

OBJ_FILES := main.o RIFFWaveFileShifter.o
CPPFLAGS += -Wall -Werror
APP_BINARY := riffwavefileshifter


all: $(OBJ_FILES)
	g++ $(CPPFLAGS) -o $(APP_BINARY) $(OBJ_FILES) -lsndfile

clean:
	rm -f *.o
	rm -f $(APP_BINARY)

