#include <limits.h>
#include <pthread.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

struct Header {
  alignas(16) size_t size;
  bool is_free;
  struct Header *next;
};

typedef struct Header header_t;

static header_t *head, *tail;

static pthread_mutex_t global_mallock_lock = PTHREAD_MUTEX_INITIALIZER;

// traverse linked list and check if any memory chunk has enough space and
// is_free
static header_t *get_free_block(size_t size) {
  header_t *curr = head;

  while (curr) {
    if (curr->is_free && curr->size >= size) {
      return curr;
    }
    curr = curr->next;
  }

  return NULL;
}

static bool is_init_aligned;

// Try to get already allocated freed chunk.
// If got one -> occupy it and return pointer to block without header.
// If not -> allocate new memory, create header in this memory, point tail to it
// and return block without header
//
//
// WORKS ONLY FOR ALLOCATIONS UNDER 2GB
void *malloc(size_t size) {
  if (!size) {
    return NULL;
  }

  header_t *header;

  size_t aligned_size = size;
  if (size % 16 != 0) {
    aligned_size = size + (16 - (size % 16));
  }

  if (aligned_size > INT_MAX - sizeof(header_t) || aligned_size < size) {
    return NULL;
  }

  pthread_mutex_lock(&global_mallock_lock);
  header = get_free_block(aligned_size);

  if (header) {
    header->is_free = false;
    pthread_mutex_unlock(&global_mallock_lock);
    // return header+1;
    return (void *)(header + 1);
  }

  if (!is_init_aligned) {
    uintptr_t init_break = (uintptr_t)sbrk(0);
    if (init_break % 16 != 0 && sbrk((int)(16 - init_break % 16)) == (void *)-1) {
      pthread_mutex_unlock(&global_mallock_lock);
      return NULL;
    }
    is_init_aligned = true;
  }

  size_t total_size = sizeof(header_t) + aligned_size;

  void *new_block = sbrk(total_size);
  if (new_block == (void *)-1) {
    pthread_mutex_unlock(&global_mallock_lock);
    return NULL;
  }

  header = new_block;
  header->is_free = false;
  header->size = aligned_size;
  header->next = NULL;

  if (!head) {
    head = header;
  }

  if (tail) {
    tail->next = header;
  }

  tail = header;
  pthread_mutex_unlock(&global_mallock_lock);

  return (void *)(header + 1);
}
