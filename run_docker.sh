#! /bin/sh

if [ ! -d $HOME/flugbuch2 ]; then
    mkdir -p $HOME/flugbuch2
fi
docker build -t flugbuch2 .
firefox http://localhost:8383
docker run -ti --rm \
    -v $HOME/flugbuch2:/root/flugbuch2 \
    -p 8383:80 \
    flugbuch2 
