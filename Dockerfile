FROM ubuntu:18.04
COPY . burn-in-app/
#ENTRYPOINT ["/bin/bash","-c"]
#CMD ["./burn-in-app/Debug/burn-in-test-ware-Client"]
CMD ["/bin/bash"]