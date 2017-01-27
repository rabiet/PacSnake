#ifndef _map_structs_h
#define _map_structs_h

struct Map {
    int width;
    int length;
    enum MapFieldTypes *fields;
};

#endif
