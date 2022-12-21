#include <stdio.h>

struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;
extern void tolayer2(struct rtpkt packet);

struct distance_table
{
  int costs[4][4];
} dt2;


extern void read2(FILE *file)
{
  //  Initialize distance_table to 999
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      dt2.costs[i][j] = 999;
    }
  }

  char *line = NULL;
  size_t len = 0;

  // Read file
  getline(&line, &len, file);
  sscanf(line, "%d %d %d %d", &dt2.costs[0][0], &dt2.costs[1][1], &dt2.costs[2][2], &dt2.costs[3][3]);

}

void sendPktToNeighbour2() {
  for (int m = 0; m < 4; ++m) {
    struct rtpkt sndpkt;
    sndpkt.sourceid = 2;
    // find min cost iterate the column for each row which correspond to i in min_cost
    for (int i = 0; i < 4; ++i) {
      int min = 999;
      for (int j = 0; j < 4; ++j) {
        if (min >= dt2.costs[i][j]) {
          min = dt2.costs[i][j];
          sndpkt.mincost[i] = min;
        }
      }
    }

    if (dt2.costs[m][m] != 0 && dt2.costs[m][m] != 999) {
      sndpkt.destid = m;
      tolayer2(sndpkt);
    }
  }
}
extern void rtinit2()
{
  /* TODO */
  sendPktToNeighbour2();
}

extern void rtupdate2(struct rtpkt *rcvdpkt)
{
  /* TODO */
  // update table
  // flag if the packet should be sent
  int src_id = rcvdpkt->sourceid;
  int dest_id = rcvdpkt->destid;
  // old and new minimum cost for node1
  int old_dv[4] = {}, new_dv[4] = {};
  for (int i = 0; i < 4; ++i) {
    // update col = src_id and only when it's smaller -- dont do this
    // store old value first
    int min = 999;
    // this is to check if old minimum cost is actually larger or not
    for (int j = 0; j < 4; ++j) {
      if (min >= dt2.costs[i][j]) {
        old_dv[i] = min = dt2.costs[i][j];
      }
    }
    
    // assign to new distance table no matter if it is minimum or not
    // this table is just a helper function to calculate minimum cost
    dt2.costs[i][src_id] = dt2.costs[src_id][src_id] + rcvdpkt->mincost[i];

    // get the new distant vector (new minimum cost for each row)
    min = 999;
    for (int j = 0; j < 4; ++j) {
      if (min >= dt2.costs[i][j]) {
        new_dv[i] = min = dt2.costs[i][j];
      }
    }
  }
  
  for (int i = 0; i < 4; ++i) {
    if (old_dv[i] != new_dv[i]) {
      sendPktToNeighbour2();
      break;
    }
  }
}

void printdt2(void)
{
  struct distance_table *dtptr = &dt2;

  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}

extern void print_min_cost2()
{

  int min_cost[4];

  for (int i = 0; i < 4; i++)
  {
    int min = dt2.costs[i][i];
    for (int j = 0; j < 4; j++)
    {
      min = min > dt2.costs[i][j] ? dt2.costs[i][j] : min;
    }

    min_cost[i] = min;
  }

  printf("Min cost %d : %d %d %d %d\n", 2, min_cost[0], min_cost[1],
         min_cost[2], min_cost[3]);
}
