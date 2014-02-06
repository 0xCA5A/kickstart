

OBJ_FILES := enr_jack_client.o
HEADER_FILES := $(wildcard *.h)

CFLAGS += -mcpu=cortex-a9 --mfpu=neon-vfpv3 -O3 -Wall -std=gnu99
LDFLAGS +=-ljack
APP_BINARY := enr_jack_client


all: $(OBJ_FILES) $(HEADER_FILES)
	gcc $(CFLAGS) -o $(APP_BINARY) $(OBJ_FILES) $(LDFLAGS)

# %.o: %.c $(HEADER_FILES)
# 	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o
	rm -f $(APP_BINARY)
	rm doc/html -rf
