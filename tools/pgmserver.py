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
    

def client(server,port,data):
    s = socket()
    print "connecting to %s:%s" % (server,port)
    s.connect((server, port))
    print "connected"
    dataLen = len(data)
    dataLenBytes = packSize(dataLen)
    print "sending %s bytes of data" % dataLen
    s.send(dataLenBytes)
    s.send(data)
    print "waiting on response"
    
    responseSizePacked = s.recv(packedSize)
    responseSize = unpackSize(responseSizePacked)
    response = ''
    while len(response) < responseSize:
        response += s.recv(responseSize - len(response))
        print "Received %s bytes " % len(response)
    print "Got:" + response
    s.close()
    
    return response


if __name__ == '__main__':

    # Open the socket
    serversocket = socket(AF_INET,SOCK_STREAM)
    serversocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
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
        if size <= 0:
            print "closing connection"
            clientsocket.close()
            continue
            
        imgData = ''
        while len(imgData) < size:
            imgData += clientsocket.recv(size - len(imgData))
            print "received %s bytes" % len(imgData)
        
        # -- Write to a temporary file
        filename = "tmp.pgm"
        f = file(filename,'w')
        f.write(imgData)
        f.flush()
        f.close()
        
        # -- Call sift
        os.system('sift < %s | python process-sift.py > results.key' % filename)
        
        # -- Close the connection to complete the transaction
        print "sending response"
        fileData = file("results.key").read()
        clientsocket.send(packSize(len(fileData)))
        clientsocket.send(fileData)
        clientsocket.close()