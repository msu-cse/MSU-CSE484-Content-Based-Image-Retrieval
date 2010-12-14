import sys

if len(sys.argv) < 2: 
    print "Must provide a keyfile argument"
    sys.exit(0)

# == Exampple Keydata
# 176 128
# 5.53 69.38 1.10 -2.567
#  0 0 6 128 28 0 0 1 58 0 0 9 4 0 0 113 105 0 0 0
#  0 0 0 128 10 1 0 0 0 1 0 24 9 0 3 16 1 76 43 7
#  128 12 0 0 0 6 5 128 124 12 0 0 0 0 0 87 1 0 1 1
#  0 1 0 1 4 0 0 0 0 128 105 2 128 92 0 0 0 125 76 16
#  80 128 1 0 0 5 6 8 7 9 1 0 0 0 0 3 0 0 0 0
#  0 45 17 0 4 10 0 0 0 128 81 4 50 44 0 0 0 29 60 56
#  16 4 0 0 0 0 0 12

# -- Skip the first line explaining the dimensions
# Also, skip the floating-point stuffs
f = file(sys.argv[1])
lines = f.readlines()
for i in xrange(2,len(lines),8):
    print ' '.join([line.strip() for line in lines[i:i+7]])