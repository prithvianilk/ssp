FROM debian:latest

RUN apt-get update
RUN apt-get install -y gcc g++ openjdk-11-jdk
RUN apt-get clean

RUN mkdir /pipes
ADD bin/docker_proc /bin/docker_proc
ENTRYPOINT ["docker_proc"]