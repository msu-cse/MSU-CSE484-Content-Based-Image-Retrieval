'''
Run Lemur against the index, parse the output, and return a list of image results
'''
from os import system

# -- Configuration --
pathToCBIR='/Users/zach/cbir-msu-cse484'
lemurConfig = {
    'bin': 'RetEval',
    'params': pathToCBIR + '/lemur-reteval-params',
    'index': pathToCBIR + '/index/docs.index.key',
    'query': pathToCBIR + '/lemur-query',
    'results': pathToCBIR + '/lemur-result',
    'model': "okapi",
    'numResults': 10,
    'trec': 1
}

def writeRetEvalParams():
    f = file(lemurConfig['params'],'w')
    f.write("""
<parameters>
    <index>%(index)s</index>
    <retModel>%(model)s</retModel>
    <textQuery>%(query)s</textQuery>
    <resultFile>%(results)s</resultFile>
    <TRECResultFormat>%(trec)s</TRECResultFormat>
    <resultCount>%(numResults)s</resultCount>
</parameters>
""" % lemurConfig)
    f.flush()
    f.close()

def executeRetEval():
    system('%(bin)s %(params)s' % lemurConfig)

def parseResults():
    # Example data
    # 1 Q0 0e53738de9069d42350ef7fd9bdc3d20.jpg 1 -6.41985 Exp
    # 1 Q0 16fde68a4c61ca952fd9b2b0477cd8c8.jpg 2 -6.43414 Exp
    # 1 Q0 0f8e9ad4e09cc89859d38bc85a17ddbf.jpg 3 -6.43447 Exp
    # 1 Q0 0e056969903f02a076fbbcd8197260b0.jpg 4 -6.43574 Exp
    
    # Want just the third field
    f = file(lemurConfig['results'],'r')
    results = []
    for line in f.readlines():
        results.append( line.split()[2] )
    return results
    


def writeQuery(data):
    f = file(lemurConfig['query'],'w')
    f.write(data)
    f.flush()
    f.close()


def executeQuery(data):
    writeQuery(data)
    writeRetEvalParams()
    executeRetEval()
    return parseResults()

if __name__ == '__main__':
    results = executeQuery("""
    <DOC 1>
    1
    </DOC>
    """)
    
    print results
    
    for i in results:
        system("open http://s3.amazonaws.com/cse484-images/%s" % i)