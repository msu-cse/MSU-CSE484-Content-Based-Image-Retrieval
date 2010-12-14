from socket import *
import sys
import struct
import tempfile
import os
# Caught exception: The index saved belongs to a different dataset

SERVER_HOST='localhost'
SERVER_PORT=8081

def packSize(size):
    return struct.pack('>L', size)

def unpackSize(sizeBytes):
    return struct.unpack('>L',sizeBytes)[0]

packedSize = struct.calcsize('>L')

def server():
    pass
    

def client(data):
    s = socket()
    s.connect((SERVER_HOST, SERVER_PORT))
    print "connected"
    dataLen = len(data)
    dataLenBytes = packSize(dataLen)
    print "sending data"
    s.send(dataLenBytes)
    s.send(data)
    print "waiting on response"
    response = s.recv(999999)
    print response
    
    s.close()


if __name__ == '__main__':

    # Open the socket
    serversocket = socket(AF_INET,SOCK_STREAM)
    serversocket.bind(('0.0.0.0',SERVER_PORT))
    serversocket.listen(100)
    


    # Accept incoming connections
    while 1:
        print "listening"
        (clientsocket, address) = serversocket.accept()
        print "accepted from %s:%s" % address
        
        # -- Receive the data
        sizeBytes = clientsocket.recv(packedSize)
        size = unpackSize(sizeBytes)
        print "waiting on %s bytes" % (size)
        imgData = clientsocket.recv(int(size))

        
        # -- Write to a temporary file
        filename = "tmp.pgm"
        f = file(filename,'w')
        f.write(imgData)
        f.flush()
        f.close()
        
        # -- Call sift
        # os.system('sift < %s > results.key' % filename)
        
        # -- Process the data, send it back down the pipe.
        # XXX
        
        # -- Close the connection to complete the transaction
        print "sending response"
        clientsocket.send(file("tmp.pgm").read())
        clientsocket.close()