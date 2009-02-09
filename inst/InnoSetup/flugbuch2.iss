[Files]
Source: ..\..\proj\msvc8\flugbuch2\Release\flugbuch2_wt.exe; DestDir: {app}
Source: ..\..\res\*.*; DestDir: {app}; Excludes: *\.svn\*; Flags: recursesubdirs
Source: ..\wt_config.xml; DestDir: {app}
[Icons]
Name: {group}\Webserver starten; Filename: {app}\flugbuch2_wt.exe; Parameters: "--docroot . --http-address 0.0.0.0 --http-port 8080 --config ""{app}\wt_config.xml"""; WorkingDir: {app}
Name: {group}\Web Client; Filename: http://localhost:8080
[Setup]
AppCopyright=Richard Ulrich
AppName=Flugbuch2
AppVerName=2.0.0.2
ShowLanguageDialog=no
DefaultDirName={pf}\flugbuch2
OutputBaseFilename=flugbuch2_inst
DefaultGroupName=flugbuch2
