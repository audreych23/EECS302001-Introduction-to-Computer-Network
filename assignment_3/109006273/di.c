#include <stdio.h>
#include <stdlib.h>


//  Define 999 as infinity
int infinity = 999;

//  Number of nodes
int nodes_num = 4;

//  The distance table
//  table[i][j] reocrd the distance between node i and node j
int table[4][4];


// The structure of the node
struct Node{

  //  Record the minimum distance from the source node to its adjacent nodes
  int dist[4];

  //  Record whether the node have been visited
  int visit[4];

}nodes[4];

//  Print the minimum distance from the source node to its adjacent nodes
//  Input arguement: (int)id is the id of the source node
void print_min_cost(int id)
{
  printf("Min cost %d : %d %d %d %d\n", id, nodes[id].dist[0], nodes[id].dist[1], nodes[id].dist[2], nodes[id].dist[3]);
}

int findMinimumWeightAvailable(int id) {
  Node src_node = nodes[id];
  int min_dist = infinity;
  int min_node = -1;
  for (int i = 0; i < nodes_num; ++i) {
    if (!src_node.visit[i] && src_node.dist[i] <= min_dist) {
      min_dist = src_node.dist[i];
      min_node = i;
    }
  }
  return min_node;
}
//  Use Dijkstra's algorithm to find the minimum distance from the source node to its neighbors
//  Input arguement: (int)id is the id of the source node
void dijkstra(int id)
{
  // Hint: You should refer the distance in table[][] and use the struct Node to implement your algorithm
  /* TODO */
  Node src_node = nodes[id];
  // infinity means it is currently unreachable
  // src_node.visit[i] means if the node has been processed
  for (int i = 0; i < nodes_num; ++i) {
    src_node.dist[i] = infinity;
    src_node.visit[i] = 0;
  }
  src_node.dist[id] = 0;
  for (int m = 0; m < (nodes_num - 1); ++m) {
    // find minimum current updated distance
    // ___ : the minimum distance from the current set of distance reachable
    // is already the minimum from src to that node
    int u = findMinimumDistanceAvailable(id); // utility function
    src_node.visit[u] = 1;
    // update the current distance reachable
    for (int v = 0; v < nodes_num; ++v) {
      // it hasn't been processed, can be reached (table not 0 and initial value is not inf)
      // and smaller than current dist[v]
      if (!src_node.visit[v] &&
          table[u][v] &&
          src_node.dist[u] != infinity &&
          src_node.dist[v] > src_node.dist[u] + table[u][v]) {
        src_node.dist[v] = src_node.dist[u] + table[u][v]
      }
    }
  }
}

// Initialize
void init()
{
  // Initialize nodes' distance to infinity and visit to 0
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      nodes[i].dist[j] = infinity;
      nodes[i].visit[j] = 0;
    }
  }

  // Initialize the distance table
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      table[i][j] = infinity;
    }
  }
}


int main(void)
{
    // Initalize node
    init();

    // Declare variables to open the file
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    file = fopen("di_config.txt", "r");

    // Read the config file to get the distance table
    for(int i = 0;i < 4; i++)
    {
        getline(&line, &len, file);
        sscanf(line, "%d %d %d %d", &table[i][0], &table[i][1], &table[i][2], &table[i][3]);
    }


    // Implement algorithm on node 0
    dijkstra(0);

    // Implement algorithm on node 1
    dijkstra(1);

    // Implement algorithm on node 2
    dijkstra(2);

    // Implement algorithm on node 3
    dijkstra(3);

    // Print
    print_min_cost(0);
    print_min_cost(1);
    print_min_cost(2);
    print_min_cost(3);


    // Close the file
    fclose(file);

    return 0;
}
