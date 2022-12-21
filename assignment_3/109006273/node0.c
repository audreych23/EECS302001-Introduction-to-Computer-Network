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
  int costs[4][4] ;
} dt0;


extern void read0(FILE *file)
{

  //  Initialize distance_table to 999
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      dt0.costs[i][j] = 999;
    }
  }
  char *line = NULL;
  size_t len = 0;

  // Read file
  getline(&line, &len, file);
  sscanf(line, "%d %d %d %d", &dt0.costs[0][0], &dt0.costs[1][1], &dt0.costs[2][2], &dt0.costs[3][3]);
}


extern void rtinit0()
{
  /* TODO */
  for (int m = 0; m < 3; ++m) {
    struct rtpkt sndpkt;
    sndpkt.sourceid = 0;
    // find min cost iterate the column for each row which correspond to i in min_cost
    for (int i = 0; i < 3; ++i) {
      int min = 999;
      for (int j = 0; j < 3; ++j) {
        if (min <= dt0.costs[i][j]) {
          min = dt0.costs[i][j];
          sndpkt.mincost[i] = min;
        }
      }
    }

    if (dt0.costs[m][m] != 0 && dt0.costs[m][m] != 999) {
      sndpkt.destid = m;
      tolayer2(sndpkt);
    }
  }
}


extern void rtupdate0(struct rtpkt *rcvdpkt)
{
  /* TODO */
  // update table
  int src_id = rcvdpkt->sourceid;
  int dest_id = rcvdpkt->destid;
  for (int i = 0; i < 4; ++i) {
    // update col = src_id and only when it's smaller
    if (dt0.costs[i][src_id] > dt0.costs[src_id][src_id] + min_cost[i]) {
      // dt0.costs[i][src_id] = min(dt0.costs[i][src_id], dt0.costs[src_id][src_id] + min_cost[i]);
      dt0.costs[i][src_id] = dt0.costs[src_id][src_id] + min_cost[i];
    }
  }
  // send back the packet
  for (int m = 0; m < 3; ++m) {
    struct rtpkt sndpkt;
    sndpkt.sourceid = 0;
    // find min cost iterate the column for each row which correspond to i in min_cost
    for (int i = 0; i < 3; ++i) {
      int min = 999;
      for (int j = 0; j < 3; ++j) {
        if (min <= dt0.costs[i][j]) {
          min = dt0.costs[i][j];
          sndpkt.mincost[i] = min;
        }
      }
    }

    if (dt0.costs[m][m] != 0 && dt0.costs[m][m] != 999) {
      sndpkt.destid = m;
      tolayer2(sndpkt);
    }
  }
}


 void printdt0(void)
{
  struct distance_table *dtptr = &dt0;
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n", dtptr->costs[1][1],
           dtptr->costs[1][2], dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n", dtptr->costs[2][1],
           dtptr->costs[2][2], dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n", dtptr->costs[3][1],
           dtptr->costs[3][2], dtptr->costs[3][3]);
}

extern void linkhandler0(int linkid, int newcost)
/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
{
  /* TODO */


}

extern void print_min_cost0()
{
  int min_cost[4];

  for(int i = 0;i < 4;i++)
  {
    int min = dt0.costs[i][i];
    for(int j = 0;j < 4; j++)
    {
      min = min > dt0.costs[i][j] ? dt0.costs[i][j] : min;
    }

    min_cost[i] = min;
  }

  printf("Min cost %d : %d %d %d %d\n", 0, min_cost[0], min_cost[1], min_cost[2], min_cost[3]);
}
