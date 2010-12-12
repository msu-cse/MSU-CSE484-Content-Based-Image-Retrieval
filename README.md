# CBIR for C

## Log4C

Download [log4c][log4c]

    # Installation directory.
    # - CSE Systems -
    export PREFIX=$HOME
    # - A computer you own -
    export PREFIX=/usr/local
    
    tar xvzf log4c-1.2.1.tar.gz
    cd log4c-1.2.1
    ./configure --prefix=$PREFIX
    make
    make install

## Boost

Download [boost][boost]

    # Installation directory.
    # - CSE Systems -
    export PREFIX=$HOME
    # - A computer you own -
    export PREFIX=/usr/local
    
    tar xvzf boost_1_45_0.tar.gz
    cd boost_1_45_0
    ./bootstrap --prefix=$PREFIX
    ./bjam

[log4c]: http://sourceforge.net/projects/log4c/files/log4c/1.2.1/log4c-1.2.1.tar.gz/download "Logging for C"
[boost]: http://sourceforge.net/projects/boost/files/boost/1.45.0/boost_1_45_0.tar.gz/download "Boost Libraries"