#!/bin/bash
gcc -Ofast src/Astar_write.c lib/write_functions.c -o bin/writebin.exe
./bin/writebin.exe bin/binary.bin

gcc -Ofast src/Astar_heap.c lib/astar_functions.c lib/queue.c -o bin/astar.exe
./bin/astar.exe bin/binary.bin path/result.csv path/longlat.csv
