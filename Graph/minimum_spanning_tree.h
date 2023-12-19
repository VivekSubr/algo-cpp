#pragma once

//We use prim's algorhitm to get minimum cost of visiting each node in a min spanning tree

//1.
//Maintain two disjoint sets of vertices. One containing vertices that are in the growing spanning tree and other 
//that are not in the growing spanning tree.

//2.
//Select the cheapest vertex that is connected to the growing spanning tree and is not in the growing spanning tree 
//and add it into the growing spanning tree. This can be done using Priority Queues. 
//Insert the vertices, that are connected to growing spanning tree, into the Priority Queue.

//3.
//Check for cycles. To do that, mark the nodes which have been already selected and insert only those nodes in the 
//Priority Queue that are not marked.

long prim()
{
    
}