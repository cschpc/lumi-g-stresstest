simple program to run a kernel (busy waiting loop) on the gpu and monitor execution time. all mpi procs executes the same amount of operations, so what you should look for are big slowdowns that indicate issues with the gpu itself. the wrapper will interrogate dmesg from every node "proc 0", and report all messages that are generated during the execution time of the program itself.

compile: CC -xhip gpu\_stress\_kern.cc -o out -O3
expected ex time ~ 2 minutes, is enough to trigger heating issues (as of Feb 2024, on LUMI-G)

