#!/usr/bin/env python

# pylint: disable=C0301

"""script to modify the isc dhcp configuration depending on captured bootp frames

tested with ubuntu 12.04
    * python 2.7.3

additional packages:
    * python-libpcap

add this markers to your /etc/dhcp/dhcpd.conf:

### BEGIN GENERATED CONTENT
### END GENERATED CONTENT

if a bootp frame is received, the script will fill in the necessary section
to feed the requesting device with files.

to start this permanently, copy the python source file and add a line like this to your /etc/rc.local:


/opt/DHCPDManager.py eth0 &> /var/log/DHCPDManager.log &

"""


import pcap
import sys
import os
import time
import socket
import struct
import re
import subprocess


class ISCDHCPServerManager(object):
    """isc dhcp server related stuff
    """
    def __init__(self):
        print '[%s] create ISCDHCPServerManager object' % (self.__class__.__name__)
        self.__dhcpd_configuration_file = '/etc/dhcp/dhcpd.conf'
        self.__dhcpd_config_start_delimiter = '### BEGIN GENERATED CONTENT'
        self.__dhcpd_config_end_delimiter = '### END GENERATED CONTENT'

        self.__chop_regex_pattern = '%s.*?%s' % (self.__dhcpd_config_start_delimiter, self.__dhcpd_config_end_delimiter)
        self.__chop = re.compile(self.__chop_regex_pattern, re.DOTALL)

        self.__mac_address_place_holder = 'MAC_ADDRESS_PLACE_HOLDER'
        self.__ip_address_place_holder = 'IP_ADDRESS_PLACE_HOLDER'

        self.__config_section = 'host production_device {  server-name "servername";  hardware ethernet %s; fixed-address %s; \
                    if substring (option vendor-class-identifier, 0, 10) = "DM814x ROM" \
                    {  filename "u-boot-spl-debrick.bin";  } elsif substring \
                    (option vendor-class-identifier, 0, 17) = "AM335x U-Boot SPL" \
                    {  filename "u-boot-debrick.img";  } else {  filename "uImage";  }  }' % (self.__mac_address_place_holder, self.__ip_address_place_holder)

    def check_for_service(self):
        print '[%s] check if isc dhcp server service is ready' % (self.__class__.__name__)

        command = ["service", "isc-dhcp-server"]
        my_process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = my_process.communicate()

        if re.findall('unrecognized', out) or re.findall('unrecognized', err):
            return False

        return True

    def restart_service(self):
        print '[%s] restart isc dhcp server service' % (self.__class__.__name__)

        command = ["service", "isc-dhcp-server", "restart"]
        my_process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        _, _ = my_process.communicate()

    def check_for_config_file(self):
        print '[%s] check if config file %s is ready' % (self.__class__.__name__, self.__dhcpd_configuration_file)
        if not os.path.isfile(self.__dhcpd_configuration_file):
            return False

        return True

    def check_config_for_delimiter(self):
        print '[%s] check configuration file %s for delimiter' % (self.__class__.__name__, self.__dhcpd_configuration_file)

        with open(self.__dhcpd_configuration_file) as config_file:
            file_content = config_file.readlines()

        start_delimiter_found = False
        end_delimiter_found = False

        for line in file_content:
            if re.findall(self.__dhcpd_config_start_delimiter, line):
                start_delimiter_found = True
                print '\tstart delimiter found'
            if re.findall(self.__dhcpd_config_end_delimiter, line):
                end_delimiter_found = True
                print '\tend delimiter found'

        if start_delimiter_found and end_delimiter_found:
            return True

        return False

    def adjust_configuration(self, device_mac_address, device_ip_address):
        print '[%s] adjust configuration section: device mac: %s, device ip: %s' % (self.__class__.__name__, device_mac_address, device_ip_address)
        with open(self.__dhcpd_configuration_file, 'r') as config_file:
            config_file_content = config_file.read()

        new_config_section = "%s\n%s\n%s" % (self.__dhcpd_config_start_delimiter,
                self.__config_section.replace(self.__mac_address_place_holder, device_mac_address).replace(self.__ip_address_place_holder, device_ip_address),
                self.__dhcpd_config_end_delimiter)
        new_config_file_content = self.__chop.sub(new_config_section, config_file_content)

        with open(self.__dhcpd_configuration_file, 'w+') as config_file:
            config_file.write(new_config_file_content)


