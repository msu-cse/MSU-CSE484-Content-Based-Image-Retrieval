from ConfigParser import ConfigParser
import os
import sys


CBIR_PATH = os.path.dirname(os.path.abspath(os.path.join('..', __file__)))
SIFT_PATH = os.path.join(CBIR_PATH, 'sift')

# This is used to translate the path in this directory to an absolute path.
ROOT = os.path.dirname(os.path.abspath(__file__))
path = lambda *a: os.path.join(ROOT, *a)

config_fn = path('config.conf')
config = ConfigParser()
try:
    config.readfp(file(config_fn))
except IOError:
    sys.exit("Error: Cannot open configuration file '%s'" % config_fn)

SERVER_PORT = int(config.get('server', 'port'))
IMAGE_PATH = config.get('paths', 'images')
