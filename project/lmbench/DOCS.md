# Docker container context switch benchmark

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