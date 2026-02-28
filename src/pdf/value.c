#include "value.h"
#include "labster/core/list.h"
#include "labster/core/memPool.h"
#include "labster/core/slice.h"
#include "labster/core/string.h"
#include <stdio.h>

static PdfValue *pdfValueNew(MemPool *mempool, PdfValueKind kind) {
  PdfValue *value = mempoolAllocType(mempool, PdfValue);
  value->kind = kind;
  return value;
}

PdfValue *pdfValueNewName(MemPool *mempool, Slice name) {
  PdfValue *value = pdfValueNew(mempool, PDF_VALUE_NAME);
  value->name = name;
  return value;
}

PdfValue *pdfValueNewNumber(MemPool *mempool, double number) {
  PdfValue *value = pdfValueNew(mempool, PDF_VALUE_NUMBER);
  value->number = number;
  return value;
}

PdfValue *pdfValueNewDict(MemPool *mempool, List(PdfDictionaryEntry) entries) {
  PdfValue *value = pdfValueNew(mempool, PDF_VALUE_DICT);
  value->dictEntries = entries;
  return value;
}

PdfValue *pdfValueNewArray(MemPool *mempool, List(PdfValue*) values) {
  PdfValue *value = pdfValueNew(mempool, PDF_VALUE_ARRAY);
  value->arrayValues = values;
  return value;
}

PdfValue *pdfValueNewString(MemPool *mempool, Slice string) {
  PdfValue *value = pdfValueNew(mempool, PDF_VALUE_STRING);
  value->string = string;
  return value;
}

PdfValue *pdfValueNewLink(MemPool *mempool, int objectId, int versionId) {
  PdfValue *value = pdfValueNew(mempool, PDF_VALUE_LINK);
  value->link.objectId = objectId;
  value->link.versionId = versionId;
  return value;
}

PdfValue *pdfValueNewStream(MemPool *mempool, Slice content) {
  PdfValue *value = pdfValueNew(mempool, PDF_VALUE_STREAM);
  value->streamContent = content;
  return value;
}

void pdfValueAppend(const PdfValue *value, String *output) {
  switch (value->kind) {
    case PDF_VALUE_NAME:
      stringPush(*output, '/');
      stringAppendSlice(*output, value->name);
      break;

    case PDF_VALUE_DICT:
      stringAppendCStr(*output, "<< ");
      listFor (entry, value->dictEntries) {
        stringPush(*output, '/');
        stringAppendSlice(*output, entry.name);
        stringPush(*output, ' ');
        pdfValueAppend(entry.value, output);
        stringPush(*output, ' ');
      }
      stringAppendCStr(*output, ">>");
      break;

    case PDF_VALUE_ARRAY:
      stringAppendCStr(*output, "[ ");
      listFor (inner, value->arrayValues) {
        pdfValueAppend(inner, output);
        stringPush(*output, ' ');
      }
      stringAppendCStr(*output, "]");
        break;

    case PDF_VALUE_NUMBER: {
      stringAppendFmt(*output, 128, "%g", value->number);
      break;
    }

    case PDF_VALUE_STRING:
      stringAppendCStr(*output, "(");
      for (size_t i = 0; i < value->string.length; i++) {
        char c = value->string.pointer[i];
        switch (c) {
          case '(': stringAppendCStr(*output, "\\("); break;
          case ')': stringAppendCStr(*output, "\\)"); break;
          case '\n': stringAppendCStr(*output, "\\\n"); break;
          default: stringPush(*output, c); break;
        }
      }
      stringAppendCStr(*output, ")");
      break;

    case PDF_VALUE_LINK: {
      stringAppendFmt(*output, 128, "%d %d R", value->link.objectId, value->link.versionId);
      break;
    }

    case PDF_VALUE_STREAM: {
      stringAppendFmt(*output, 128, "<< /Length %ld >> stream\n", value->streamContent.length);
      stringAppendSlice(*output, value->streamContent);
      stringAppendCStr(*output, "\nendstream");
      break;
    }
  }
}
