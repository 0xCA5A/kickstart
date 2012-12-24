CC = g++
CPPFLAGS = -Wall -Werror -O2

INCLUDES = -I/home/scale/g++Projects/gLib/

LDFLAGS = -ljsoncpp

OBJECTS = UDPSocketServer.o SocketServer.o UnixDomainSocketServer.o UinputCommander.o JSONAnalyzer.o
APPLICATION_NAME = mouseMover


all: $(APPLICATION_NAME)


# $@: $@.cpp
# 	$(CC) -c $< $(LDFLAGS)

# $@.o: $main.cpp
# 	$(CC) -c $(CFLAGS) $<


$(APPLICATION_NAME): main.cpp $(OBJECTS)
	$(CC) $(CPPFLAGS) main.cpp $(OBJECTS) -o $(APPLICATION_NAME) $(LDFLAGS)


.PHONY: clean

clean:
	rm -fv *.o
	rm -fv $(APPLICATION_NAME)

