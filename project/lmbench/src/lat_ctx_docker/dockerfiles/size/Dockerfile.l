<<<<<<< HEAD
FROM ubuntu:22.04
=======
FROM debian:latest
>>>>>>> 4baf74dad59284450bd70105ece07015563f3992
RUN mkdir /pipes
ADD bin/docker_proc /bin/docker_proc
ENTRYPOINT ["docker_proc"]