#pragma once

typedef struct
{
    union
    {
        struct
        {
            unsigned short x;
            unsigned short y;
        };
        unsigned int val;
    };
} Coords;

typedef struct
{
    Coords coord;
    Coords prev_coord;
} Centroid;

typedef struct
{
    Coords coord;
    unsigned int cur_distance;
    int cluster;
} Point;