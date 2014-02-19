
with this script you can avoid using ex*el to design /define your flash partition layout.

```
[i] NAND partition layout:
-------------------------------------------------------------------------------------------------------------
index         identifier    start addr  end addr    offset      size [kib]  size [blocks]
-------------------------------------------------------------------------------------------------------------
  0                      SPL    0x00000000  0x0001FFFF  0x00000000  128     1
  1             SPL Backup 1    0x00020000  0x0003FFFF  0x00020000  128     1
  2             SPL Backup 2    0x00040000  0x0005FFFF  0x00040000  128     1
  3             SPL Backup 3    0x00060000  0x0007FFFF  0x00060000  128     1
  4                   U-Boot    0x00080000  0x000FFFFF  0x00080000  512     4
  5       U-Boot Environment    0x00100000  0x0011FFFF  0x00100000  128     1
  6      Rescue Linux Kernel    0x00120000  0x0041FFFF  0x00120000  3072    24
  7  Rescue Root File System    0x00420000  0x0141FFFF  0x00420000  16384   128
  8             Linux Kernel    0x01420000  0x0171FFFF  0x01420000  3072    24
  9         Root File System    0x01720000  0x0571FFFF  0x01720000  65536   512
 10               User Data     0x05720000  0x05B1FFFF  0x05720000  4096    32
 11               Media Data    0x05B20000  0x05B1FFFF  0x05B20000  37760   295

summary:
nand flash size:                                131072kib, 1024 blocks (block size: 128kib)
partitioned nand flash size:                    131072kib, 1024 blocks (block size: 128kib)
free / unpartitioned nand flash size:           0kib, 0 blocks (block size: 128kib)
sam@nemo:~/projects/github/kickstart/nand_flash_partition_checker$ 
```
