# How to compile
- In order to compile the entire project, you will use the make files, you need to do the command `make` in the main directory `tarjan/`. The binary files are created into the directory `tarjan/bin/`and subfolders containing in turn a bin folder. 
- You can also individually fill in a sub-section of the project using the make file in the specific sub-section.
-The `make clean` command deletes all files required for compilation (binary and object files).

# How to run

## Algorithms
### Sequential Tarjan Algorithms
-This algorithm performs sequential Tarjan on a given input graph from a binary file (.bin) following the graph.h library representation.
-To use such an algorithm, we run the program with one of the four possible optimisations (0,1,2 and 3) in the `./tarjan/bin/` folder after compilation, `./tarjan/bin/sequential_Ox.out` , x represents the chosen optimisation.
-The first input parameter is the graph on which we want to run this algorithm `./tarjan/data/seed.bin`.
-The second input parameter is the file that will contain the SCCs found `./tarjan/data/SSC_discovered.bin`.
-An example of using it in the root directory `./tarjan/` is as follows: 
`./bin/sequential_O0.out ./data/seed.bin ./data/SSC_discovered.bin`.

### Preprocess sequential Tarjan Algorithms
-This algorithm performs Tarjan's algorithm but with an antecedent step. Before passing the graph to Tarjan sequential we perform a preprocessing in which trivial SCCs (nodes that have an edge towards themselves) and nodes that cannot be part of SSCs, which are therefore useless to control (nodes without outgoing edges and nodes without incoming edges) are removed.
-To use this algorythm, we run the program with one of the four possible optimisations (0,1,2 and 3) in the `tarjan/bin/` folder after compilation, `./tarjan/bin/sequential_pre_Ox.out` , x represents the chosen optimisation.
-The first input parameter is the graph on which we want to run this algorithm `./tarjan/data/seed.bin`.
-The second input parameter is the file that will contain the SCCs found `./tarjan/data/SSC_discovered.bin`.
-An example of using it in the root directory `./tarjan/` is as follows: 
`./bin/sequential_pre_O0.out ./data/seed.bin ./data/SSC_discovered.bin`.

### Parallel MPI Tarjan Algorithms


### Parallel CUDA Tarjan Algorithms

### Parallel CUDA and MPI Tarjan Algorithms

## Tools
### Generate graphs
#### 

### Print Graph

### Print SSC discovered

## How to obtain report***********************************************************
### Generate measure


### Generate tables and Speedup graph
- In order to generate the graphs and tables of all the measuraments you need to execute the file `extract.py` in the directory `measurements\` with the command `python3 measurements\extract.py`.