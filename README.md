# How to compile
- In order to compile the entire project, you will use the make files, you need to do the command `make` in the main directory `tarjan/`. The binary files are created into the directory `tarjan/bin/`and subfolders containing in turn a bin folder. 
- You can also individually fill in a sub-section of the project using the make file in the specific sub-section.
-The `make clean` command deletes all files required for compilation (binary and object files).

# How to run

## Algorithms
### Sequential Tarjan Algorithms
- This algorithm performs sequential Tarjan on a given input graph from a binary file (.bin) following the graph.h library representation.
- To use such an algorithm, we run the program with one of the four possible optimisations (0,1,2 and 3) in the `./tarjan/bin/` folder after compilation, `./tarjan/bin/sequential_Ox.out` , x represents the chosen optimisation.
- The first input parameter is the graph on which we want to run this algorithm `./tarjan/data/seed.bin`.
- The second input parameter is the file that will contain the SCCs found `./tarjan/data/SSC_discovered.bin`.
- An example of using it in the root directory `./tarjan/` is as follows: `./bin/sequential_O0.out ./data/seed.bin ./data/SSC_discovered.bin`.

### Preprocess sequential Tarjan Algorithms
- This algorithm performs Tarjan's algorithm but with an antecedent step. Before passing the graph to Tarjan sequential we perform a preprocessing in which trivial SCCs (nodes that have an edge towards themselves) and nodes that cannot be part of SSCs, which are therefore useless to control (nodes without outgoing edges and nodes without incoming edges) are removed.
- To use this algorithm, we run the program with one of the four possible optimisations (0,1,2 and 3) in the `tarjan/bin/` folder after compilation, `./tarjan/bin/sequential_pre_Ox.out` , x represents the chosen optimisation.
- The first input parameter is the graph on which we want to run this algorithm `./tarjan/data/seed.bin`.
- The second input parameter is the file that will contain the SCCs found `./tarjan/data/SSC_discovered.bin`.
- An example of using it in the root directory `./tarjan/` is as follows: `./bin/sequential_pre_O0.out ./data/seed.bin ./data/SSC_discovered.bin`.

### Parallel MPI Tarjan Algorithms
- This algorithm implements a version of a parallelization of Tarjan's algorithm using MPI.
- To use this algorithm, we launch the program with `mpiexec`.
- The first input parameter is the number of processes to instantiate `\-n y`, where y is the number of processes.
- The second input parameter is the binary file of the mpi program, with one of the four possible optimisations (0,1,2 and 3) in the `tarjan/bin/` folder after compilation, `./tarjan/bin/mpi_Ox.out` , x representing the chosen optimisation.
- The third input parameter is the graph on which we want to run this algorithm `./tarjan/data/seed.bin`.
- The fourth input parameter is the file that will contain the SCCs found `./tarjan/data/SSC_discovered.bin`.
- An example of using it in the root directory `./tarjan/` is as follows:  `mpiexec \-n 8 ./bin/mpi_O0.out ./data/seed.bin ./data/SSC_discovered.bin`.

### Parallel CUDA Global Memory Tarjan Algorithms
- This algorithm performs Tarjan's algorithm but with a preprocessing step performed by the GPU, using CUDA (nvcc v12.0). Before passing the graph to Tarjan sequential, a preprocessing is performed in which trivial SCCs (nodes that have an arc to themselves) and nodes that cannot be part of SSCs, which are therefore useless to check (nodes without outgoing arcs and nodes without incoming arcs) are removed. The data structures used for this GPU preprocessing phase are located on the Global Memory.
- To use this algorithm, we run the program with one of the four possible optimisations (0,1,2 and 3) in the `tarjan/bin/` folder following compilation, `./tarjan/bin/cuda_Ox.out` , x representing the chosen optimisation.
- The first input parameter is the graph on which we want to run this algorithm `./tarjan/data/seed.bin`.
- The second input parameter is the file that will contain the SCCs found `./tarjan/data/SSC_discovered.bin`.
- The third parameter indicates the total number of cuda threads created considering the fixed one-dimensional block size of 512 threads.
- An example of using it in the root directory `./tarjan/` is as follows:  `./bin/cuda_O0.out ./data/seed.bin ./data/SSC_discovered.bin`.

