/*
 * Student ID: 516021910154
 * Student NAME: Xingyu Liu
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "cachelab.h"

/* define Argnuments
•	-h: Optional help flag that prints usage info
•	-v: Optional verbose flag that displays trace info
•	-s <s>: Number of set index bits (S = 2s is the number of sets)
•	-E <E>: Associativity (number of lines per set)
•	-b <b>: Number of block bits (B = 2b is the block size)
•	-t <tracefile>: Name of the valgrind trace to replay
 */
typedef struct arguments {
    int h;
    int v;
    int s;
    int E;
    int b;
    char * t;
} Arguments;

// define CacheLine,CacheSet and Cache
typedef struct cache_line {
    int valid;
    int tag;
    int visit;
} CacheLine;

typedef CacheLine * CacheSet;
typedef CacheSet * Cache;

//global integers
int VisitTime = 0;
int hit_count = 0;
int miss_count = 0;
int eviction_count = 0;

//print help
void print_help() {
    printf("Usage:  [-hv] -s <num> -E <num> -b <num> -t <tracefile>\n");
    printf("Options:\n");
    printf(" -h Optional help flag that prints usage info.\n");
    printf(" -v Optional verbose flag that displays trace info.\n");
    printf(" -s <num> Number of set index bits.\n");
    printf(" -E <num> Associativity (number of lines per set).\n");
    printf(" -b <num> Number of block offset bits.\n");
    printf(" -t <tracefile>: Name of the valgrind trace to replay.\n");
    printf("\nExamples:\n");
    printf(" linux>  -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf(" linux>  -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

//initialize cache
void init_cache(Cache * cache, Arguments * args) {
    int set_size = 1 << args->s;
    int num_lines = args->E;
    *cache = (CacheSet *)malloc(sizeof(CacheSet) * set_size);
    for(int i =0; i<set_size; ++i) {
        (*cache)[i] = (CacheLine*)malloc(sizeof(CacheLine)*num_lines);
        for(int j = 0; j<num_lines; ++j) {
            (*cache)[i][j].valid = 0;
            (*cache)[i][j].tag = -1;
            (*cache)[i][j].visit = 0;
        }
    }
}

//begin simulate the process
void simulation(Cache * cache, Arguments * args, int address) {
    //get index and tag
    address >>= args->b;
    int set_index, tag_bits, i;

    int sbit = (int) ((1 << args->s) - 1);
    set_index = sbit & address;
    address >>= args->s;
    tag_bits = address;
    CacheSet set = (*cache)[set_index];
    //simulate hit, change visit time and return
    for (i = 0; i < args->E; ++i) {
        if (set[i].valid == 1 && set[i].tag == tag_bits) {
            VisitTime++;
            set[i].visit=VisitTime;
            hit_count++;
            return;
        }
    }
    //simulate miss,change visit time, valid and tag
    miss_count++;
    for (i = 0; i < args->E; ++i) {
        if (set[i].valid == 0) {
            set[i].tag = tag_bits;
            set[i].valid = 1;
            VisitTime++;
            set[i].visit=VisitTime;
            return;
        }
    }
    //simulate eviction
    eviction_count++;
    // LRU, get the mini index
    int min_index = 0, min_time = set[0].visit;
    for (i = 0; i < args->E; ++i) {
        if (set[i].visit < min_time) {
            min_index = i;
            min_time = set[i].visit;
        }
    }
    //change visit time and tag
    set[min_index].tag = tag_bits;
    VisitTime++;
    set[min_index].visit=VisitTime;
}

int main(int argc, char ** argv) {
    Arguments args;

    //get args
    int input;
    while((input = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch(input) {
        case 'h'://help
            args.h = 1;
            print_help();
            exit(0);
        case 'v'://Optional verbose flag that displays trace info
            args.v = 1;
            break;
        case 's'://Number of set index bits (S = 2s is the number of sets)
            args.s = atoi(optarg);
            break;
        case 'E'://Associativity (number of lines per set)
            args.E  = atoi(optarg);
            break;
        case 'b'://Number of block bits (B = 2b is the block size)
            args.b = atoi(optarg);
            break;
        case 't'://Name of the valgrind trace to replay
            args.t = (char*)malloc(sizeof(char) * strlen(optarg));
            strcpy(args.t, optarg);
            break;
        default:
            print_help();
            exit(-1);
        }
    }

    //initialize cache
    Cache cache;
    init_cache(&cache, &args);

    //dealing with input
    char command;
    unsigned address;
    int size = -1;

    FILE * fp = fopen(args.t, "r");
    if(fp == NULL) {
        printf("%s: No such file or directory\n", args.t);
        exit(1);
    }
    while(fscanf(fp, " %c %x,%d", &command, &address, &size) > 0) {
        if(size==-1) continue;
        switch(command) {
            //just ignore I
        case 'I':
            break;
        case 'L'://once for L
            simulation(&cache, &args, address);
            break;
        case 'S'://once for S
            simulation(&cache, &args, address);
            break;
        case 'M'://twice for M
            simulation(&cache, &args, address);
            simulation(&cache, &args, address);
            break;
        default:
            break;
        }
    }
    fclose(fp);

    //print result
    printSummary(hit_count, miss_count, eviction_count);

    //clean cache
    for(int i = 0; i<(1<<args.s); ++i) {
        free(cache[i]);
    }
    free(cache);
    return 0;
}
