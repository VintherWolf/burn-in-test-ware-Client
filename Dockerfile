FROM ubuntu:18.04
EXPOSE 1955 1956
EXPOSE 22

# Install Dependencies and Update
RUN apt-get update && apt-get install -y \
    build-essential \
    tree \
    openssh-server

# Copy working dir to /burn-in-app
COPY . /burn-in-app
WORKDIR /burn-in-app

# Build app
RUN cd ./client/ && make
RUN cd ./server/ && make

# Make shared folder
RUN cd .. && mkdir /usr/logfiles

# Setup SSH
RUN groupadd sshgroup && useradd -ms /bin/bash -g sshgroup remoteuser
RUN service ssh start

# Set process to bash and output tree to 
# /usr/logfiles/tree (i.e. burn-in-testlogs/tree)
ENTRYPOINT ["/bin/bash", "-c"]
CMD ["./getinfo.sh"]