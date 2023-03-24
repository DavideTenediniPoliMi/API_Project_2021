# Algorithm & Data Structures Project 2020/2021 (Graph Ranker) 
## PoliMi (API 052509 Prof. D. Martinenghi)
The codebase.c file contains the final submission which passed all the testcases within the memory and time limits, thus scoring the maximum amount of points (30L).
## **Problem Statement**
Write a **Graph Ranker**. 

The goal of the program is to keep track of the **indices** of the **K** *best* graphs.
The graphs are **directed** and **weighted**. All of the graphs will have exactly **N** nodes. Indices of the nodes are from 0 to N - 1 .

A graph is better than another graph if it has a lower score. The score of a graph is the **sum** of all the **least cost paths** from the node **0** to any other node. If a node is **disconnected** from node 0 then the path from 0 to that node is to be considered 0.

The **index** of a graph is the number of graphs evaluated before it, starting from 0. If more than one graph has the same score, then the one with the lower index is better. The indices of the Top K graphs can be printed in **any order**.

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
* Every algorithm, data structure, or function needs to be coded from scratch

## **Solution**
The program has 4 main modules : (*each one has a dedicated file in the folder **src** other than being in the main **codebase***)
* Fast IO
* Heap
* Graph Scoring
* Program Flow

### **Fast IO**
For input and output, I use `gc` and `pc`. These are aliases for `_getchar_nolock` and `_putchar_nolock` on Windows, or `getchar_unlocked` and `putchar_unlocked` on Linux.

`int expect_ui()` : ignores and discards everything until it finds the first unsigned integer and returns it. If no unsigned integer is found before the end of the file/input stream, it returns `EOF`. Internally it uses a custom (*faster*) version of `atoi` I made which works only with unsigned integers. The return type is `int` because of `EOF`, so this means that if you work on numbers bigger than 2<sup>31</sup> it won't work properly.

`int expect_char()` : ignores and discards everything until it finds the first alphabetical character and returns it. If no alphabetical character is found before the end of the file/input stream, it returns `EOF`.

`void print_ui(ui num)` : prints `num`. Internally it uses a custom (*faster*) version of `itoa` I made which works only with unsigned integers.

### **Heap**
This heap implementation can be initialized as a min-heap, a max-heap, a bounded-min-heap, or a bounded-max-heap.
It is specifically implemented to solve this problem. In fact, the elements are indices (**node** indices for the graph or **graph** indices for the ranking) with an associated priority (**weight** for Dijkstra or **score** for the ranking). In both cases, the indices are unsigned integers and the priorities are unsigned long long integers.

`void init_pq(const ui bound, const ui capacity, int (*cmp_fun)(const node *, const node *), priority_queue *pq)` : 
* `bound` : is the maximum number of elements the heap can contain. If you don't want a bounded priority queue just put a big enough number.
* `capacity` : is the initial size of the allocated heap. The heap will automatically resize upon need until it reaches `bound`. If you want a static and already allocated heap, put `bound` = `capacity`.
* `cmp_fun` : is the function that dictates the order inside the heap. It should return a value greater than 0 if the first node should be higher than the second node, otherwise a value smaller than 0.

    ```C 
    // Examples of cmp_fun

    typedef struct t_node{
        ui   index;
        ulli priority;   
    } node;

    //min-heap
    int cmp_fun_min(const node *n1, const node *n2) {
        return (n2->priority - n1->priority);
    }

    //max-heap
    int cmp_fun_max(const node *n1, const node *n2) {
        return (n1->priority != n2->priority) ? (n1->priority - n2->priority) : -1;
    }
    ```

`void destroy_pq(priotity_queue *pq)` : frees the memory allocated.

`void reset_heap(priotity_queue *pq)` : empties the heap.

`ui is_empty_pq(priotity_queue *pq)` : returns 1 if the heap is empty, 0 otherwise.

`ui peek_pq(const priority_queue *pq)` : returns the index of the top node if it exists, -1 otherwise.

`void push_pq(const ui index, const ui priority, priority_queue *pq)` : adds a node to the heap with `index` and `priority`. If the size of the heap is equal to `bound` it rejects the node. (*Use this method for unbounded heaps*).

`void push_bounded_pq(const ui index, const ui priority, priority_queue *bounded_pq)` : if there are fewer nodes than bound it adds a node to the heap with `index` and `priority`, otherwise if priority is better than the worst priority in the heap it deletes the worst node in the heap and it inserts the new one. (*Use this method for bounded heaps*).

`void pop_pq(priority_queue *pq)` : deletes the best node in the heap if it exists. (*Use this method for unbounded heaps*).

### **Graph Scoring**
I use Dijkstra's algorithm to find the shortest paths from node 0 to every other node. I use a min-heap as a priority queue. I keep track of the nodes that are currently inside the queue with the array `pushed`, so I only push a node if it's not already in the queue.

To compute the score I sum every cell in the `distance` array, except the distances equal to `ULLIMAX`. 

### **Program Flow**
I initialize all the structures in the `main`, this way I don't spend a lot of time doing `malloc`. When needed I reset the structures. The program starts by reading N and K. Then it expects a command, if the command starts with an *A* I save the adjacency matrix, compute the score and push it into the bounded priority queue. If it starts with a *T* I print the top K. I keep repeating this process until I reach `EOF`.
