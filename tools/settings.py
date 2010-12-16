from ConfigParser import ConfigParser
from os.path import dirname, abspath, join
import logging.config
import sys

# Root path to CBIR
CBIR_PATH = abspath(join(dirname(__file__),'..'))

# Path to the tools directory
TOOLS_PATH = join(CBIR_PATH,'tools')

# This is used to translate the path in this directory to an absolute path.
path = lambda *a: join(TOOLS_PATH, *a)

config_fn = path('config.conf')
config = ConfigParser()
try:
    config.readfp(file(config_fn))
except IOError:
    sys.exit("Error: Cannot open configuration file '%s'" % config_fn)

PGM_SERVER = {
    'bind': config.get('server.pgm','bind'),
    'port': int(config.get('server.pgm', 'port')),
    'hostname': config.get('server.pgm', 'hostname')
}

CBIR_SERVER = {
    'port': int(config.get('server.cbir', 'port')),
    'hostname': config.get('server.cbir', 'hostname')
}

IMAGE_PATH = config.get('paths', 'images')
BROWSER = config.get('paths','browser')
SIFT_PATH = join(CBIR_PATH, config.get('paths','sift'))

LEMUR_CONFIG = {
    'bin': config.get('lemur', 'retEvalBin'),
    'params': join(CBIR_PATH, config.get('lemur', 'params')),
    'index': join(CBIR_PATH, config.get('lemur', 'index')),
    'query': join(CBIR_PATH, config.get('lemur', 'query')),
    'results': join(CBIR_PATH, config.get('lemur', 'results')),
    'model': config.get('lemur','model'),
    'numResults': int(config.get('lemur','numResults')),
    'trec': int(config.get('lemur','trec'))
}

LOG_CONFIG = path('logging.conf')
logging.config.fileConfig(LOG_CONFIG)


if __name__ == '__main__':
    import pprint
    pp = pprint.PrettyPrinter()
    
    print "== Configuration =="
    
    settings = [
        'CBIR_SERVER',
        'PGM_SERVER',
        'IMAGE_PATH',
        'SIFT_PATH',
        'LEMUR_CONFIG',
        'ROOT',
        'CBIR_PATH'
    ]
    
    for k,v in globals().items():
        if k in settings:
            print "%s -> " % k,
            pp.pprint(v)