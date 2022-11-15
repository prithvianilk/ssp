# Docker container context switch benchmark

## Design

- Overhead time of a single pipe ipc is calculated.
- The main process creates #procs named pipes, each with a unique name.
- The main process starts # procs - 1 docker containers. 
- A pipeline of container -> pipe -> container is formed.
- A token is passed from the main process to the next docker container. The main process waits for the same token from the last docker container.
- Time is measured between all the passes. The time is averaged out by the number of processes. Finally, overhead time is removed to find the average time for a context switch.
- Then all (# procs - 1) docker containers are killed.

## References

- Context Switch Latency benchmark: https://lmbench.sourceforge.net/man/lat_ctx.8.html
- Ref for timing related functions: https://lmbench.sourceforge.net/man/timing.3.html

## Documentation

### doit function
- Runs an infinite loop for a child process (i), reading from his previous pipe and writing to his pipe.
- If process_size > 0, allocate some new memory.


### create_daemons function
- Creates procs - 1 new processes (there are now procs processes including main). After doing that it closes unused pipes.
- Then it calls doit for each child. 
```

procs = 3

main p0 
p0 r,w
p1 r,w
p2 r,w

child p1
p0 r
p1 w
p2

child p2
p0
p1 r
p2 w

```
