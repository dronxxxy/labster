#pragma once

#include "labster/core/list.h"
#include "labster/core/memPool.h"
#include "labster/core/slice.h"
#include "labster/core/string.h"
#include <stdint.h>

typedef struct PdfValue PdfValue;

typedef struct {
  Slice name;
  PdfValue *value;
} PdfDictionaryEntry;

typedef enum {
  PDF_VALUE_NAME,
  PDF_VALUE_DICT,
  PDF_VALUE_ARRAY,
  PDF_VALUE_NUMBER,
  PDF_VALUE_STRING,
  PDF_VALUE_LINK,
  PDF_VALUE_STREAM,
} PdfValueKind;

typedef struct PdfValue {
  PdfValueKind kind;
  
  union {
    Slice name;
    Slice string;
    double number;
    List(PdfDictionaryEntry) dictEntries;
    List(PdfValue*) arrayValues;
    struct { int objectId, versionId; } link;
    Slice streamContent;
  };
} PdfValue;

PdfValue *pdfValueNewName(MemPool *mempool, Slice name);
PdfValue *pdfValueNewNumber(MemPool *mempool, double number);
PdfValue *pdfValueNewDict(MemPool *mempool, List(PdfDictionaryEntry) entries);
PdfValue *pdfValueNewArray(MemPool *mempool, List(PdfValue*) values);
PdfValue *pdfValueNewString(MemPool *mempool, Slice string);
PdfValue *pdfValueNewLink(MemPool *mempool, int objectId, int versionId);
PdfValue *pdfValueNewStream(MemPool *mempool, Slice content);

void pdfValueAppend(const PdfValue *value, String *output);
