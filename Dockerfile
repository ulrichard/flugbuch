FROM ubuntu:trusty
MAINTAINER Richard Ulrich   "richi@ulrichard.ch"

# install instructions for sky can be found here: https://tel.red/repos.htm

# Tell debconf to run in non-interactive mode
ENV DEBIAN_FRONTEND noninteractive

# Make sure the repository information is up to date
RUN apt-get update

# Enable Universe and Multiverse and install dependencies.
RUN echo deb http://archive.ubuntu.com/ubuntu trusty universe multiverse >> /etc/apt/sources.list

# Ensure we can add personal package archves
RUN apt-get install -y software-properties-common

# add the ppas
RUN apt-add-repository -y ppa:pgquiles/wt
RUN apt-add-repository -y ppa:richi-paraeasy/ppa

# Make sure the repository information is up to date
RUN apt-get update

# install flugbuch
RUN apt-get install -y flugbuch2

# Set locale (fix locale warnings)
RUN localedef -v -c -i en_US -f UTF-8 en_US.UTF-8 || true
RUN echo "Europe/Berlin" > /etc/timezone

# Expose the HTTP and HTTPS ports
EXPOSE 80
EXPOSE 443

# Start apache
ENTRYPOINT ["/sbin/init"]
