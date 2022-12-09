# Makefile for top level of lmbench
# $Id: Makefile 1.17 00/05/31 16:16:15+03:00 staelin@hpli8.hpli.hpl.hp.com $

# Possible things to $(MAKE):
#
# build		(default) go to the source directory and build the benchmark
# results	go to the source directory and build and run the benchmark
# rerun		run the benchmark again
# see		see the results that came with this release
#		Go to the results directory and read the Makefile.
# doc.lpr	print the documentation
# doc.x		preview the documentation (needs X, groff, pic, etc)
# clean		go to the subdirs and $(MAKE) clean
# get		$(MAKE) sure all files are checked out
# shar		build a shippable shar archive

SHELL=/bin/sh

build: 
	cd src && $(MAKE)

lat_ctx_docker:
	gcc -o bin/docker_proc src/lat_ctx_docker/docker_proc.c
	sudo docker build -t docker_proc -f ./src/lat_ctx_docker/Dockerfile .

	sudo docker build -t docker_proc_sm -f ./src/lat_ctx_docker/dockerfiles/sizes/Dockerfile.sm .
	sudo docker build -t docker_proc_m -f ./src/lat_ctx_docker/dockerfiles/sizes/Dockerfile.m .
	sudo docker build -t docker_proc_l -f ./src/lat_ctx_docker/dockerfiles/sizes/Dockerfile.l .
	sudo docker build -t docker_proc_xl -f ./src/lat_ctx_docker/dockerfiles/sizes/Dockerfile.xl .
	sudo docker build -t docker_proc_xxl -f ./src/lat_ctx_docker/dockerfiles/sizes/Dockerfile.xxl .

	sudo docker build -t docker_proc_l1 -f ./src/lat_ctx_docker/dockerfiles/layers/Dockerfile.1 .
	sudo docker build -t docker_proc_l2 -f ./src/lat_ctx_docker/dockerfiles/layers/Dockerfile.2 .
	sudo docker build -t docker_proc_l3 -f ./src/lat_ctx_docker/dockerfiles/layers/Dockerfile.3 .
	sudo docker build -t docker_proc_l4 -f ./src/lat_ctx_docker/dockerfiles/layers/Dockerfile.4 .
	sudo docker build -t docker_proc_l5 -f ./src/lat_ctx_docker/dockerfiles/layers/Dockerfile.5 .

results: FRC
	cd src && $(MAKE) results

rerun: 
	cd src && $(MAKE) rerun

see:
	cd results && $(MAKE) summary percent 2>/dev/null | more

doc.lpr:
	cd doc && $(MAKE) PS && lpr *.PS

doc.x:
	cd doc && $(MAKE) x

clobber clean: 
	for i in doc src results scripts; do \
		echo ===== $$i =====; \
		(cd $$i && $(MAKE) clean); \
	done
	/bin/rm -rf bin/*
	-bk clean 

get: 
	for i in doc src results scripts; do \
		echo ===== $$i =====; \
		(cd $$i && bk get -q); \
	done
	@co -q

info: 
	for i in doc src results scripts; do \
		echo ===== $$i =====; \
		(cd $$i && info); \
	done

release: scripts/mkrelease
	scripts/mkrelease

scripts/mkrelease:
	cd scripts && co mkrelease

# XXX - . must be named lmbench for this to work
shar:
	$(MAKE) clean
	co -q Makefile
	$(MAKE) get
	cd .. && \
	find lmbench -type f -print  | egrep -v 'noship|RCS' > /tmp/FILES
	cd .. && shar -S -a -n lmbench1.0 -L 50K < /tmp/FILES 

FRC:
