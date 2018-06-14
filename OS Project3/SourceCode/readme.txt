compiling:
gcc JobBenchmark.c -o job
gcc -c project3.c
gcc project3.o -o AuBatch -lpthread

use:
./Aubatch
test job(benchmark) fcfs(policy) 5(number of jobs) 1(MIN_CPU TIME) 10(MAX_CPU TIME)
>list(show job queue)
>quit(exit and show preformance information)