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
} dt1;


extern void read1(FILE *file)
{
  //  Initialize distance_table to 999
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      dt1.costs[i][j] = 999;
    }
  }

  char *line = NULL;
  size_t len = 0;

  // Read file
  getline(&line, &len, file);
  sscanf(line, "%d %d %d %d", &dt1.costs[0][0], &dt1.costs[1][1], &dt1.costs[2][2], &dt1.costs[3][3]);
}

void sendPktToNeighbour1() {
  for (int m = 0; m < 4; ++m) {
    struct rtpkt sndpkt;
    sndpkt.sourceid = 1;
    // find min cost iterate the column for each row which correspond to i in min_cost
    for (int i = 0; i < 4; ++i) {
      int min = 999;
      for (int j = 0; j < 4; ++j) {
        if (min >= dt1.costs[i][j]) {
          min = dt1.costs[i][j];
          sndpkt.mincost[i] = min;
        }
      }
    }

    if (dt1.costs[m][m] != 0 && dt1.costs[m][m] != 999) {
      sndpkt.destid = m;
      tolayer2(sndpkt);
    }
  }
}
extern void rtinit1()
{
  /* TODO */
  sendPktToNeighbour1();
}

extern void rtupdate1(struct rtpkt *rcvdpkt)
{
  /* TODO */
  // flag if the packet should be sent
  int dv_updated = 0;
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
      if (min >= dt1.costs[i][j]) {
        min = dt1.costs[i][j];
        old_dv[i] = min;
      }
    }
    
    // assign to new distance table no matter if it is minimum or not
    // this table is just a helper function to calculate minimum cost
    dt1.costs[i][src_id] = dt1.costs[src_id][src_id] + rcvdpkt->mincost[i];

    // get the new distant vector (new minimum cost for each row)
    min = 999;
    for (int j = 0; j < 4; ++j) {
      if (min >= dt1.costs[i][j]) {
        min = dt1.costs[i][j];
        new_dv[i] = min;
      }
    }
  }
  
  for (int i = 0; i < 4; ++i) {
    if (old_dv[i] > new_dv[i]) {
      dv_updated = 1;
    }
  }

  // send back the packet
  if (dv_updated) {
    sendPktToNeighbour1();
  }
}

void printdt1(void)
{
  struct distance_table *dtptr = &dt1;

  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}

extern void linkhandler1(int linkid, int newcost)
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
{
  /* TODO */


}

extern void print_min_cost1()
{

  int min_cost[4];

  for (int i = 0; i < 4; i++)
  {
    int min = dt1.costs[i][i];
    for (int j = 0; j < 4; j++)
    {
      min = min > dt1.costs[i][j] ? dt1.costs[i][j] : min;
    }

    min_cost[i] = min;
  }

  printf("Min cost %d : %d %d %d %d\n", 1, min_cost[0], min_cost[1],
         min_cost[2], min_cost[3]);
}
