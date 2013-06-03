CFLAGS = -mcpu=cortex-a8 -g
TOOLCHAIN_PREFIX = arm-linux-gnueabihf-


all: clean bin

compile: code.c startup.s
	$(TOOLCHAIN_PREFIX)gcc $(CFLAGS) -c code.c -o code.o
	$(TOOLCHAIN_PREFIX)as $(CFLAGS) startup.s -o startup.o

link: compile
	$(TOOLCHAIN_PREFIX)ld -T linker.lds code.o startup.o -o code.elf

bin: link
	$(TOOLCHAIN_PREFIX)objcopy --gap-fill=0xff -O binary code.elf code.bin

deploy: check-env bin
	scp code.bin $(REMOTEUSER)@$(REMOTEHOST):$(REMOTEPATH)

check-env:
	@if test -z "$$REMOTEUSER"; \
		then echo "[!] environment variabel REMOTEUSER is not defined (exported), expect something like 'user')"; \
		exit 1; \
	fi; \

	@if test -z "$$REMOTEHOST"; \
		then echo "[!] environment variabel REMOTEHOST is not defined (exported), expect something like '10.9.27.42')"; \
		exit 1; \
	fi; \

	@if test -z "$$REMOTEPATH"; \
		then echo "[!] environment variabel REMOTEPATH is not defined (exported),expect somethink like '/tftpboot/coreboot.bin')"; \
		exit 1; \
	fi; \

clean:
	rm -f startup.o
	rm -f code.o code.elf code.bin
