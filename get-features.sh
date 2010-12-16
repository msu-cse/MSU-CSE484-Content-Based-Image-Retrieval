#!/bin/sh

# Download
wget http://www.cse.msu.edu/~tongwei/cse484project.tar.gz

# Extract
tar xvzf cse484project.tar.gz cse484project/features/

# Move
mkdir features
mv cse484project/features/* features

# Remove directories. Don't rm -rf in case there's something else there
rmdir cse484project/features
rmdir cse484project

rm cse484project.tar.gz