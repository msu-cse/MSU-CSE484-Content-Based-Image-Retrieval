"""
Browse search results by cluster.

Usage:
    python tools/browse_cluster.py 1 2 3 4

Displays images from clusters 1,2,3,4 in the browser (on Mac OS X, or any system that supports the 'open' command).

"""

from lemur import *

def browseCluster(which):
    results = executeQuery("""
    <DOC 1>
    %s
    </DOC>
    """ % which)
    
    print results
    
    for i in results:
        print "http://s3.amazonaws.com/cse484-images/%s" % i
        system("open http://s3.amazonaws.com/cse484-images/%s" % i)


if __name__ == "__main__":
    import sys
    
    if len(sys.argv) == 1:
        browseCluster(0)
    
    else:
        for i in sys.argv[1:]:
            browseCluster(i)