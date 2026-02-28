#pragma once

#include <stddef.h>
#include <stdio.h>

void *_listNewWithCapacity(size_t elementSize, size_t capacity);
void *_listNew(size_t elementSize);
void listFree(void *list);

size_t listLength(const void *list);
void *listAt(void *list, size_t index);

void _listReserve(void **list, size_t capacity);
void *_listPush(void **list);
void _listClear(void **list);
void _listPop(void **list);
void _listAppend(void **list, const void *source, size_t length);

void _listLink(void *list, void **link);

#define listNewWithCapacity(type, capacity) ((type*)_listNewWithCapacity(sizeof(type), capacity))
#define listNew(type) ((type*)_listNew(sizeof(type)))

#define listReserve(list, capacity) _listReserve((void**)list, capacity)
#define listPush(list, value) (*(typeof(list))_listPush((void**)&list) = value)
#define listAppend(list, source, length) _listAppend((void**)&list, source, length)
#define listLink(list, link) _listLink(list, (void**)link)
#define listClear(list) _listClear((void**)&(list))
#define listPop(list) ({ \
    typeof(*list) __value = listLast(list); \
    _listPop((void**)&(list)); \
    __value; \
  })
#define listLast(list) list[listLength(list) - 1]

#define List(type) type*

#define listForReverse(name, list) \
  for (ssize_t __i_##name = (ssize_t)listLength(list) - 1, __once_##name = 1; __i_##name >= 0; __i_##name--, __once_##name = 1) \
    for (typeof(*list) name = list[__i_##name]; __once_##name; __once_##name = 0)

#define listFor(name, list) \
  for (size_t __i_##name = 0, __once_##name = 1; __i_##name < listLength(list); __i_##name++, __once_##name = 1) \
    for (typeof(*list) name = list[__i_##name]; __once_##name; __once_##name = 0)

#define listForPtr(name, list) \
  for (size_t __i_##name = 0, __once_##name = 1; __i_##name < listLength(list); __i_##name++, __once_##name = 1) \
    for (typeof(list) name = &list[__i_##name]; __once_##name; __once_##name = 0)

#define listCreate(type, ...) ({ \
    List(type) __result = listNew(type); \
    type __values[] = { __VA_ARGS__ }; \
    listAppend(__result, __values, sizeof(__values) / sizeof(type)); \
    __result; \
  })

