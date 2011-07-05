<VirtualHost *:80>	
	ServerName flugbuch2.ulrichard.ch
	DocumentRoot /usr/share/flugbuch2/www_docroot
	ServerAdmin richi@paraeasy.ch
	
	<Directory /usr/share/flugbuch2/www_docroot>
		Options FollowSymLinks MultiViews ExecCGI
		DirectoryIndex flugbuch2_wt.fcgi
		AllowOverride None
		Order allow,deny
		allow from all
	</Directory>

	ErrorLog /var/log/apache2/flugbuch2_error.log
    CustomLog /var/log/apache2/flugbuch2_access.log combined
</VirtualHost>
