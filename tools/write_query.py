'''
Writes a query file in TREC format, given a list of 'words' from stdin or from the command-line.  The first argument is the input file, the second argument is the output file.  If they are not specified, stdin/stdout are used.

In either case, exactly one line is processed.

Writes a query in TREC web format.
'''

import sys
data = ''

if len(sys.argv) > 1:
    f = file(sys.argv[1])
    data = f.readline()
    f.close()
else:
    stdin = sys.stdin
    data = stdin.readline()

data = """<DOC 1>
%s
</DOC>
""" % '\n'.join(data.split())

if len(sys.argv) > 2:
    f = file(sys.argv[2],'w')
    f.write(data)
    f.close()
else:
    print data
