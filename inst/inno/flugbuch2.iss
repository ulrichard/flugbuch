[Files]
Source: C:\flugbuch2\proj\msvc8\flugbuch2\Release\flugbuch2_wt.exe; DestDir: {app}
Source: C:\flugbuch2\res\*.*; DestDir: {app}; Excludes: *\.svn\*; Flags: recursesubdirs
[Icons]
Name: {group}\Webserver starten; Filename: {app}\flugbuch2_wt.exe; Parameters: --docroot . --http-address 0.0.0.0 --http-port 8080; WorkingDir: {app}
Name: {group}\Web Client; Filename: http://localhost:8080
[Setup]
AppCopyright=Richard Ulrich
AppName=Flugbuch2
AppVerName=2.0.0.1
ShowLanguageDialog=no
DefaultDirName={pf}\flugbuch2
OutputBaseFilename=flugbuch2_inst
DefaultGroupName=flugbuch2
