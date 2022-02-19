# AStar-algorithm

Here you have an implementation of the A* Star algorithm with a binary heap queue to find the shortest way between two points in a weighted graph.
This case is implemented to the case of the map of Catalonia containing 3472620 nodes and 201235 ways of the streets and roads of the region, 
collected by the OpenStreetMap (OSM) project.    

The csv with the list of nodes and ways of the map is stored in the compressed folder _data.zip_ that you must unzip in order to run the program. The folder must be also name _data_ and be stored in the main folder of the project.  
In the _src_ folder there are the two main programs of the algorithm. The first _AStar\_write_ reads the list of nodes, processes the graph and stores it in a binary file in the _bin_ folder. The other one _Astar\_heap_ reads that binary and performs the path search, storing the results in the _path_ folder.  
  
The produced results are two csv files, one containing the list of ID of the path nodes and another with the longitud and latitud coordinates of the path, that we used to represent the path on a map as in the _finalPath.PNG_ file.  
  
In the _lib_ folder there is stored the corresponding function libraries of the program, including our own implementation of a binary heap priority queue for the A* algorithm. The headers of these libraries can be found in the _include_ folder.

The example implementation looks for the shortest path from Lleida to Girona, but it can be easily changed modifying the ID of the starting and final nodes in the _Astar\_heap.c_ program.  
  
To run the program we provide you two different .sh files that specifies the neccesary linked files for each program, and that you can execute directly. The _run\_astar.sh_ compiles and runs the complete project and _run\_astar\_part2.sh_ can be used to compile and run just the second part of the program, once the binary file with the graph is processed.  
  
This project is made in collaboration with Elena Fraile Valdés.

