FROM ubuntu:22.04
RUN mkdir /pipes
RUN mkdir /perf-stats
ADD bin/docker_proc /bin/docker_proc
RUN apt-get update
RUN apt-get install -y linux-tools-common linux-tools-aws linux-tools-`uname -r`
ADD src/lat_ctx_docker/run.sh /run.sh
RUN chmod +x run.sh
ENTRYPOINT ["./run.sh"]