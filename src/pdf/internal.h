#include "labster/core/memPool.h"
#include "labster/core/string.h"
#include "labster/pdf/document.h"
#include "value.h"

typedef struct PdfPage {
  MemPool *mempool;
  PdfValue *link, *info;
  String buffer;
} PdfPage;

typedef struct {
  size_t offset;
  int versionId;
  bool isActive;
} PdfObjectRecord;

typedef struct PdfFont {
  PdfValue *value;
  Slice name;
} PdfFont;

static inline PdfObjectRecord pdfObjectRecordNew(size_t offset, size_t versionId, bool isActive) {
  return (PdfObjectRecord) {
    .isActive = isActive,
    .versionId = versionId,
    .offset = offset,
  };
}

typedef struct Pdf {
  MemPool *mempool;
  String buffer;
  List(PdfPage*) pages;
  PdfValue *mediaBox, *pagesObject;
  List(PdfObjectRecord) xRefTable;
  List(PdfFont*) fonts;
  int lastObjectId;
} Pdf;

PdfPage *pdfPageNew(MemPool *mempool, PdfValue *info, PdfValue *link);
PdfFont *pdfFontNew(MemPool *mempool, PdfValue *value, Slice name);

void pdfWriteHeader(Pdf *pdf, PdfVersion version);
void pdfWritePages(Pdf *pdf);
PdfValue *pdfWriteTrailerMetadata(Pdf *pdf);
void pdfWriteXRefTable(Pdf *pdf);
void pdfWriteTrailer(Pdf *pdf, PdfValue *trailerMetadata);
void pdfWriteStartXRef(Pdf *pdf, size_t offset);

PdfValue *pdfAddLink(Pdf *pdf);
void pdfSetLink(Pdf *pdf, PdfValue *link, PdfValue *value);
PdfValue *pdfAddObject(Pdf *pdf, PdfValue *value);
