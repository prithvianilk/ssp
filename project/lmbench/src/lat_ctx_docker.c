char	*id = "$Id$\n";

#include "bench.h"
#include "lat_ctx_docker/container.h"

#define	MAXPROC	2048
#define	CHUNK	(4<<10)
#define	TRIPS	5
#ifndef	max
#define	max(a, b)	((a) > (b) ? (a) : (b))
#endif

void	initialize_overhead(iter_t iterations, void* cookie);
void	cleanup_overhead(iter_t iterations, void* cookie);
void	benchmark_overhead(iter_t iterations, void* cookie);
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

	benchmp(initialize_overhead, benchmark_overhead, cleanup_overhead, 0, 1, warmup, repetitions, &state);
	if (gettime() == 0) return(0);
	state.overhead = gettime();
	state.overhead /= get_n();
	fprintf(stderr, "\n\"size=%dk ovr=%.2f\n", state.process_size/1024, state.overhead);

	return (0);
}

void initialize_overhead(iter_t iterations, void* cookie)
{
	fprintf(stderr, "in initialise_overhead\n");
	int procs;
	struct _state* pState = (struct _state*)cookie;

	if (iterations) return;

	pState->pids = NULL;

	pState->data = (pState->process_size > 0) ? malloc(pState->process_size) : NULL;
	if (pState->data)
		bzero(pState->data, pState->process_size);

	procs = create_pipes(pState->procs);
	if (procs < pState->procs) {
		cleanup_overhead(0, cookie);
		exit(1);
	}
}

void benchmark_overhead(iter_t iterations, void* cookie)
{
	fprintf(stderr, "in benchmark overhead\n");
	struct _state* pState = (struct _state*)cookie;
	int	i = 0;
	char* msg = (char*)malloc(sizeof(char)*2);
	msg = "a";

	char *write_pipe_path = (char*)malloc(sizeof(char)*10);
	char *read_pipe_path = (char*)malloc(sizeof(char)*10);

	fprintf(stderr, "iterations %d\n", iterations);

	while (iterations-- > 0) {
		fprintf(stderr, "i=%d\n", i);
		int pid;
		switch(pid = fork())
		{
			case -1:
				fprintf(stderr, "could not fork");
				exit(1);

			case 0:
				sprintf(read_pipe_path, "pipes/%d", i);
				fprintf(stderr, "started open for read\n", i);
				int read_pipe_fd = open(read_pipe_path, O_RDONLY);
				fprintf(stderr, "opened for read\n", i);

				char *msg_read = (char*)malloc(sizeof(char)*2);
				read_token(read_pipe_fd, msg_read);
				fprintf(stderr, "completed read\n", i);
				exit(0);
				break;

			default:
				sprintf(write_pipe_path, "pipes/%d", i);
				fprintf(stderr, "started open for write\n", i);
				int write_pipe_fd = open(write_pipe_path, O_WRONLY);
				fprintf(stderr, "opened for write\n", i);

				write_token(write_pipe_fd, msg);
				fprintf(stderr, "completed write\n", i);

				if (++i == pState->procs) {
					i = 0;
				}
				bread(pState->data, pState->process_size);
			break;
		}
	}
}

void cleanup_overhead(iter_t iterations, void* cookie)
{
	fprintf(stderr, "in cleanup overhead\n");
	int i;
	struct _state* pState = (struct _state*)cookie;

	if (iterations) return;

	if (pState->data) free(pState->data);

	delete_pipes();
}

int create_pipes(int procs)
{
	//creating the pipes folder
	char *dir = "pipes";
	mkdir(dir, 0666);

	char *pipe = (char*)malloc(sizeof(char)*10);
	int	i;
	/*
	 * Get a bunch of pipes.
	 */
	morefds();
		for(int i=0;i<procs;++i)
		{
			sprintf(pipe, "pipes/%d", i);
			if(mkfifo(pipe, 0666)!=0)
			{
				return i;
			}
		}
	return procs;
}

void delete_pipes()
{
	int pid = fork();
	if(pid == 0)
	{
		execl("/usr/bin/rm", "/usr/bin/rm", "-rf", "pipes", NULL);
	}
}

void read_token(int read_pipe_fd, char* token) {
  int bytes_read = read(read_pipe_fd, token, 1);
  if (bytes_read != 1) {
    fprintf(stderr, "Error while reading token: %d\n", bytes_read);
    exit(1);
  }
  fprintf(stderr,"Read: %s from pipe: %d\n", token, read_pipe_fd);
//   fflush(stdout);
}

void write_token(int write_pipe_fd, char* token) {
  int bytes_written = write(write_pipe_fd, token, 1);
  if (bytes_written != 1) {
    fprintf(stderr, "Error while writing token: %d\n", bytes_written);
    exit(1);
  }
  fprintf(stderr, "Wrote: %s to pipe: %d\n", token, write_pipe_fd);
//   fflush(stdout);
}