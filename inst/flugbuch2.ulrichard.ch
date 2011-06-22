<VirtualHost *:80>	
	ServerName flugbuch2.ulrichard.ch
	DocumentRoot /usr/share/flugbuch2/www_docroot
	ServerAdmin richi@paraeasy.ch
	
	<IfModule mod_fastcgi.c>
		FastCgiServer /usr/share/flugbuch2/www_docroot/flugbuch2_wt.fcgi -initial-env WT_APP_ROOT=/etc/flugbuch2
	</IfModule>

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
