char	*id = "$Id$\n";

#include "bench.h"
#include "lat_ctx_docker/container.h"
#include <errno.h>


#define	MAXPROC	2048
#define	CHUNK	(4<<10)
#define	TRIPS	5
#ifndef	max
#define	max(a, b)	((a) > (b) ? (a) : (b))
#endif

void	initialize_overhead(iter_t iterations, void* cookie);
void	cleanup_overhead(iter_t iterations, void* cookie);
void	benchmark_overhead(iter_t iterations, void* cookie);

void initialize(iter_t iterations, void* cookie);
void benchmark(iter_t iterations, void* cookie);
void cleanup(iter_t iterations, void* cookie);

int create_pipes(int procs);
void read_token(int read_pipe_fd, char* token);
void write_token(int write_pipe_fd, char* token);

struct _state {
	int	process_size;
	double	overhead;
	int	procs;
	pid_t*	pids;
	int	**p;
	void*	data;
};

int main(int ac, char **av)
{
	int	i, maxprocs;
	int	c;
	int	parallel = 1;
	int	warmup = 0;
	int	repetitions = TRIES;
	struct _state state;
	char *usage = "[-P <parallelism>] [-W <warmup>] [-N <repetitions>] [-s kbytes] processes [processes ...]\n";
	double	time;

	/*
	 * Need 4 byte ints.
	 */
	if (sizeof(int) != 4) {
		fprintf(stderr, "Fix sumit() in ctx.c.\n");
		exit(1);
	}

	state.process_size = 0;
	state.overhead = 0.0;
	state.pids = NULL;

	/*
	 * If they specified a context size, or parallelism level, get them.
	 */
	while (( c = getopt(ac, av, "s:P:W:N:")) != EOF) {
		switch(c) {
		case 'P':
			parallel = atoi(optarg);
			if (parallel <= 0) lmbench_usage(ac, av, usage);
			break;
		case 'W':
			warmup = atoi(optarg);
			break;
		case 'N':
			repetitions = atoi(optarg);
			break;
		case 's':
			state.process_size = atoi(optarg) * 1024;
			break;
		default:
			lmbench_usage(ac, av, usage);
			break;
		}
	}

	if (optind > ac - 1)
		lmbench_usage(ac, av, usage);

	// /* compute pipe + sumit overhead */
	maxprocs = atoi(av[optind]);
	for (i = optind; i < ac; ++i) {
		state.procs = atoi(av[i]);
		if (state.procs > maxprocs)
			maxprocs = state.procs;
	}
	state.procs = maxprocs;

	create_pipes(state.procs);

	benchmp(initialize_overhead, benchmark_overhead, cleanup_overhead, 0, 1, warmup, repetitions, &state);
	if (gettime() == 0) return(0);
	state.overhead = gettime();
	state.overhead /= get_n();
	fprintf(stderr, "\n\"size=%dk ovr=%.2f\n", state.process_size/1024, state.overhead);

	delete_pipes();
	sleep(2);

	fprintf(stderr, "Number Of Containers, Context Switch Time\n");
	/* compute the context switch cost for N containers */
	for (i = optind; i < ac; ++i) {
		state.procs = atoi(av[i]);

		create_pipes(state.procs);
		run_containers(state.procs - 1, state.process_size);

		benchmp(initialize, benchmark, cleanup, 0, parallel, warmup, repetitions, &state);

		time = gettime();
		time /= get_n();
		time /= state.procs;
		time -= state.overhead;

		if (time > 0.0)
			fprintf(stderr, "%d,%.2f\n", state.procs, time);

		kill_containers(state.procs - 1);
		delete_pipes();

		sleep(2);
	}

	// kill_containers(state.procs - 1);
	delete_pipes();

	return (0);
}

void initialize_overhead(iter_t iterations, void* cookie)
{
	// fprintf(stderr, "in initialise_overhead\n");
	int procs;
	struct _state* pState = (struct _state*)cookie;

	if (iterations) return;

	pState->pids = NULL;

	pState->data = (pState->process_size > 0) ? malloc(pState->process_size) : NULL;
	if (pState->data)
		bzero(pState->data, pState->process_size);

	// procs = create_pipes(pState->procs);
	// if (procs < pState->procs) {
	// 	cleanup_overhead(0, cookie);
	// 	exit(1);
	// }
}

