rm -r builddeb
mkdir builddeb
cd builddeb
svn export https://flugbuch2.svn.sourceforge.net/svnroot/flugbuch2/trunk flugbuch2-2.0.13
cd flugbuch2-2.0.13
dpkg-buildpackage -rfakeroot -S
cd ..
rm -r flugbuch2-2.0.13
dput ppa:richi-paraeasy/ppa ./flugbuch2_2.0.13_source.changes

