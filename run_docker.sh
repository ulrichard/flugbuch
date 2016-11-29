#! /bin/sh

if [ ! -d /usr/share/flugbuch2 ]; then
    sudo mkdir -p /usr/share/flugbuch2
fi
docker build -t flugbuch2 .
docker run -d -v /usr/share/flugbuch2:/usr/share/flugbuch2 -p 8083:80 -p 2283:22 flugbuch2 
sleep 2
ssh -p 2283 docker@localhost
