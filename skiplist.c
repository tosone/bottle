#include <skiplist.h>

#define zfree free

/* Create a new skiplist. */
skiplist *slCreate(void) {
  int j;
  skiplist *zsl;

  zsl = malloc(sizeof(*zsl));
  zsl->level = 1;
  zsl->length = 0;
  zsl->header = slCreateNode(SKIPLIST_MAXLEVEL, 0, NULL);
  for (j = 0; j < SKIPLIST_MAXLEVEL; j++) {
    zsl->header->level[j].forward = NULL;
    zsl->header->level[j].span = 0;
  }
  zsl->header->backward = NULL;
  zsl->tail = NULL;
  return zsl;
}

/* Create a skiplist node with the specified number of levels.
 * The SDS string 'ele' is referenced by the node after the call. */
skiplistNode *slCreateNode(int level, double score, sds ele) {
  skiplistNode *zn = malloc(sizeof(*zn) + level * sizeof(struct zskiplistLevel));
  zn->score = score;
  zn->ele = ele;
  return zn;
}

/* Returns a random level for the new skiplist node we are going to create.
 * The return value of this function is between 1 and ZSKIPLIST_MAXLEVEL
 * (both inclusive), with a powerlaw-alike distribution where higher
 * levels are less likely to be returned. */
int slRandomLevel(void) {
  int level = 1;
  while ((random() & 0xFFFF) < (SKIPLIST_P * 0xFFFF))
    level += 1;
  return (level < SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL;
}

/* Insert a new node in the skiplist. Assumes the element does not already
 * exist (up to the caller to enforce that). The skiplist takes ownership
 * of the passed SDS string 'ele'. */
skiplistNode *slInsert(skiplist *zsl, double score, sds ele) {
  skiplistNode *update[SKIPLIST_MAXLEVEL], *x;
  unsigned int rank[SKIPLIST_MAXLEVEL];
  int i, level;

  // serverAssert(!isnan(score));
  x = zsl->header;
  for (i = zsl->level - 1; i >= 0; i--) {
    /* store rank that is crossed to reach the insert position */
    rank[i] = i == (zsl->level - 1) ? 0 : rank[i + 1];
    while (x->level[i].forward &&
           (x->level[i].forward->score < score ||
            (x->level[i].forward->score == score &&
             sdscmp(x->level[i].forward->ele, ele) < 0))) {
      rank[i] += x->level[i].span;
      x = x->level[i].forward;
    }
    update[i] = x;
  }
  /* we assume the element is not already inside, since we allow duplicated
     * scores, reinserting the same element should never happen since the
     * caller of zslInsert() should test in the hash table if the element is
     * already inside or not. */
  level = slRandomLevel();
  if (level > zsl->level) {
    for (i = zsl->level; i < level; i++) {
      rank[i] = 0;
      update[i] = zsl->header;
      update[i]->level[i].span = zsl->length;
    }
    zsl->level = level;
  }
  x = slCreateNode(level, score, ele);
  for (i = 0; i < level; i++) {
    x->level[i].forward = update[i]->level[i].forward;
    update[i]->level[i].forward = x;

    /* update span covered by update[i] as x is inserted here */
    x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
    update[i]->level[i].span = (rank[0] - rank[i]) + 1;
  }

  /* increment span for untouched levels */
  for (i = level; i < zsl->level; i++) {
    update[i]->level[i].span++;
  }

  x->backward = (update[0] == zsl->header) ? NULL : update[0];
  if (x->level[0].forward)
    x->level[0].forward->backward = x;
  else
    zsl->tail = x;
  zsl->length++;
  return x;
}

/* Delete an element with matching score/element from the skiplist.
 * The function returns 1 if the node was found and deleted, otherwise
 * 0 is returned.
 *
 * If 'node' is NULL the deleted node is freed by zslFreeNode(), otherwise
 * it is not freed (but just unlinked) and *node is set to the node pointer,
 * so that it is possible for the caller to reuse the node (including the
 * referenced SDS string at node->ele). */
int slDelete(skiplist *zsl, double score, sds ele, skiplistNode **node) {
  skiplistNode *update[SKIPLIST_MAXLEVEL], *x;
  int i;

  x = zsl->header;
  for (i = zsl->level - 1; i >= 0; i--) {
    while (x->level[i].forward &&
           (x->level[i].forward->score < score ||
            (x->level[i].forward->score == score &&
             sdscmp(x->level[i].forward->ele, ele) < 0))) {
      x = x->level[i].forward;
    }
    update[i] = x;
  }
  /* We may have multiple elements with the same score, what we need
     * is to find the element with both the right score and object. */
  x = x->level[0].forward;
  if (x && score == x->score && sdscmp(x->ele, ele) == 0) {
    slDeleteNode(zsl, x, update);
    if (!node)
      slFreeNode(x);
    else
      *node = x;
    return 1;
  }
  return 0; /* not found */
}

/* Free the specified skiplist node. The referenced SDS string representation
 * of the element is freed too, unless node->ele is set to NULL before calling
 * this function. */
void slFreeNode(skiplistNode *node) {
  sdsfree(node->ele);
  zfree(node);
}

/* Free a whole skiplist. */
void slFree(skiplist *zsl) {
  skiplistNode *node = zsl->header->level[0].forward, *next;

  zfree(zsl->header);
  while (node) {
    next = node->level[0].forward;
    slFreeNode(node);
    node = next;
  }
  zfree(zsl);
}

/* Find the rank for an element by both score and key.
 * Returns 0 when the element cannot be found, rank otherwise.
 * Note that the rank is 1-based due to the span of zsl->header to the
 * first element. */
unsigned long slGetRank(skiplist *zsl, double score, sds ele) {
  skiplistNode *x;
  unsigned long rank = 0;
  int i;

  x = zsl->header;
  for (i = zsl->level - 1; i >= 0; i--) {
    while (x->level[i].forward &&
           (x->level[i].forward->score < score ||
            (x->level[i].forward->score == score &&
             sdscmp(x->level[i].forward->ele, ele) <= 0))) {
      rank += x->level[i].span;
      x = x->level[i].forward;
    }

    /* x might be equal to zsl->header, so test if obj is non-NULL */
    if (x->ele && sdscmp(x->ele, ele) == 0) {
      return rank;
    }
  }
  return 0;
}

/* Internal function used by zslDelete, zslDeleteRangeByScore and
 * zslDeleteRangeByRank. */
void slDeleteNode(skiplist *zsl, skiplistNode *x, skiplistNode **update) {
  int i;
  for (i = 0; i < zsl->level; i++) {
    if (update[i]->level[i].forward == x) {
      update[i]->level[i].span += x->level[i].span - 1;
      update[i]->level[i].forward = x->level[i].forward;
    } else {
      update[i]->level[i].span -= 1;
    }
  }
  if (x->level[0].forward) {
    x->level[0].forward->backward = x->backward;
  } else {
    zsl->tail = x->backward;
  }
  while (zsl->level > 1 && zsl->header->level[zsl->level - 1].forward == NULL)
    zsl->level--;
  zsl->length--;
}
