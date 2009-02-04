
Take a look at the file for ubuntu, and get the packages installed. 
The following pages will help further:

www.boost.org
www.webtoolkit.eu/wt/wiki/index.php/Installing_Wt_2.1_on_MS_Windows
dtemplatelib.sourceforge.net  (build the release with multithreaded dll runtime which is not the default)

What you won't need that's listed in the file for ubuntu:
- mdbtools (used to import access files from the old Flugbuch. That's currently only supported in the linux version)
- everything related to fcgi (wt supports only the standalone server on windows)
- the separate boost binaries. (It's best if you download the whole boost library, and build it)
