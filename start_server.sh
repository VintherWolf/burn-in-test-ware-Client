#!/bin/bash

docker container run -p 1955:1955 -p 2222:22 -v C:/burn-in-testlogs:/usr/logfiles -ti --rm burn-in-test-app:lts /bin/bash

#docker exec -ti server_burn-in-app /bin/bash

