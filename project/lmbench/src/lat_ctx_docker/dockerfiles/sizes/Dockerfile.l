FROM debian:latest
RUN mkdir /pipes
ADD bin/docker_proc /bin/docker_proc
ENTRYPOINT ["docker_proc"]