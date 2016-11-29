FROM ubuntu:trusty
MAINTAINER Richard Ulrich   "richi@ulrichard.ch"

# install instructions for sky can be found here: https://tel.red/repos.htm

# Tell debconf to run in non-interactive mode
ENV DEBIAN_FRONTEND noninteractive

# Make sure the repository information is up to date
RUN apt-get update

# Enable Universe and Multiverse and install dependencies.
#RUN echo deb http://archive.ubuntu.com/ubuntu trusty universe multiverse >> /etc/apt/sources.list

# Ensure we can add personal package archves
RUN apt-get install -y software-properties-common openssh-server

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

# Create user "docker" and set the password to "docker"
RUN useradd -m -d /home/docker docker
RUN echo "docker:docker" | chpasswd

# Create OpenSSH privilege separation directory, enable X11Forwarding
RUN mkdir -p /var/run/sshd
RUN echo X11Forwarding yes >> /etc/ssh/ssh_config

# Prepare ssh config folder so we can upload SSH public key later
RUN mkdir /home/docker/.ssh
RUN chown -R docker:docker /home/docker
RUN chown -R docker:docker /home/docker/.ssh

# Replace 1000 with your user / group id
RUN export uid=1000 gid=1000 && \
    echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker && \
    chmod 0440 /etc/sudoers.d/docker
#USER docker
#ENV HOME /home/docker

# Expose the HTTP, HTTPS and SSH ports
EXPOSE 80
EXPOSE 443
EXPOSE 22

# Start apache
#ENTRYPOINT ["/sbin/init"]
ENTRYPOINT ["/usr/sbin/sshd",  "-D"]

