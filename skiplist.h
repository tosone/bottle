#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sds.h>

#define SKIPLIST_MAXLEVEL 32
#define SKIPLIST_P 0.25

typedef struct skiplistNode {
  sds ele;
  double score;
  struct skiplistNode *backward;
  struct zskiplistLevel {
    struct skiplistNode *forward;
    unsigned long span;
  } level[];
} skiplistNode;

typedef struct skiplist {
  struct skiplistNode *header, *tail;
  unsigned long length;
  int level;
} skiplist;

skiplistNode *slCreateNode(int level, double score, sds ele);
skiplist *slCreate(void);
int slRandomLevel(void);
skiplistNode *slInsert(skiplist *zsl, double score, sds ele);
unsigned long slGetRank(skiplist *zsl, double score, sds ele);
int slDelete(skiplist *zsl, double score, sds ele, skiplistNode **node);
void slFree(skiplist *zsl);
void slFreeNode(skiplistNode *node);
int slDelete(skiplist *zsl, double score, sds ele, skiplistNode **node);
void slDeleteNode(skiplist *zsl, skiplistNode *x, skiplistNode **update);