class DHCPDManager(object):
    """pcap and bootp related stuff
    """
    def __init__(self, dev):
        print '[%s] create DHCPDManager object' % (self.__class__.__name__)

        self.__dev = dev
        self.__last_configured_device_mac = 'nothing'
        self.__device_default_ip_address = '10.0.42.42'
        self.__pcap_obj = pcap.pcapObject()

        # frame filter definition
        # http://wiki.wireshark.org/DHCP
        # bootp frame ports
        # 67/UDP (request)
        # 68/UDP (response)
        self.__filter_string = 'udp dst port 67 and src port 68'
        self.__isc_dhcp_server_manager = ISCDHCPServerManager()

    @staticmethod
    def decode_ip_packet(data):
        decoded_data = {}
        decoded_data['dst_mac'] = ':'.join(x.encode('hex') for x in data[0:6])
        decoded_data['src_mac'] = ':'.join(x.encode('hex') for x in data[6:12])
        decoded_data['ethettype'] = data[12:14]
        decoded_data['version'] = (ord(data[14]) & 0xf0) >> 4
        decoded_data['header_len'] = ord(data[14]) & 0x0f
        decoded_data['tos'] = ord(data[15])
        decoded_data['id'] = socket.ntohs(struct.unpack('H', data[18:20])[0])
        decoded_data['protocol'] = ord(data[23])
        decoded_data['source_address'] = pcap.ntoa(struct.unpack('i', data[26:30])[0])
        decoded_data['destination_address'] = pcap.ntoa(struct.unpack('i', data[30:34])[0])

        return decoded_data

    #def dumphex(self, data):
        #date_bytes = map(lambda x: '%.2x' % x, map(ord, data))
        #i = 0
        #for i in xrange(0, len(date_bytes) / 16):
            #print '\t%s' % string.join(date_bytes[i * 16:(i+1) * 16], ' ')
        #print '\t%s' % string.join(date_bytes[(i+1) * 16:], ' ')

    def start(self):
        if not self.__isc_dhcp_server_manager.check_for_service():
            print '[%s] isc dhcp server unrecognized, exit immediately' % (self.__class__.__name__)
            sys.exit(1)

        if not self.__isc_dhcp_server_manager.check_for_config_file():
            print '[%s] isc dhcp server configuration file not found' % (self.__class__.__name__)
            sys.exit(1)

        if not self.__isc_dhcp_server_manager.check_config_for_delimiter():
            print '[%s] delimiter not found in dhcpd.conf, exit immediately' % (self.__class__.__name__)
            sys.exit(1)

        print '[%s] start pcap live capture' % (self.__class__.__name__)
        # NOTE(casasam):  to_ms does nothing if running on linux
        snaplen = 1600
        promisc = 0
        to_ms = 100
        self.__pcap_obj.open_live(self.__dev, snaplen, promisc, to_ms)

        print '[%s] set filter string to "%s"' % (self.__class__.__name__, self.__filter_string)
        self.__pcap_obj.setfilter(self.__filter_string, 0, 0)

        # self.__pcap_obj.setnonblock(1)

        sys.stdout.flush()

        # try-except block to catch keyboard interrupt. failure to shut
        # down cleanly can result in the interface not being taken out of promisc
        # mode.
        try:
            while 1:
                self.__pcap_obj.dispatch(1, self.check_for_bootp_request)

        except KeyboardInterrupt:
            print '[%s] shutting down, "%s"' % (self.__class__.__name__, sys.exc_type)
            print '\t%d packets received, %d packets dropped, %d packets dropped by interface' % (self.__pcap_obj.stats())

    def check_for_bootp_request(self, pktlen, data, timestamp):
        if not data:
            return

        decoded = self.decode_ip_packet(data)
        print '\n[%s] got frame: %s.%f %s > %s, mac source: %s' % (self.__class__.__name__,
                                    time.strftime('%H:%M',
                                    time.localtime(timestamp)),
                                    timestamp % 60,
                                    decoded['source_address'],
                                    decoded['destination_address'],
                                    decoded['src_mac'])

        # try to find the string "DM814x ROM" in the Ethernet frame.
        # the am335x romboot code sends DM814x as identifier,
        # there is a entry in the silicon errata about this issue.

        romboot_bootp_identifier = 'DM814x ROM'
        #u_boot_bootp_identifier = 'AM335x U-Boot SPL'

        # check if frame contains the identifier to differ DHCP and BOOTP frames
        # FIXME(scasa): find a smarter way to do that...
        print '[%s] try to find pattern "%s" in frame content' % (self.__class__.__name__, romboot_bootp_identifier)
        if not re.findall(romboot_bootp_identifier, data):
            print '\tnot interesting, drop frame'
        else:
            print '\tpattern %s found in bootp request frame' % (romboot_bootp_identifier)

            device_ip_address = '10.0.42.123'

            #change configuration if client not yet configured
            if self.__last_configured_device_mac != decoded['src_mac']:
                self.__last_configured_device_mac = decoded['src_mac']

                self.__isc_dhcp_server_manager.adjust_configuration(decoded['src_mac'], device_ip_address)
                self.__isc_dhcp_server_manager.restart_service()

        sys.stdout.flush()


if __name__ == '__main__':

    #LOG_FILE = '/var/log/DHCPDManager.log'

    #print '[main] log to file %s' % (LOG_FILE)
    #sys.stdout = open(LOG_FILE, 'w+')

    # check command line arguments
    if len(sys.argv) < 2:
        print 'usage: %s <interface>' % (sys.argv[0])
        sys.exit(0)

    DEV = sys.argv[1]
    # net, mask = pcap.lookupnet(DEV)

    DHCP_CONFIG_MANAGER = DHCPDManager(DEV)
    DHCP_CONFIG_MANAGER.start()

    print '[main] leave main gracefully'