void benchmark_overhead(iter_t iterations, void* cookie)
{
	// fprintf(stderr, "in benchmark overhead\n");
	struct _state* pState = (struct _state*)cookie;
	int	i = 0;
	char* msg = (char*)malloc(sizeof(char)*2);
	msg = "a";

	char *pipe_path = (char*)malloc(sizeof(char)*10);

	// fprintf(stderr, "iterations %d\n", iterations);

	sprintf(pipe_path, "pipes/%d", i);
	int pipe_fd = open(pipe_path, O_RDWR);

	while (iterations-- > 0) {
		// fprintf(stderr, "i=%d\n", i);

		write_token(pipe_fd, msg);
		// fprintf(stderr, "completed write\n", i);

		char msg_read[2];
		read_token(pipe_fd, msg_read);
		// fprintf(stderr, "completed read\n", i);

		if (++i == pState->procs) {
			i = 0;
		}
		bread(pState->data, pState->process_size);
	}
	close(pipe_fd);
}

void cleanup_overhead(iter_t iterations, void* cookie)
{
	// fprintf(stderr, "in cleanup overhead\n");
	int i;
	struct _state* pState = (struct _state*)cookie;

	if (iterations) return;

	if (pState->data) free(pState->data);

	// delete_pipes();
}

void initialize(iter_t iterations, void* cookie)
{
	// fprintf(stderr, "in initialise\n");
	struct _state* pState = (struct _state*)cookie;

	if (iterations) return;

	initialize_overhead(iterations, cookie);

	// run_containers(pState->procs - 1, pState->process_size);
}

void benchmark(iter_t iterations, void* cookie)
{
	// fprintf(stderr, "in benchmark\n");
	struct _state* pState = (struct _state*)cookie;

	char* msg = (char*)malloc(sizeof(char)*2);
	msg = "a";

	char *write_pipe_path = "pipes/0";

	char *read_pipe_path = (char*)malloc(sizeof(char)*10);
	sprintf(read_pipe_path, "pipes/%d", pState->procs - 1);

	int write_pipe_fd = open(write_pipe_path, O_WRONLY);
	int read_pipe_fd = open(read_pipe_path, O_RDONLY);
	while (iterations-- > 0) {
		// fprintf(stderr, "writing %s in iter %d\n", msg, iterations);
		write_token(write_pipe_fd, msg);

		char msg_read[2];
		read_token(read_pipe_fd, msg_read);
		// fprintf(stderr, "read %s in iter %d\n", msg_read, iterations);

		bread(pState->data, pState->process_size);
	}
}

void cleanup(iter_t iterations, void* cookie)
{
	// fprintf(stderr, "in cleanup\n");
	struct _state* pState = (struct _state*)cookie;

	if (iterations) return;

	cleanup_overhead(iterations, cookie);

	// kill_containers(pState->procs - 1);
}

void mkdir_pipes() {
	int pid = fork();
	if (pid == -1) {
		fprintf(stderr, "Error: Failed to fork\n");
		exit(1);
	} else if (pid == 0) {
		execl("/usr/bin/mkdir", "/usr/bin/mkdir", "pipes", NULL);
	} else {
		wait(NULL);
	}
}

int create_pipes(int procs)
{
	//creating the pipes folder
	mkdir_pipes();

	char *pipe = (char*)malloc(sizeof(char)*10);
	int	i;
	/*
	 * Get a bunch of pipes.
	 */
	morefds();
		for(int i = 0; i < procs; ++i) {
			sprintf(pipe, "pipes/%d", i);
			if(mkfifo(pipe, 0666) != 0) {
				fprintf(stderr, "Error: Failed to create pipe: %s\n", pipe);
				return i;
			}
		}
	return procs;
}

void delete_pipes() {
	// fprintf(stderr, "In delete_pipes\n");
	int pid = fork();
	if (pid == -1) {
		fprintf(stderr, "Error: Failed to fork\n");
		exit(1);
	} else if (pid == 0) {
		execl("sudo", "sudo", "/usr/bin/rm", "-rf", "pipes", NULL);
	} else {
		wait(NULL);
	}
}

void read_token(int read_pipe_fd, char* token) {
  int bytes_read = read(read_pipe_fd, token, 1);
  if (bytes_read != 1) {
    fprintf(stderr, "Error while reading token: %d\n", bytes_read);
    exit(1);
  }
//   fprintf(stderr,"Read: %s from pipe: %d\n", token, read_pipe_fd);
}

void write_token(int write_pipe_fd, char* token) {
  int bytes_written = write(write_pipe_fd, token, 1);
  if (bytes_written != 1) {
    fprintf(stderr, "Error while writing token: %d\n", bytes_written);
    exit(1);
  }
//   fprintf(stderr, "Wrote: %s to pipe: %d\n", token, write_pipe_fd);
}