### Parallel CUDA Texture Memory Tarjan Algorithms
- This algorithm performs Tarjan's algorithm but with a preprocessing step performed by the GPU, using CUDA (nvcc v12.0). Before passing the graph to Tarjan sequential, a preprocessing is performed in which trivial SCCs (nodes that have an arc to themselves) and nodes that cannot be part of SSCs, which are therefore useless to check (nodes without outgoing arcs and nodes without incoming arcs) are removed. The data structures used for this GPU preprocessing phase are located on the Texture Memory.
- To use this algorithm, we run the program with one of the four possible optimisations (0,1,2 and 3) in the `tarjan/bin/` folder after compilation, `./tarjan/bin/cuda_texture_Ox.out` , x representing the chosen optimisation.
- The first input parameter is the graph on which we want to run this algorithm `./tarjan/data/seed.bin`.
- The second input parameter is the file that will contain the SCCs found `./tarjan/data/SSC_discovered.bin`.
- The third parameter indicates the total number of cuda threads created considering the fixed one-dimensional block size of 512 threads.
- An example of using it in the root directory `./tarjan/` is as follows:  `./bin/cuda_texture_O0.out ./data/seed.bin ./data/SSC_discovered.bin`.

### Parallel CUDA Global Memory and MPI Tarjan Algorithms
- idea algoritmo
- To use this algorithm, we run the program with `mpiexec`.
- The first input parameter is the number of instantiated MPI processes `\-n y`, where y is the number of processes.
- The second input parameter is the binary file of the program, with one of four possible optimisations (0,1,2 and 3) in the `tarjan/bin/` folder after compilation, `./tarjan/bin/cuda_mpi_Ox.out` , x representing the chosen optimisation.
- The third input parameter is the graph on which you want to run the algorithm `./tarjan/data/seed.bin`.
- The fourth input parameter is the file that will contain the SCCs found `./tarjan/data/SSC_discovered.bin`.
- An example use in the root directory `./tarjan/` is as follows:  `mpiexec \-n 8 ./bin/cuda_mpi_Ox.out ./data/seed.bin ./data/SSC_discovered.bin`.

## Tools
### Generate graphs
#### Random
- This tool generate a random graph following the graph representation of the `graph.h` library, with max_n_node node and each node have mean number of edge with a variance_edge.
- To use this tool, we run the program in the `tarjan/bin/` folder after compilation, `./bin/rsg.out`.
- The first parameter is the path of graph generated `./tarjan/data/random-graph.bin`.
- The second parameter is a integer that indicate the number of node of graph. 
- The thrid parameter is an integer that indicate the the mean of edge for each node.
- The fourth parameter is the variance of number of edge for each node.
- An example use in the root directory `./tarjan/` is as follows:  `./bin/rsg.out ./data/random-graph.bin 1000 15 0.5`.

### Tile
- This tool generates a graph starting from a seed, representation of the `graph.h` library. This tool uses an interger n to generate a graph with 2^n replicas of the seed keeping all the edges already present in the seed, in addition edges are added between the different seeds of the final graph following the probability passed.
- To use this tool, we run the program in the `tarjan/bin/` folder after compilation, `./bin/rgg.out`.
- The first parameter is the path of seed graph `./tarjan/data/seed.bin`.
- The second parameter is the path of graph generated `./tarjan/data/tile-graph.bin`.
- The thrid parameter is an integer n that indicate the 2^n copy to be created.
- The fourth parameter is the  probability of create an edge between a node of a copy and another and viceversa.
- An example use in the root directory `./tarjan/` is as follows:  `./bin/rgg.out ./data/seed.bin ./data/tile-graph.bin 2 0.2`.
- Es: seed 10 edge and second parameter 1 graph generated 20 edge.  
- Es: seed 10 edge and second parameter 2 graph generated 40 edge.  
- Es: seed 10 edge and second parameter 3 graph generated 80 edge.  
- Es: seed 10 edge and second parameter 4 graph generated 160 edge.

### Print Graph
- This tool print to standard output the graph from a file in input.
- The first parameter is the path of the graph file to be printed. 
### Print SSC discovered

### Compare

## How to obtain report***********************************************************
### Generate measure


### Generate tables and Speedup graph
- In order to generate the graphs and tables of all the measuraments you need to execute the file `extract.py` in the directory `measurements\` with the command `python3 measurements\extract.py`.