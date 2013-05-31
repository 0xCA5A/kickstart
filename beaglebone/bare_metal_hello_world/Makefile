
all: clean bin

compile: code.c
	arm-linux-gnueabihf-gcc -mcpu=cortex-a8 -c code.c -o code.o
	arm-linux-gnueabihf-as -mcpu=cortex-a8 -g startup.s -o startup.o
	
link: compile
# 	arm-linux-gnueabihf-ld -T ld.lds --gc-sections -Bstatic -Ttext 0x402F0400 main.o -Map main.map -o main.elf
	arm-linux-gnueabihf-ld -T linker.lds code.o startup.o -o code.elf
	
bin: link
	arm-linux-gnueabihf-objcopy --gap-fill=0xff -O binary code.elf code.bin

deploy: bin
	scp code.bin user@172.29.6.21:/tftpboot/coreboot.bin

clean:
	rm -f code.o code.elf code.bin
	rm -f startup.o
