FROM ubuntu:trusty
MAINTAINER Richard Ulrich   "richi@ulrichard.ch"

# Tell debconf to run in non-interactive mode
ENV DEBIAN_FRONTEND noninteractive

# To be able to install libapache.
RUN echo 'deb http://archive.ubuntu.com/ubuntu trusty multiverse' >> /etc/apt/sources.list && \
    echo 'deb http://archive.ubuntu.com/ubuntu trusty-updates multiverse' >> /etc/apt/sources.list && \
    echo 'deb http://security.ubuntu.com/ubuntu trusty-security multiverse' >> /etc/apt/sources.list

# Ensure we can add personal package archves
RUN apt-get update && apt-get -y upgrade && apt-get install -y software-properties-common 

# add the personal package archives
RUN apt-add-repository -y ppa:pgquiles/wt && \
    apt-add-repository -y ppa:richi-paraeasy/ppa

# install flugbuch
RUN apt-get update && apt-get install -y flugbuch2

# Set locale (fix locale warnings)
RUN localedef -v -c -i en_US -f UTF-8 en_US.UTF-8 || true
RUN echo "Europe/Berlin" > /etc/timezone

# Expose the HTTP port
EXPOSE 80

# Start the built in webserver
CMD ["/usr/share/flugbuch2/www_docroot/flugbuch2_wt_http", "--docroot", "/usr/share/flugbuch2/www_docroot", "--http-address", "0.0.0.0"]

