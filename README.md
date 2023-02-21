# Final project of Algorithms and Data structures

---

## Design specifications

The goal of the project is to create a ranked list of direct weighted graphs. The program takes as input:
1. <b>d:</b> number of nodes of graphs.
2. <b>k:</b> length of ranked list.
3. A sequence of actions to do between <b>AddGraph</b> and <b>TopK</b>.

The solution uses the Dijkstra's algorithm to compute the minimum path for each matrix and a max-heap to implement the ranked list.

### AddGraph

This action requires to add the graph to the ranked list. The command is followed by the adjacency matrix, with all the elements separated by a ','.

### TopK

With the command topK the program has to print the ranked list that contains the indexes of all the graphs. The ranked list is created calculating the sum of all the shortest paths between the node 0 and all the nodes reachable from the starting node. If there are more graphs with the same sum we take the first arrived.
