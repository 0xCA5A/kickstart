#!/usr/bin/env python
# -*- coding: utf-8 -*-


import sys


class NANDFlashPartition(object):
    """class to store nand flash partition information"""
    def __init__(self, nand_flash_configuration, partition_name, size_in_kib):
        #print "[i] hello from NANDFlashPartition constructor"

        # check block alignment
        if size_in_kib % nand_flash_configuration.get_block_size_in_kib() != 0 :
            print "[!] error in alignment, %d not a multiple of %d" % (size_in_kib, nand_flash_configuration.get_block_size_in_kib())
            sys.exit(1)

        self._nand_flash_configuration = nand_flash_configuration
        self._partition_name = partition_name
        self._size_in_kib = size_in_kib

    def get_partition_name(self):
        """partition name accessor"""
        return self._partition_name

    def get_size_in_kib(self):
        """size accessor"""
        return self._size_in_kib

    def get_size_in_blocks(self):
        """size accessor"""
        return self._size_in_kib / self._nand_flash_configuration.get_block_size_in_kib()


class NANDFlashConfiguration(object):
    """class to store nand flash configuration as erase block - and page size"""
    def __init__(self, page_size_in_kib, block_size_in_kib, nand_flash_size_in_kib, nand_base_address):
        print "[i] hello from NANDFlashConfiguration constructor"
        print "    page size: %dkib" % (page_size_in_kib)
        print "    block size: %dkib" % (block_size_in_kib)
        print "    nand flash size: %dkib" % (nand_flash_size_in_kib)
        print "    nand base address: 0x%08X" % (nand_base_address)
        self._page_size_in_kib = page_size_in_kib
        self._block_size_in_kib = block_size_in_kib
        self._nand_flash_size_in_kib = nand_flash_size_in_kib
        self._nand_base_address = nand_base_address

    def get_page_size_in_kib(self):
        """page size accessor"""
        return self._page_size_in_kib

    def get_block_size_in_kib(self):
        """block size accessor"""
        return self._block_size_in_kib

    def get_nand_flash_size_in_kib(self):
        """size accessor"""
        return self._nand_flash_size_in_kib

    def get_nand_base_address(self):
        """base address accessor"""
        return self._nand_base_address


class NANDFlashPartitionChecker(object):
    """class to design / check a nand flash partition layout, block aligned..."""

    def __init__(self):
        """constructor, init nand flash partition list"""
        print '[i] hello from NANDFlashPartitionChecker constructor, i am so happy to be here...'
        self.nand_flash_partition_list = []

    def add_nand_partition(self, nand_flash_partition):
        """add nand partition to local list"""
        print '[i] add NAND partition: size %dkib, identifier %s' % (nand_flash_partition.get_size_in_kib(), nand_flash_partition.get_partition_name())
        self.nand_flash_partition_list.append(nand_flash_partition)

    def print_nand_partition_layout(self, nand_flash_configuration):
        """print partition layout using the partition information added to the local list (using add_nand_partition function)"""
        print '[i] NAND partition layout:'


        partition_index = 0
        offset_in_blocks = 0
        offset_in_kib = 0

        print "-------------------------------------------------------------------------------------------------------------"
        print "index\t\t  identifier\tstart addr\tend addr\toffset\t\tsize [kib]\tsize [blocks]"
        print "-------------------------------------------------------------------------------------------------------------"
        for nand_flash_partition in self.nand_flash_partition_list:

            if nand_flash_partition.get_size_in_blocks() != 0:
                size_in_kib = nand_flash_partition.get_size_in_kib()
                size_in_blocks = nand_flash_partition.get_size_in_blocks()
            else:
                size_in_kib = nand_flash_configuration.get_nand_flash_size_in_kib() - offset_in_kib
                size_in_blocks = nand_flash_configuration.get_nand_flash_size_in_kib() / nand_flash_configuration.get_block_size_in_kib() - offset_in_blocks

                # check for configuration errors
                if partition_index != (len(self.nand_flash_partition_list) - 1):
                    print "[!] 0 partition size terminator not allowed in the middle of the partitions"
                    sys.exit(1)

            print " %2d%25s\t0x%08X\t0x%08X\t0x%08X\t%d\t\t%d" % (partition_index,
                nand_flash_partition.get_partition_name(),
                offset_in_kib * 1024 + nand_flash_configuration.get_nand_base_address(),
                offset_in_kib * 1024 + nand_flash_partition.get_size_in_kib() * 1024 + nand_flash_configuration.get_nand_base_address() - 1,
                offset_in_kib * 1024,
                size_in_kib,
                size_in_blocks)

            partition_index += 1
            offset_in_kib += size_in_kib
            offset_in_blocks += size_in_blocks

        print "\nsummary:"
        print "nand flash size:\t\t\t%dkib, %d blocks (block size: %dkib)" % (nand_flash_configuration.get_nand_flash_size_in_kib(),
            nand_flash_configuration.get_nand_flash_size_in_kib() / nand_flash_configuration.get_block_size_in_kib(),
            nand_flash_configuration.get_block_size_in_kib())
        print "partitioned nand flash size:\t\t%dkib, %d blocks (block size: %dkib)" % (offset_in_kib,
            offset_in_blocks,
            nand_flash_configuration.get_block_size_in_kib())
        print "free / unpartitioned nand flash size:\t%dkib, %d blocks (block size: %dkib)" % (nand_flash_configuration.get_nand_flash_size_in_kib() - offset_in_kib,
            nand_flash_configuration.get_nand_flash_size_in_kib() / nand_flash_configuration.get_block_size_in_kib() - offset_in_blocks,
            nand_flash_configuration.get_block_size_in_kib())


if __name__ == "__main__":

    MY_NAND_FLASH_CONFIGURATION = NANDFlashConfiguration(2, 128, 128 * 1024, 0x00000000)
    CHECKER = NANDFlashPartitionChecker()

    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "SPL", 128))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "SPL Backup 1", 128))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "SPL Backup 2", 128))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "SPL Backup 3", 128))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "U-Boot", 512))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "U-Boot Environment", 128))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "Rescue Linux Kernel", 3 * 1024))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "Rescue Root File System", 16 * 1024))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "Linux Kernel", 3 * 1024))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "Root File System", 64 * 1024))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "User Data", 4 * 1024))
    CHECKER.add_nand_partition(NANDFlashPartition(MY_NAND_FLASH_CONFIGURATION, "System Data", 0))

    CHECKER.print_nand_partition_layout(MY_NAND_FLASH_CONFIGURATION)






