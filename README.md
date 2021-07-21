# Algorithm and Data Structures Project 2020/2021 (Graph Ranker) PoliMi (API 052509 Prof. D. Martinenghi)
## **Statement**
Write a **Graph Ranker**. 

The goal of the program is to keep track of the **indices** of the **K** *best* graphs.
The graphs are **directed** and **weighted**. All of the graphs will have exactly *N* nodes. Indices of the nodes are from 0 to N - 1 .

A graph is better than another graph if it has a lower score. The score of a graph is the **sum** of all the **least cost paths** from the node **0** to any other node. If a node is **disconnected** from the node 0 then the path from 0 to that node is to be considerd 0.

The **index** of a graph is the number of graphs evalued before it, starting from 0. If more than one graph have the same score, then the one with the lower index is better. The indices of the Top K graphs can be printed in **any order**.

**K**, **N** and all of the weights are in the range **[0 , 2<sup>32</sup> - 1]**.
## **Input**
One line with 2 integers, N and K.

A series of text commands each on one line:
* *AddGraph* 
* *TopK*

The command *AddGraph* is followed by N lines each with N integers separated by a comma.

## **Output**
One line for every *TopK* command, each line with **at most K** integers separated by a space.

## **Requirements & Restrictions**
* C language (*C11*)
* No library allowed other than the standard C libraries
* No multithreading
* Read from *stdin* and write on *stdout*
* Every algorithm, data structure or function needs to be coded from scratch

