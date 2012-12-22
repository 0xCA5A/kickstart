CC      = g++
CPP = g+++
CPPFLAGS = -Wall -Werror -O2

INCLUDES = -I/home/scale/g++Projects/gLib/

LDFLAGS = -lm

OBJECTS = UDPSocketServer.o SocketServer.o UnixDomainSocketServer.o UinputCommander.o JSONAnalyzer.o
APPLICATION_NAME = mouse_mover


all: $(APPLICATION_NAME)


# $@: $@.cpp
# 	$(CC) -c $< $(LDFLAGS)

# $@.o: $main.cpp
# 	$(CC) -c $(CFLAGS) $<


$(APPLICATION_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) main.cpp $(OBJECTS) -o $(APPLICATION_NAME) $(LDFLAGS)


.PHONY: clean cleanest

clean:
	rm -fv *.o
	rm -fv $(APPLICATION_NAME)

