#!/bin/sh
# build a debian sourcepackage and upload it to the launchpad ppa
FLUGBUCHVERSIONSTR=2.0.15~natty
export GPGKEY=DA94BB53
export DEBEMAIL="richi@paraeasy.ch"
export DEBFULLNAME="Richard Ulrich"
killall -q gpg-agent
eval $(gpg-agent --daemon)
rm -r builddeb
mkdir builddeb
cd builddeb
svn export https://flugbuch2.svn.sourceforge.net/svnroot/flugbuch2/trunk flugbuch2-$FLUGBUCHVERSIONSTR
cd flugbuch2-$FLUGBUCHVERSIONSTR
dpkg-buildpackage -rfakeroot -S
cd ..
rm -r flugbuch2-$FLUGBUCHVERSIONSTR
dput ppa:richi-paraeasy/ppa ./flugbuch2_${FLUGBUCHVERSIONSTR}_source.changes
killall -q gpg-agent

