#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHAR_TO_NODE(s, n) {  \
  n.file = s[0] - 'a';        \
  n.rank = s[1] - '1';        \
  n.d = 0;                    \
  n.next = NULL;              \
}

struct node;
struct node {
  int file;
  int rank;
  int d;

  struct node *next;
};

// Movement
int file[] = { -1, 1, 1, -1, 2, -2, 2, -2 };
int rank[] = { 2, 2, -2, -2, 1, 1, -1, -1 };

struct node* new_node(int file, int rank, int d) {
  struct node* n = (struct node*)malloc(sizeof(struct node));
  n->file = file;
  n->rank = rank;
  n->d = d;
  n->next = NULL;
  return n;
}

short knight (const char *p1, const char *p2) {
  struct node start, end, *current, *tail, *new;
  bool visited[8][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
  };
  
  CHAR_TO_NODE(p1, start);
  CHAR_TO_NODE(p2, end);

  current = &start;
  tail = current;

  do {
    if (current->file == end.file && current->rank == end.rank) {
      return current->d;
    }

    if (!visited[current->file][current->rank]) {
      visited[current->file][current->rank] = true;

      for (int i = 0; i < 8; i++) {
        int new_file = current->file + file[i];
        int new_rank = current->rank + rank[i];

        if (new_file < 0 || new_rank < 0 || new_file >= 8 || new_rank >= 8) {
          continue;
        }

        new = new_node(new_file, new_rank, current->d + 1);
        tail->next = new;
        tail = new;
      }
    }

    current = current->next;
  } while (current != NULL);

  return -1;
}
