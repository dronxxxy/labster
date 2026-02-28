#include "labster/pdf/document.h"
#include "labster/core/file.h"
#include "labster/core/list.h"
#include "labster/core/memPool.h"
#include "labster/core/slice.h"
#include "labster/core/string.h"
#include "value.h"
#include "internal.h"

Pdf *pdfNew(PdfPageSize pageSize, PdfVersion version) {
  MemPool *mempool = mempoolNew();
  Pdf *pdf = mempoolAllocType(mempool, Pdf);

  pdf->mempool = mempool;
  pdf->buffer = mempoolStringNew(pdf->mempool);
  pdf->pages = mempoolListNew(pdf->mempool, PdfPage*);
  pdf->xRefTable = mempoolListCreate(pdf->mempool, PdfObjectRecord, pdfObjectRecordNew(0, 0xffff, false));
  pdf->lastObjectId = 1;
  pdf->fonts = mempoolListNew(pdf->mempool, PdfFont*);
  pdf->mediaBox = pdfValueNewArray(pdf->mempool, mempoolListCreate(pdf->mempool, PdfValue*,
    pdfValueNewNumber(pdf->mempool, 0),
    pdfValueNewNumber(pdf->mempool, 0),
    pdfValueNewNumber(pdf->mempool, pageSize.width),
    pdfValueNewNumber(pdf->mempool, pageSize.height),
  ));

  pdfWriteHeader(pdf, version);

  pdf->pagesObject = pdfAddLink(pdf);

  return pdf;
}

void pdfSetMetadata(Pdf *pdf, PdfMetadata metadata);

PdfFont *pdfAddFont(Pdf *pdf, Slice name) {
  PdfValue *value = pdfAddObject(pdf, pdfValueNewDict(pdf->mempool, mempoolListCreate(pdf->mempool, PdfDictionaryEntry,
    { sliceFromString("Type"), pdfValueNewName(pdf->mempool, sliceFromString("Font")) },
    { sliceFromString("Subtype"), pdfValueNewName(pdf->mempool, sliceFromString("Type1")) },
    { sliceFromString("BaseFont"), pdfValueNewName(pdf->mempool, name) },
  )));
  PdfFont *font = pdfFontNew(pdf->mempool, value, name);
  listPush(pdf->fonts, font);
  return font;
}

PdfPage *pdfAddPage(Pdf *pdf) {
  PdfValue *content = pdfAddLink(pdf);
  PdfValue *info = pdfAddObject(pdf, pdfValueNewDict(pdf->mempool, mempoolListCreate(pdf->mempool, PdfDictionaryEntry,
    { sliceFromString("Type"), pdfValueNewName(pdf->mempool, sliceFromString("Page")) },
    { sliceFromString("Parent"), pdf->pagesObject },
    { sliceFromString("Contents"), content },
  )));
  PdfPage *page = pdfPageNew(pdf->mempool, info, content);
  listPush(pdf->pages, page);
  return page;
}

void pdfPageDrawText(PdfPage *page, Slice text, PdfPosition position, PdfFont *font, float size, PdfColor color) {
  stringAppendCStr(page->buffer, "BT\n");

  stringAppendCStr(page->buffer, "/");
  stringAppendSlice(page->buffer, font->name);
  stringAppendFmt(page->buffer, 128, " %g ", size);
  stringAppendCStr(page->buffer, "Tf\n");

  stringAppendFmt(page->buffer, 128, "%g %g Td\n", position.x, position.y);

  pdfValueAppend(pdfValueNewString(page->mempool, text), &page->buffer);
  stringAppendCStr(page->buffer, " Tj\n");

  stringAppendCStr(page->buffer, "ET\n");
}

bool pdfWrite(Pdf *pdf, const char *path) {
  PdfValue *trailerMetadata = pdfWriteTrailerMetadata(pdf);
  pdfWritePages(pdf);
  size_t xRefTableOffset = stringLength(pdf->buffer);
  pdfWriteXRefTable(pdf);
  pdfWriteTrailer(pdf, trailerMetadata);
  pdfWriteStartXRef(pdf, xRefTableOffset);
  bool result = fileWrite(path, sliceNew(pdf->buffer, listLength(pdf->buffer)));
  pdfFree(pdf);
  return result;
}

void pdfFree(Pdf *pdf) {
  mempoolFree(pdf->mempool);
}
