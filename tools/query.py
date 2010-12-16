'''
Performs a query on the provided image.
'''
import fileinput
import logging
from os import system
from os.path import exists,join

import lemur
import server
import settings
import sys

log = logging.getLogger('cbir.query')

if __name__ == '__main__':
    
    for filename in sys.argv[1:]:
        
        filename = filename.strip()
        
        if not exists(filename):
            log.error("File %s does not exist" % filename)
            continue
        
        # -- Get PGM keypoints
        log.info( "Processing %s" % filename)
        f = file(filename)
        keypoints = server.client(data=f.read(),
            server=settings.PGM_SERVER['hostname'],
            port=settings.PGM_SERVER['port'])
        f.close()
        
        if not keypoints:
            log.error("An error occurred while processing the PGM")
            continue
        
        # -- Get Clusters
        log.info("Getting words for %s" % filename)
        words = server.client(data=keypoints,
            server=settings.CBIR_SERVER['hostname'],
            port=settings.CBIR_SERVER['port'])
        if not words:
            log.error("An error occurred while getting words")
            continue
        
        # -- Perform query
        log.info("Performing LEMUR query")
        results = lemur.executeQuery(words)
        if not results:
            log.error("An error occured while querying LEMUR")
        
        # -- Display results
        for image in results:
            url = join(settings.IMAGE_PATH, image)
            print url
            system("%s %s" % (settings.BROWSER,url))