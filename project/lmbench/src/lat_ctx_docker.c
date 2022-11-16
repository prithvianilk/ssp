char	*id = "$Id$\n";

#include "bench.h"
#include "lat_ctx_docker/container.h"

#define	MAXPROC	2048
#define	CHUNK	(4<<10)
#define	TRIPS	5
#ifndef	max
#define	max(a, b)	((a) > (b) ? (a) : (b))
#endif

int create_pipes(int procs);

struct _state {
	int	process_size;
	double	overhead;
	int	procs;
	pid_t*	pids;
	int	**p;
	void*	data;
};

int
main(int ac, char **av)
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

	return (0);
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
