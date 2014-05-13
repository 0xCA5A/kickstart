wandboard debian setup
======================

u-boot and linux kernel
-----------------------

excellent u-boot and kernel build / mmc card setup howto here [0].

NOTE: the kernel build toolchain is named as 'gnueabihf'. during the kernel build process i have found cc1 processes on my system with switch -mfloat-abi=soft enabled.

if we beleve in this [1]
```
Full software floating point - compiler should refuse to generate a real FPU instruction and -mfpu= is ignored.
```
the -mfpu=vfp switch is ignored.

anyway, float / double grep in kernel source shows only a few results in gpu and video data structures.


```
sam      17902  0.0  0.0  25068  6848 pts/14   R+   14:22   0:00 /home/sam/projects/wandboard/armv7-multiplatform/dl/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin/../libexec/gcc/arm-linux-gnueabihf/4.8.3/cc1 -E -quiet -nostdinc -I /home/sam/projects/wandboard/armv7-multiplatform/KERNEL/arch/arm/include -I arch/arm/include/generated -I include -I /home/sam/projects/wandboard/armv7-multiplatform/KERNEL/arch/arm/include/uapi -I arch/arm/include/generated/uapi -I /home/sam/projects/wandboard/armv7-multiplatform/KERNEL/include/uapi -I include/generated/uapi -imultiarch arm-linux-gnueabihf -iprefix /home/sam/projects/wandboard/armv7-multiplatform/dl/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin/../lib/gcc/arm-linux-gnueabihf/4.8.3/ -isysroot /home/sam/projects/wandboard/armv7-multiplatform/dl/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin/../arm-linux-gnueabihf/libc -D __GENKSYMS__ -D __KERNEL__ -D __LINUX_ARM_ARCH__=7 -U arm -D CC_HAVE_ASM_GOTO -D KBUILD_STR(s)=#s -D KBUILD_BASENAME=KBUILD_STR(blk_flush) -D KBUILD_MODNAME=KBUILD_STR(blk_flush) -isystem /home/sam/projects/wandboard/armv7-multiplatform/dl/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin/../lib/gcc/arm-linux-gnueabihf/4.8.3/include -include /home/sam/projects/wandboard/armv7-multiplatform/KERNEL/include/linux/kconfig.h -MD block/.blk-flush.o.d block/blk-flush.c -mlittle-endian -mabi=aapcs-linux -mno-thumb-interwork -mfpu=vfp -marm -march=armv7-a -mfloat-abi=soft -mtune=cortex-a9 -mtls-dialect=gnu -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -Werror=implicit-function-declaration -Wno-format-security -Wframe-larger-than=1024 -Wno-unused-but-set-variable -Wdeclaration-after-statement -Wno-pointer-sign -Werror=implicit-int -Werror=strict-prototypes -fno-strict-aliasing -fno-common -fno-delete-null-pointer-checks -fno-dwarf2-cfi-asm -funwind-tables -fstack-protector -fomit-frame-pointer -fno-strict-overflow -fconserve-stack -O2
```

Robert Nelson told me this:

```
In the kernel, Floating Point is explicitly not allowed. It's just the way it's always been, as not every cpu has floating point hardware.
```



minicom minirc file
-------------------
```
sam@guido:~/projects/wandboard$ cat /etc/minicom/minirc.wandboard
pu port             /dev/ttyUSB0
pu baudrate         115200
pu rtscts           No
pu xonxoff          Yes
```


[0]
http://eewiki.net/display/linuxonarm/Wandboard

[1]
https://wiki.debian.org/ArmHardFloatPort/VfpComparison





