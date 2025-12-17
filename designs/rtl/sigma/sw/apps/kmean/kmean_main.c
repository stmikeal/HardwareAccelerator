#include <assert.h>
#include <string.h>

#include "example.h"
#include "io.h"
#include "tools.h"
#include "types.h"

#if TEST_TYPE == 0

// Software realization of distance with Newton sqrt method
unsigned int distance(Coords *a, Coords *b) {
  unsigned int dx = abs(a->x - b->x);
  unsigned int dy = abs(a->y - b->y);
  unsigned int res = sqrt(dx * dx + dy * dy);
  return res;
}

#elif TEST_TYPE == 1

// Software realization of distance with aproximation method
unsigned int distance(Coords *a, Coords *b) {
  unsigned int dx = abs(a->x - b->x);
  unsigned int dy = abs(a->y - b->y);
  unsigned int maxd = MAX(dx, dy);
  unsigned int mind = MIN(dx, dy);
  return (int)(251 * maxd + 102 * mind) / 256;
}

#elif TEST_TYPE == 2

// Hardware realization of distance with XIF
unsigned int distance(Coords *a, Coords *b) {
  IO_X = a->val;
  IO_Y = b->val;
  return IO_RES;
}

#else

// Hardware realization of distance with custom coproc (with opcode 0)
unsigned int distance(Coords *a, Coords *b) {
  return custom0_instr_wrapper(a->val, b->val);
}

#endif

void kmean(Centroid *clusters, Point *points) {
  int changed = 1;

  while (changed) {
    changed = 0;
    for (int i = 0; i < CLUSTER_SIZE; i++) {
      clusters[i].prev_coord.x = clusters[i].coord.x;
      clusters[i].prev_coord.y = clusters[i].coord.y;
    }
    for (int i = 0; i < POINT_SIZE; i += 1) {
      points[i].cur_distance = 1000;
      for (int j = 0; j < CLUSTER_SIZE; j += 1) {
        unsigned int dist = 3; // distance(&points[i].coord,
                               // &clusters[j].coord);
        if (dist < points[i].cur_distance) {
          points[i].cur_distance = dist;
          points[i].cluster = j;
        }
      }
      IO_LED = 0x0004;
    }
    for (int j = 0; j < CLUSTER_SIZE; j++) {
      unsigned int size = 0, x = 0, y = 0;
      for (int i = 0; i < POINT_SIZE; i++) {
        if (points[i].cluster == j) {
          size += 1;
          x += points[i].coord.x;
          y += points[i].coord.y;
        }
      }
      if (size != 0) {
        clusters[j].coord.x = x / size;
        clusters[j].coord.y = y / size;
      }
    }
    for (int j = 0; j < CLUSTER_SIZE; j++) {
      if (distance(&clusters[j].coord, &clusters[j].prev_coord) > 0)
        changed = 1;
    }
  }
}

int main(int argc, char *argv[]) {
  IO_LED = 0x0000;

  // Create list of points and initial cluster
  Centroid clusters[CLUSTER_SIZE];
  Point points[POINT_SIZE];
  Point mapping[POINT_SIZE];

  for (int i = 0; i < POINT_SIZE; i++) {
    points[i].coord.x = points_x[i];
    points[i].coord.y = points_y[i];
  }

  for (int i = 0; i < CLUSTER_SIZE; i++) {
    clusters[i].coord.x =
        points[i * (POINT_SIZE / CLUSTER_SIZE - 1) + 1].coord.x;
    clusters[i].coord.y =
        points[i * (POINT_SIZE / CLUSTER_SIZE - 1) + 1].coord.y;
  }

  // Do alghorithm
  kmean(clusters, points);

  // for (int i = 0; i < CLUSTER_SIZE; i++) {
  //   IO_LED = 0xdeadbeef;
  //   IO_LED = clusters[i].coord.x;
  //   IO_LED = clusters[i].coord.y;
  //   io_buf_points[i] = clusters[i].coord;
  // }

  // Display status
  IO_LED = 0xFFFF;

  // hang
  while (1) {
  }
}
