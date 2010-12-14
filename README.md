# CBIR for C

## Prerequisites

- homebrew
- XCode
- libhdf5
- boost
- log4cxx
- FLANN

### Homebrew

Homebrew allows easy installation of packages from source onto Mac OS X.  Any package that can be installed with Homebrew can easily be installed with the usual `./configure && make && make install` or via a repository such as `apt-get` or `yum`.

    $ ruby -e "$(curl -fsSL https://gist.github.com/raw/323731/install_homebrew.rb)"
    
### libhdf5

    $ sudo brew install hdf5

### boost

    $ sudo brew install boost

### log4cxx

    $ sudo brew install log4cxx

### FLANN

See the instructions in the wiki.  Note that a complete, tested-with-CBIR version of FLANN is included in the FLANN directory.

## Building CBIR

Just type

    $ `make`

## Using CBIR

To see a list of options,

    $ ./cbir --help

Here's the two general use-cases:

#### Convert the input `esp.feature` file to HDF5

HDF5 is a binary format, which is much faster to load than the plaintext space-separated format.  If you will be loading a feature file frequently, it is recommended that you convert it to HDF5.

    $ ./cbir --feature-file=esp.feature --convert-to-hdf5

#### Everything Else

This command will build the clusters, save them to a file (HDF5 format), index the clusters, save the index to file (FLANN format, which is HDF5), and perform the nearest-neighbor search to create the bag-of-words representation for each image file in the provided directory (in this case, `docs`).

    $ mkdir docs
    $ ./cbir --feature=features/esp.feature.hdf5 \
        --build-clusters \
        --bag-of-words=docs

>> ( Expected runtime: 20 minutes total )

## Integrating with Lemur

If you've already built Lemur (see the wiki for instructions) you should be ready to build the index and query!

### Building the Index

Launch Lemur:

    java -cp share/lemur/lemur.jar -Djava.library.path=lib -jar share/lemur/LemurIndex.jar &

- Set the Index Type to **KeyfileIncIndex**
- Set the index name to a memorable path, for example `/path/to/cbir/doc-index`
- Add add the directory containing our "documents" (bag-of-words files) to the list of **Files/Directories to Index**
- Click **Build Index**

### Querying

Create the following files.  This is very similar to the earlier homework from CSE 484.  Feel free to change these parameters to suit your needs.

#### retEval.params

    <parameters>
    <index>/path/to/cbir/doc-index.key</index>
    <retModel>okapi</retModel>
    <textQuery>query.txt</textQuery>
    <resultFile>result.txt</resultFile>
    <TRECResultFormat>1</TRECResultFormat>
    <resultCount>20</resultCount>
    </parameters>
    
#### query.txt

    <DOC 1>
    # Contents of the bag-of-words representation of your query image goes here
    # For testing purposes, it can be useful to enter a single number.
    # This will get you all of the images that have a given 'word', that is, have a feature that belongs to the specified cluster.
    </DOC>

Note that we're skipping the `parse_query.params` and `ParseToText` steps and directly creating a query. Now that you've got your files set up and ready to go, execute the query.

    bin/RetEval retEval.params query.txt

Open up the newly-created `result.txt` and view your results.

# Running the Bag-of-Words Server

Once the clusters are built from the dataset, you can use the server to parse any set of features.

    ./cbir ./cbir --server=8080 -c features/esp.feature.hdf5.clusters.hdf5
    
This will start the server listening on Port 8080.  It will listen for 4 bytes to provide the size of the feature data (in network-byte-order) and then read that many bytes.  That information will then be parsed as a 'feature' file in text mode. 

After parsing, the nearest_neighbors operation will be executed, and the server will return data that can be directly interpreted by Lemur as a query:

***Example***

    <DOC 1>
    1
    1231
    4342
    </DOC>

The easiest way to interact with the server is the Python script `tools/pgmserver.py`.

    python
    from pgmserver import *
    oneSingleFeature="0 0 0 13 125 14 0 0 23 19 3 17 98 9 1 18 86 29 2 0 0 0 2 74 125 16 0 0 0 0 1 61 0 0 0 5 125 22 0 0 39 22 2 5 119 43 14 12 125 35 0 0 0 1 7 80 125 7 0 0 0 0 3 86 0 0 0 5 104 49 4 0 10 2 0 0 41 114 97 17 125 45 0 0 0 10 45 62 125 66 0 0 0 0 2 41 0 0 0 0 12 31 11 2 0 0 0 0 4 36 95 9 10 6 0 0 0 14 83 65 40 6 0 0 0 0 9 101"
    query = client('localhost',8080,oneSingleFeature)
    print query