CC = g++

LDFLAGS = -ljsoncpp
# DEFINES += -DDEBUG
# DEFINES += -DFUNCTIONTRACE
CPPFLAGS = -Wall -Werror -O2


# OBJECTS = UDPSocketServer.o UnixDomainSocketServer.o UinputCommander.o JSONAnalyzer.o
HEADERS = $(shell find . -name '*.h')
SOURCES = $(shell find . -name '*.cpp' | grep -v main.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)

APPLICATION_NAME = MouseMover


all: $(APPLICATION_NAME)


%.o: %.cpp %.h
	$(CC) -c $(CPPFLAGS) $(DEFINES) $<


$(APPLICATION_NAME): main.cpp $(OBJECTS) $(HEADERS)
	$(CC) $(CPPFLAGS) main.cpp $(OBJECTS) -o $(APPLICATION_NAME) $(LDFLAGS) $(DEFINES)


.PHONY: clean

clean:
	rm -fv *.o
	rm -fv $(APPLICATION_NAME)

