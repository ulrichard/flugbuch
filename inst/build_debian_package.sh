#!/bin/sh
# build a debian sourcepackage and upload it to the launchpad ppa
export GPGKEY=DA94BB53
export DEBEMAIL="richi@paraeasy.ch"
export DEBFULLNAME="Richard Ulrich"
killall -q gpg-agent
eval $(gpg-agent --daemon)
rm -r builddeb
mkdir builddeb
cd builddeb
for DISTRIBUTION in maverick natty oneiric
do
	FLUGBUCHVERSIONSTR=2.0.19~${DISTRIBUTION}
	svn export https://flugbuch2.svn.sourceforge.net/svnroot/flugbuch2/trunk flugbuch2-$FLUGBUCHVERSIONSTR
	cd flugbuch2-$FLUGBUCHVERSIONSTR
		sed -i  -e "s/jaunty/${DISTRIBUTION}/g" -e "s/karmic/${DISTRIBUTION}/g" -e "s/maverick/${DISTRIBUTION}/g" -e "s/natty/${DISTRIBUTION}/g" -e "s/oneiric/${DISTRIBUTION}/g" debian/changelog

	dpkg-buildpackage -rfakeroot -S
	cd ..
	rm -r flugbuch2-$FLUGBUCHVERSIONSTR
	dput ppa:richi-paraeasy/ppa ./flugbuch2_${FLUGBUCHVERSIONSTR}_source.changes
done				
killall -q gpg-agent

