#!/bin/bash
gcc -Ofast src/Astar_heap.c lib/astar_functions.c lib/queue.c -o bin/a.exe
./bin/a.exe bin/binary.bin path/result.csv
