rm -r builddeb
mkdir builddeb
cd builddeb
svn export https://flugbuch2.svn.sourceforge.net/svnroot/flugbuch2/trunk flugbuch2-2.0.15
cd flugbuch2-2.0.15
dpkg-buildpackage -rfakeroot -S
cd ..
rm -r flugbuch2-2.0.15
dput ppa:richi-paraeasy/ppa ./flugbuch2_2.0.15~natty_source.changes

