#include "internal.h"
#include "labster/core/list.h"
#include "labster/core/memPool.h"
#include "labster/core/slice.h"
#include "labster/core/string.h"
#include "labster/pdf/document.h"
#include "value.h"
#include <assert.h>

PdfPage *pdfPageNew(MemPool *mempool, PdfValue *info, PdfValue *link) {
  PdfPage *page = mempoolAllocType(mempool, PdfPage);
  page->mempool = mempool;
  page->link = link;
  page->info = info;
  page->buffer = mempoolStringNew(mempool);
  return page;
}

PdfFont *pdfFontNew(MemPool *mempool, PdfValue *value, Slice name) {
  PdfFont *font = mempoolAllocType(mempool, PdfFont);
  font->value = value;
  font->name = name;
  return font;
}

void pdfWriteHeader(Pdf *pdf, PdfVersion version) {
  stringAppendCStr(pdf->buffer, "%PDF-");
  switch (version) {
    case PDF_VERSION_10: stringAppendCStr(pdf->buffer, "1.0"); break;
    case PDF_VERSION_14: stringAppendCStr(pdf->buffer, "1.4"); break;
  }
  stringAppendCStr(pdf->buffer, "\n");
}

PdfValue *pdfAddLink(Pdf *pdf) {
  listPush(pdf->xRefTable, pdfObjectRecordNew(-1, 0, true));
  return pdfValueNewLink(pdf->mempool, pdf->lastObjectId++, 0);  
}

void pdfSetLink(Pdf *pdf, PdfValue *link, PdfValue *value) {
  assert(link->kind == PDF_VALUE_LINK);

  pdf->xRefTable[link->link.objectId].offset = stringLength(pdf->buffer);
  
  stringAppendFmt(pdf->buffer, 128, "%d %d obj ", link->link.objectId, link->link.versionId);
  pdfValueAppend(value, &pdf->buffer);
  stringAppendCStr(pdf->buffer, " endobj\n");
}

PdfValue *pdfAddObject(Pdf *pdf, PdfValue *value) {
  PdfValue *link = pdfAddLink(pdf);
  pdfSetLink(pdf, link, value);
  return link;
}

void pdfWritePages(Pdf *pdf) {
  listFor (page, pdf->pages) {
    pdfSetLink(pdf, page->link, pdfValueNewStream(pdf->mempool, sliceNew(page->buffer, listLength(page->buffer))));
  }
}

PdfValue *pdfWriteTrailerMetadata(Pdf *pdf) {
  List(PdfValue*) pageValues = mempoolListCreate(pdf->mempool, PdfValue*);
  listFor (page, pdf->pages)
    listPush(pageValues, page->info);
  PdfValue *kids = pdfValueNewArray(pdf->mempool, pageValues);

  List(PdfDictionaryEntry) fonts = mempoolListNew(pdf->mempool, PdfDictionaryEntry);
  listFor (font, pdf->fonts) {
    PdfDictionaryEntry entry = { font->name, font->value };
    listPush(fonts, entry);
  }
  PdfValue *fontsValue = pdfValueNewDict(pdf->mempool, fonts);
  PdfValue *resources = pdfValueNewDict(pdf->mempool, mempoolListCreate(pdf->mempool, PdfDictionaryEntry,
    { sliceFromString("Font"), fontsValue }
  ));

  pdfSetLink(pdf, pdf->pagesObject, pdfValueNewDict(pdf->mempool, mempoolListCreate(pdf->mempool, PdfDictionaryEntry,
    { sliceFromString("Type"), pdfValueNewName(pdf->mempool, sliceFromString("Pages")) },
    { sliceFromString("Count"), pdfValueNewNumber(pdf->mempool, listLength(pdf->pages)) },
    { sliceFromString("Kids"), kids },
    { sliceFromString("MediaBox"), pdf->mediaBox},
    { sliceFromString("Resources"), resources },
  )));

  PdfValue *metadataValue = pdfValueNewDict(pdf->mempool, mempoolListCreate(pdf->mempool, PdfDictionaryEntry,
    { sliceFromString("Type"), pdfValueNewName(pdf->mempool, sliceFromString("Catalog")) },
    { sliceFromString("Pages"), pdf->pagesObject },
  ));
  return pdfAddObject(pdf, metadataValue);
}

void pdfWriteXRefTable(Pdf *pdf) {
  stringAppendFmt(pdf->buffer, 128, "xref\n0 %ld\n", listLength(pdf->xRefTable));
  listFor(record, pdf->xRefTable)
    stringAppendFmt(pdf->buffer, 128, "%010ld %05d %s\n", record.offset, record.versionId, record.isActive ? "n" : "f");
}

void pdfWriteTrailer(Pdf *pdf, PdfValue *trailerMetadata) {
  stringAppendCStr(pdf->buffer, "trailer\n");
  PdfValue *trailerValue = pdfValueNewDict(pdf->mempool, mempoolListCreate(pdf->mempool, PdfDictionaryEntry,
    { sliceFromString("Size"), pdfValueNewNumber(pdf->mempool, listLength(pdf->xRefTable)) },
    { sliceFromString("Root"), trailerMetadata },
  ));
  pdfValueAppend(trailerValue, &pdf->buffer);
  stringPush(pdf->buffer, '\n');
}

void pdfWriteStartXRef(Pdf *pdf, size_t offset) {
  stringAppendFmt(pdf->buffer, 128, "startxref\n%ld\n%%%%EOF", offset);
}
