rm -r builddeb
mkdir builddeb
cd builddeb
svn export https://flugbuch2.svn.sourceforge.net/svnroot/flugbuch2/trunk flugbuch2-2.0.9
cd flugbuch2-2.0.9/inst
dpkg-buildpackage -rfakeroot -S
cd ..
dput ppa:richi-paraeasy/ppa ./flugbuch2_2.0.9_source.changes
cd ..
rm -r flugbuch2-2.0.9

