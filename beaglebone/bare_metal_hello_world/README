# bare metal c project for beaglebone, ti sitara am335x

## doc

check here for further information:
* http://balau82.wordpress.com/2010/02/14/simplest-bare-metal-program-for-arm/
* http://developers.stf12.net/cpp-demo/gcc-linker-script-and-stm32-a-tutorial
* http://stackoverflow.com/questions/6139952/what-is-the-booting-process-for-arm
* http://www.cs.utsa.edu/~whaley/teach/FHPO_F11/ARM/CortAProgGuide.pdf

## reset vector setup
~ do not understand yet...

source: http://www.cs.utsa.edu/~whaley/teach/FHPO_F11/ARM/CortAProgGuide.pdf, 10.4 Exception mode summary, page 161



# quick and dirty bootp howto

## server configuration
* install ubuntu 12.04
* install package isc-dhcp-server
* install package xinetd
* install package tftpd

assume server ip as 10.0.42.1, dhcp subnet 10.0.42.0 (eth0).

## configure dhcp server
```
user@dorie:~$ cat /etc/default/isc-dhcp-server 
...
INTERFACES="eth0"
```

```
user@dorie:~$ cat /etc/dhcp/dhcpd.conf
ddns-update-style none;
log-facility local7;

default-lease-time 6000;
max-lease-time 7200;

allow booting;
allow bootp;


subnet 10.0.42.0 netmask 255.255.255.0 {
  range 10.0.42.100 10.0.42.250;
  option routers 10.0.42.1;
  option domain-name-servers 8.8.8.8;
} 


host REPLACE_WITH_CLIENT_NAME {
  server-name "servername";
  #next-server servername; # tftp server
  hardware ethernet d4:94:de:ad:be:ef;
  fixed-address 10.0.42.69;

  if substring (option vendor-class-identifier, 0, 10) = "DM814x ROM" {
    filename "code.bin";
  }
}
```

## restart dhcp server
```
user@dorie:~$ sudo service isc-dhcp-server restart
```

## configure tftp server
```
user@dorie:~$ cat /etc/xinetd.d/tftp 
service tftp
{
	protocol        = udp
	port            = 69
	socket_type     = dgram
	wait            = yes
	user            = nobody
	server          = /usr/sbin/in.tftpd
	server_args     = /tftpboot
	disable         = no
}
```

```
user@dorie:~$ sudo mkdir /tftpboot
user@dorie:~$ sudo chown user:user /tftpboot/
```

## restart tftp server
```
user@dorie:~$ sudo service xinetd restart
```

## build and put code binary file to the tftp share

```
sam@guido:~/projects/github/kickstart/beaglebone/bare_metal_hello_world$ make
rm -f startup.o
rm -f code.o code.elf code.bin
arm-linux-gnueabihf-gcc -mcpu=cortex-a8 -g -c code.c -o code.o
arm-linux-gnueabihf-as -mcpu=cortex-a8 -g startup.s -o startup.o
arm-linux-gnueabihf-ld -T linker.lds code.o startup.o -o code.elf
arm-linux-gnueabihf-objcopy --gap-fill=0xff -O binary code.elf code.bin
sam@guido:~/projects/github/kickstart/beaglebone/bare_metal_hello_world$ scp code.bin user@10.0.42.1:/tftpboot
code.bin                                      100%  224     0.2KB/s   00:00 
```

## debuggig ethernet communication
use wireshark or tcpdump to debug the bootp communication between server and beaglebone
```
user@dorie:~$ sudo tcpdump -n -i eth0
tcpdump: WARNING: eth0: no IPv4 address assigned
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on eth0, link-type EN10MB (Ethernet), capture size 65535 bytes
21:12:06.016756 IP 0.0.0.0.68 > 255.255.255.255.67: BOOTP/DHCP, Request from 50:56:de:ad:be:ef, length 364
```

or check server messages
```
user@dorie:~$ tail -f /var/log/syslog
Oct 26 22:09:18 dorie dhcpd: BOOTREQUEST from 50:56:63:c6:39:b0 via eth0
Oct 26 22:09:18 dorie dhcpd: BOOTREPLY for 10.0.42.69 to beaglebone (50:56:63:c6:39:b0) via eth0
Oct 26 22:09:19 dorie tftpd[14189]: tftpd: trying to get file: code.bin
Oct 26 22:09:19 dorie tftpd[14189]: tftpd: serving file from /tftpboot
```


