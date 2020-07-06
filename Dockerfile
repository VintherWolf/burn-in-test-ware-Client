FROM ubuntu:18.04

# Install Dependencies and Update
RUN apt-get update && apt-get install -y \
    build-essential \
    tree

# Copy working dir to /burn-in-app
COPY . /burn-in-app

# Build app
RUN cd ./burn-in-app/client/ && make
RUN cd ./burn-in-app/server/ && make

RUN mkdir ./usr/logfiles

ENTRYPOINT ["/bin/bash", "-c"]


CMD ["./burn-in-app/getinfo.sh"]

# Start App
#CMD ["./burn-in-app/burn-in-test-ware-client-app"]
#CMD ["/bin/bash"]