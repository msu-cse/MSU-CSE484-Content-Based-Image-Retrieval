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

See the instructions in the [wiki](./wiki/).  Note that a complete, tested-with-CBIR version of FLANN is included in the FLANN directory.

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
        --build-clusters -c features/esp.feature.clusters.hdf5 \
        --build-cluster-index -i features/esp.feature.clusters.index.hdf5 \
        --bag-of-words=docs \
        --img-list=features/imglist.txt \
        --size-list=features/esp.size 

