FROM ubuntu:22.04
RUN mkdir /pipes
RUN apt-get update
RUN apt-get install gcc -y
RUN apt-get clean
ADD bin/docker_proc /bin/docker_proc
ENTRYPOINT ["docker_proc"]