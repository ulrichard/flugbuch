dpkg-buildpackage -rfakeroot


rm -r builddeb
mkdir builddeb
cd builddeb
svn export https://flugbuch2.svn.sourceforge.net/svnroot/flugbuch2/trunk flugbuch2-2.0.8
cd flugbuch2-2.0.8
dpkg-buildpackage -rfakeroot -S
cd ..
rm -r flugbuch2-2.0.8
dput ppa:richi-paraeasy/ppa ./flugbuch2_2.0.8_source.changes
