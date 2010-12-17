from socket import *
import sys
import struct
import tempfile
import os
import logging
import settings
from settings import PGM_SERVER

# Caught exception: The index saved belongs to a different dataset

def packSize(size):
    return struct.pack('>L', size)

def unpackSize(sizeBytes):
    return struct.unpack('>L',sizeBytes)[0]

def recvExactly(socket, size, log = logging.getLogger('cbir')):
    '''
    Receives exactly 'size' bytes over 'socket'.  Returns the data, or None
    on error.

    @param log Optionally provide a custom logger, defaults to 'cbir'
    '''
    data = ''
    log.debug("Reading %i bytes" % size)
    while len(data) < size:
        recvd = socket.recv(size - len(data))
        if len(recvd) == 0:
            log.error("Connection dropped")
            return None
        data += recvd
        log.debug("received %s bytes" % len(data))
    return data

packedSize = struct.calcsize('>L')

def client(server,port,data):
    log = logging.getLogger('cbir.client')

    # -- Connect
    s = socket()
    log.info("connecting to %s:%s" % (server,port))
    s.connect((server, port))
    log.info("connected")

    # -- Get first four bytes for data size
    dataLen = len(data)
    dataLenBytes = packSize(dataLen)
    log.info("sending %s bytes of data" % dataLen)

    # -- Send first four bytes and the data
    s.send(dataLenBytes)
    s.send(data)

    # -- Receive response, first 4 bytes for size
    log.debug("waiting on response")
    responseSizePacked = recvExactly(s,packedSize,log)
    if responseSizePacked is None:
        return 0

    # -- Receive the data
    responseSize = unpackSize(responseSizePacked)
    response = recvExactly(s,responseSize,log)
    if response is None:
        return 0

    # Print out the info
    log.debug("Response:" + response)
    s.close()

    return response


if __name__ == '__main__':

    log = logging.getLogger('cbir.pgm-server')

    # Open the socket
    bindInfo = (PGM_SERVER['bind'], PGM_SERVER['port'])
    serversocket = socket(AF_INET, SOCK_STREAM)
    serversocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    serversocket.bind(bindInfo)
    serversocket.listen(100)

    # Accept incoming connections
    while 1:
        log.info("listening on %s:%s" % bindInfo)
        (clientsocket, address) = serversocket.accept()
        log.info( "accepted connection from %s:%s" % address )

        # -- Receive the data
        sizeBytes = recvExactly(clientSocket,packedSize,log)
        if sizeBytes is None: continue

        size = unpackSize(sizeBytes)
        if size <= 0:
            log.error("Invalid data size %s" % size)
            clientsocket.close()
            continue

        imgData = recvExactly(clientSocket,packedSize,log)
        if imgData is None: continue

        # -- Write to a temporary file
        filename = "tmp.pgm"
        f = file(filename,'w')
        f.write(imgData)
        f.flush()
        f.close()

        # -- Call sift
        os.system('%s < %s | python process-sift.py > results.key' % (
            settings.SIFT_PATH, filename))

        # -- Close the connection to complete the transaction
        log.info("sending response")
        fileData = file("results.key").read()
        clientsocket.send(packSize(len(fileData)))
        clientsocket.send(fileData)
        clientsocket.close()
