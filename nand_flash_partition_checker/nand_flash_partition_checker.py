#!/usr/bin/env python
# -*- coding: utf-8 -*-


import sys


class NANDFlashPartition(object):
    def __init__(self, nand_flash_configuration, partition_name, size_in_KiB):
        #print "[i] hello from NANDFlashPartition constructor"

        # check block alignment
        if size_in_KiB % nand_flash_configuration.get_block_size_in_KiB() != 0 :
            print "[!] error in alignment, %d not a multiple of %d" % (size_in_KiB, nand_flash_configuration.get_block_size_in_KiB())
            sys.exit(1)

        self._nand_flash_configuration = nand_flash_configuration
        self._partition_name = partition_name
        self._size_in_KiB = size_in_KiB

    def get_partition_name(self):
        return self._partition_name

    def get_size_in_KiB(self):
        return self._size_in_KiB

    def get_size_in_blocks(self):
        return self._size_in_KiB / self._nand_flash_configuration.get_block_size_in_KiB()


class NANDFlashConfiguration(object):
    def __init__(self, page_size_in_KiB, block_size_in_KiB, nand_flash_size_in_KiB, nand_base_address):
        print "[i] hello from NANDFlashConfiguration constructor"
        print "    page size: %dKiB" % (page_size_in_KiB)
        print "    block size: %dKiB" % (block_size_in_KiB)
        print "    nand flash size: %dKiB" % (nand_flash_size_in_KiB)
        print "    nand base address: 0x%08X" % (nand_base_address)
        self._page_size_in_KiB = page_size_in_KiB
        self._block_size_in_KiB = block_size_in_KiB
        self._nand_flash_size_in_KiB = nand_flash_size_in_KiB
        self._nand_base_address = nand_base_address

    def get_page_size_in_KiB(self):
        return self._page_size_in_KiB

    def get_block_size_in_KiB(self):
        return self._block_size_in_KiB

    def get_nand_flash_size_in_KiB(self):
        return self._nand_flash_size_in_KiB

    def get_nand_base_address(self):
        return self._nand_base_address


class NANDFlashPartitionChecker(object):
    """class to design / check a nand flash partition layout..."""

    def __init__(self):
        print '[i] hello from NANDFlashPartitionChecker constructor, i am so happy to be here...'
        self.nand_flash_partition_list = []

    def addNANDPartition(self, nand_flash_partition):
        print '[i] add NAND partition: size %dKiB, identifier %s' % (nand_flash_partition.get_size_in_KiB(), nand_flash_partition.get_partition_name())
        self.nand_flash_partition_list.append(nand_flash_partition)

    def printNANDPartitionLayout(self):
        print '[i] NAND partition layout:'


        partition_index = 0
        offset_in_blocks = 0
        offset_in_KiB = 0

        print "-------------------------------------------------------------------------------------------------------------"
        print "index\t\t   identifier\tstart addr\tend addr\toffset\t\tsize [KiB]\tsize [blocks]"
        print "-------------------------------------------------------------------------------------------------------------"
        for nand_flash_partition in self.nand_flash_partition_list:

            if nand_flash_partition.get_size_in_blocks() != 0:
                size_in_KiB = nand_flash_partition.get_size_in_KiB()
                size_in_blocks = nand_flash_partition.get_size_in_blocks()
            else:
                size_in_KiB = nand_flash_configuration.get_nand_flash_size_in_KiB() - offset_in_KiB
                size_in_blocks = nand_flash_configuration.get_nand_flash_size_in_KiB() / nand_flash_configuration.get_block_size_in_KiB() - offset_in_blocks

                # check for configuration errors
                if partition_index != (len(self.nand_flash_partition_list) - 1):
                    print "[!] 0 partition size terminator not allowed in the middle of the partitions"
                    sys.exit(1)

            print " %2d%25s\t0x%08X\t0x%08X\t0x%08X\t%d\t\t%d" % (partition_index,
                nand_flash_partition.get_partition_name(),
                offset_in_KiB * 1024 + nand_flash_configuration.get_nand_base_address(),
                offset_in_KiB * 1024 + nand_flash_partition.get_size_in_KiB() * 1024 + nand_flash_configuration.get_nand_base_address() - 1,
                offset_in_KiB * 1024,
                size_in_KiB,
                size_in_blocks)

            partition_index += 1
            offset_in_KiB += size_in_KiB
            offset_in_blocks += size_in_blocks



if __name__ == "__main__":

    nand_flash_configuration = NANDFlashConfiguration(2, 128, 128 * 1024, 0x81000000)
    checker = NANDFlashPartitionChecker()

    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "SPL", 128))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "SPL Backup 1", 128))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "SPL Backup 2", 128))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "SPL Backup 3", 128))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "U-Boot", 2 * 1024))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "U-Boot Environment", 128))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "Rescue Linux Kernel", 3 * 1024))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "Rescue Root File System", 16 * 1024))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "Linux Kernel", 3 * 1024))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "Root File System", 64 * 1024))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "User Data", 4 * 1024))
    checker.addNANDPartition(NANDFlashPartition(nand_flash_configuration, "System Data", 0))

    checker.printNANDPartitionLayout()






