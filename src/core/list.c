#include "labster/core/list.h"
#include "labster/core/utils.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t length;
  size_t capacity;
  size_t elementSize;
  void **link;
} ListHeader;

void *_listNewWithCapacity(size_t elementSize, size_t capacity) {
  ListHeader *list = malloc(sizeof(ListHeader) + elementSize * capacity);
  list->elementSize = elementSize;
  list->capacity = capacity;
  list->length = 0;
  list->link = NULL;
  return &list[1];
}

void *_listNew(size_t elementSize) {
  return _listNewWithCapacity(elementSize, 0);
}

static inline ListHeader *listHeader(const void *list) {
  return &((ListHeader*)list)[-1];
}

void listFree(void *list) {
  ListHeader *header = listHeader(list);
  if (header->link) *header->link = NULL;
  free(header);
}

void _listLink(void *list, void **link) {
  ListHeader *header = listHeader(list);
  header->link = link;
}

size_t listLength(const void *list) {
  return listHeader(list)->length;
}

void *listAt(void *list, size_t index) {
  ListHeader *header = listHeader(list);
  return (char*)list + header->elementSize * index;
}

void _listReserve(void **list, size_t capacity) {
  ListHeader *header = listHeader(*list);
  if (header->capacity >= capacity) return;

  header = realloc(header, sizeof(ListHeader) + capacity * header->elementSize);
  *list = &header[1];
  if (header->link) *header->link = *list;
  
  header->capacity = capacity;
}

void *_listPush(void **list) {
  ListHeader *header = listHeader(*list);
  header->length++;
  if (header->capacity <= header->length) {
    _listReserve(list, max(header->length * 2, 1));
  }
  header = listHeader(*list);
  return (char*)*list + (header->length - 1) * header->elementSize;
}

void _listClear(void **list) {
  ListHeader *header = listHeader(*list);
  header->length = 0;
}

void _listPop(void **list) {
  ListHeader *header = listHeader(*list);
  header->length--;
}

void _listAppend(void **list, const void *source, size_t length) {
  _listReserve(list, listLength(*list) + length);

  void *target = listAt(*list, listLength(*list));
  memcpy(target, source, length * listHeader(*list)->elementSize);

  listHeader(*list)->length += length;
}
