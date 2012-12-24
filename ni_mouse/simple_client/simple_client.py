#!/usr/bin/env python


import socket
import sys
import time




# Create a UDS socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)

# Connect the socket to the port where the server is listening
server_address = '/tmp/myUnixDomainSocket'
print >>sys.stderr, 'connecting to %s' % server_address

try:
    sock.connect(server_address)
except socket.error, msg:
    print >>sys.stderr, msg
    sys.exit(1)



print "wait...."
#time.sleep(5)
print "go!"

value = 0



for i in range(1, 1000):
    try:
        key1pressed = "false"
        # Send data

        if (i % 40 < 10):
            dx_sign = 1
            dy_sign = 1
            key1pressed = "true"
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
            #message = 'This is the message.  It will be repeated.'
            message = '{ "dxPos": %d, "dyPos": %d, "key1pressed": %s , "key2pressed": false }' % (dx_sign, dy_sign, key1pressed)

        print 'sending: %s' % message
        sock.sendall(message)
        #sock.send(message)

        print "%d chars sent" % (len(message))


        raw_input('enter to continue... ')

        #time.sleep(0.03)

        #input

        #amount_received = 0
        #amount_expected = len(message)

        #while amount_received < amount_expected:
            #data = sock.recv(16)
            #amount_received += len(data)
            #print >>sys.stderr, 'received "%s"' % data



    except:
        print "oooops, caught an error... "

        
    #message = '{ "dxPos": 0, "dyPos": 0, "key1pressed": false , "key2pressed": false }'
    #sock.sendall(message)
             
    #finally:
        #print >>sys.stderr, 'closing socket'
        #sock.close()