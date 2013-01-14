#!/usr/bin/env python

import socket
import sys
import time


MY_UNIX_DOMAIN_SOCKET_NAME = '/tmp/myUnixDomainSocket'
NR_OF_MOVEMENTS = 1000


#create a UDS socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)

#connect the socket to the port where the server is listening
server_address = MY_UNIX_DOMAIN_SOCKET_NAME
print '[i] connecting to %s' % server_address

try:
    sock.connect(server_address)
except socket.error, msg:
    print '[!] ' +  repr(msg)
    sys.exit(1)


print "[i] wait...."
#time.sleep(5)
print "[i] go!"


for i in range(1, NR_OF_MOVEMENTS):

    try:
        key1pressed = "false"

        if (i % 40 < 10):
            dx_sign = 1
            dy_sign = 1
            #key1pressed = "true"

        elif (i % 40 < 20):
            dx_sign = 1
            dy_sign = -1

        elif (i % 40 < 30):
            dx_sign = -1
            dy_sign = -1

        elif (i % 40 < 40):
            dx_sign = -1
            dy_sign = 1

        #if (i % 200 < 77):
            #key1pressed = "true"
        #else:
            #key1pressed = "false"

        if len(sys.argv) > 1:
            message = sys.argv[1]
        else:
            message = '{ "dxPos": %d, "dyPos": %d, "key1pressed": %s , "key2pressed": false }' % (dx_sign, dy_sign, key1pressed)

        print 'sending: %s' % message
        sock.sendall(message)
        #sock.send(message)

        print "%d chars sent" % (len(message))

        #raw_input('[i] enter to continue... ')

        time.sleep(0.2)


    except:
        print "oooops, caught an error... "
