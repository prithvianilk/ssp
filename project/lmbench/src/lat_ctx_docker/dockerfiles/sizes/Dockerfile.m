<<<<<<< HEAD
FROM debian:latest

RUN apt-get update
RUN apt-get install -y gcc g++ openjdk-11-jdk
RUN apt-get clean

=======
FROM ubuntu:22.04
>>>>>>> 4baf74dad59284450bd70105ece07015563f3992
RUN mkdir /pipes
ADD bin/docker_proc /bin/docker_proc
ENTRYPOINT ["docker_proc"]