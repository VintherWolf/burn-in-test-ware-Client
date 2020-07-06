#!/bin/bash


docker container run -p 1956:1956 -v C:/docker_log:/usr/logfiles -ti --rm burn-in-test-app:lts /bin/bash

#docker exec -ti client_burn-in-app /bin/bash

