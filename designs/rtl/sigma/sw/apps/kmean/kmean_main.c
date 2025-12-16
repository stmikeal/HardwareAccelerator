#include <assert.h>
#include <string.h>

#include "example.h"
#include "io.h"

#define IO_X (*(volatile unsigned int *)(0x80000010))
#define IO_Y (*(volatile unsigned int *)(0x80000014))
#define IO_RES (*(volatile unsigned int *)(0x80000018))

typedef struct {
  union {
    struct {
      unsigned short x;
      unsigned short y;
    };
    unsigned int val;
  };
} Coords;

typedef struct {
  Coords coord;
  Coords prev_coord;
} Centroid;

typedef struct {
  Coords coord;
  unsigned int cur_distance;
  int cluster;
} Point;

#define io_buf_points                                                          \
  (*(volatile Coords(*)[IO_BUF_INT_LENGTH / (sizeof(Coords) / sizeof(int))])(  \
      IO_BUF_ADDR))

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

unsigned int abs(int a) { return (unsigned int)((a > 0) ? a : -a); }

unsigned int distance(Coords *a, Coords *b) {
  /*
  int dx = abs(a->x - b->x);
  int dy = abs(a->y - b->y);
  int maxd = MAX(dx, dy);
  int mind = MIN(dx, dy);
  return (int)(251 * maxd + 102 * mind) / 256;
  */
  IO_X = a->val;
  IO_Y = b->val;
  return IO_RES;
}

void kmean(Centroid *clusters, Point *points) {
  int changed = 1;
  while (changed) {
    changed = 0;
    for (int i = 0; i < CLUSTER_SIZE; i++) {
      clusters[i].prev_coord.x = clusters[i].coord.x;
      clusters[i].prev_coord.y = clusters[i].coord.y;
    }
    for (int i = 0; i < POINT_SIZE; i++) {
      points[i].cur_distance = (unsigned int)-1;
      for (int j = 0; j < CLUSTER_SIZE; j++) {
        unsigned int dist = distance(&points[i].coord, &clusters[j].coord);
        if (dist < points[i].cur_distance) {
          points[i].cur_distance = dist;
          points[i].cluster = j;
        }
      }
    }
    for (int j = 0; j < CLUSTER_SIZE; j++) {
      unsigned int size = 1, x = clusters[j].coord.x, y = clusters[j].coord.y;
      for (int i = 0; i < POINT_SIZE; i++) {
        if (points[i].cluster == j) {
          size += 1;
          x += points[i].coord.x;
          y += points[i].coord.y;
        }
      }
      clusters[j].coord.x = x / size;
      clusters[j].coord.y = y / size;
    }
    for (int j = 0; j < CLUSTER_SIZE; j++) {
      if (distance(&clusters[j].coord, &clusters[j].prev_coord) > 1)
        changed = 1;
    }
  }
}

int main(int argc, char *argv[]) {
  IO_LED = 0x0;

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

  for (int i = 0; i < CLUSTER_SIZE; i++) {
    IO_LED = 0xdeadbeef;
    IO_LED = clusters[i].coord.x;
    IO_LED = clusters[i].coord.y;
    io_buf_points[i] = clusters[i].coord;
  }

  // Display status
  IO_LED = 0x55aa55aa;

  // hang
  while (1) {
  }
}
