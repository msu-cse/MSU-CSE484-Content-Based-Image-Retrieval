# CBIR for C

# Prerequisites

- gcc
- libhdf5
- boost
- log4cxx
- FLANN

## Homebrew

Homebrew allows easy installation of packages from source onto Mac OS X.  Any package that can be installed with Homebrew can easily be installed with the usual `./configure && make && make install` or via a repository such as `apt-get` or `yum`.

    $ ruby -e "$(curl -fsSL https://gist.github.com/raw/323731/install_homebrew.rb)"
    
### libhdf5

    $ sudo brew install hdf5

### boost

    $ sudo brew install boost

### log4cxx

    $ sudo brew install log4cxx

## FLANN

See the instructions in the wiki.  Note that a complete, tested-with-CBIR version of FLANN is included in the FLANN directory.

# Building CBIR

Just type

    $ `make`

If you see errors after compilation has finished, related to not being able to find a particular library or a linking problem, please see the prerequisites.

# Using CBIR

To see a list of options,

    $ ./cbir --help

## Convert the input `esp.feature` file to HDF5

HDF5 is a binary format, which is much faster to load than the plaintext space-separated format.  If you will be loading a feature file frequently, it is recommended that you convert it to HDF5.

    $ ./cbir --feature=esp.feature --convert-to-hdf5

## Clustering and Bag-of-Words

This command will build the clusters, save them to a file (HDF5 format), index the clusters, save the index to file (FLANN format, which is HDF5), and perform the nearest-neighbor search to create the bag-of-words representation for each image file in the provided directory (in this case, `docs`).

    $ mkdir docs
    $ ./cbir --feature=features/esp.feature.hdf5 \
        --build-clusters \
        --bag-of-words=docs

>> ( Expected runtime: 20 minutes total )

## CBIR Server

The CBIR server listens for incoming connections that provide feature data in the same format as the data provided to the class (space-separated integers, 128 values per line).

After receiving this data, it determines which clusters each point belongs to, which is that point's word representation.  These clusters are returned to the connected client in the form of a Lemur query script.

# Integrating with Lemur

If you've already built Lemur (see the wiki for instructions) you should be ready to build the index and query!

## Building the Index

Launch Lemur:

    java -cp share/lemur/lemur.jar -Djava.library.path=lib -jar share/lemur/LemurIndex.jar

- Set the Index Type to **KeyfileIncIndex**
- Set the index name to a memorable path, for example `/path/to/cbir/index/docs.index` (which is the default location)
- Add add the directory containing our "documents" (bag-of-words files) to the list of **Files/Directories to Index**.
- Click **Build Index**

# Querying

Querying is simple, and requires only a few steps.  There are a few different pieces that need to be started.

## Configuration

Make sure that all of the values in `tools/config.conf` are as they should be before continuing.

## PGM Server

The PGM server processes PGM images, runs them through `sift`, and returns the results in a format CBIR understands.

    python tools/server.py

Will automatically start the server on port 8081.  If you need a different port, see `tools/config.conf`

#### Why have a PGM server? Why not just call Sift directly?

Sift is only distributed in binary format, for Linux and Windows.  We run the PGM server on an Amazon EC2 instance.

## CBIR Server

The CBIR server takes feature data, and returns the bag-of-words representation of that data.

    ./cbir --server=8080 --cluster=features/esp.feature.hdf5.clusters.hdf5

## Run the Query

The Python script `tools/query.py` takes a list of filenames as arguments, retrieves the feature data from the PGM server, passes them to CBIR for bag-of-words processing, and queries Lemur to get the results.  The image files are then opened in your browser.

    python tools/query.py test-queries/query1.pgm

Note that `tools/config.conf` must be updated to show the host and port that the PGM and CBIR servers are running on, or the query will fail.  Additionally, you must have already built your clusters and bag-of-words representation (see above) and indexed it with Lemur (also see above).