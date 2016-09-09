#! /bash/sh

sudo docker build -t flugbuch2 .
sudo mkdir -p /usr/share/flugbuch2
sudo docker run -d -v /usr/share/flugbuch2:/usr/share/flugbuch2 -p 8083:80 flugbuch2 
