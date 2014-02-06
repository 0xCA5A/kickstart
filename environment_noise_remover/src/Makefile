

OBJ_FILES := enr_jack_client.o
HEADER_FILES := $(wildcard *.h)
CFLAGS += -Wall -O2 -march=armv7-a -mfpu=neon
LDFLAGS +=-ljack
APP_BINARY := enr_jack_client



all: $(OBJ_FILES) $(HEADER_FILES)
	g++ $(CPPFLAGS) -o $(APP_BINARY) $(OBJ_FILES) $(LDFLAGS)

%.o: %.cpp $(HEADER_FILES)
	g++ -c $(CPPFLAGS) -o $@ $<

clean:
	rm -f *.o
	rm -f $(APP_BINARY)
	rm doc/html -rf
