#pragma once

#include "labster/core/slice.h"
#include <stdint.h>

typedef struct PdfPage PdfPage;
typedef struct Pdf Pdf;
typedef struct PdfFont PdfFont;

typedef enum {
  PDF_VERSION_10,
  PDF_VERSION_14,
} PdfVersion;

typedef struct {
  float width;
  float height;
} PdfPageSize;

static inline PdfPageSize pdfPageSizeNew(float width, float height) { return (PdfPageSize) { width, height }; }

typedef struct {
  float x;
  float y;
} PdfPosition;

static inline PdfPosition pdfPositionNew(float x, float y) { return (PdfPosition) { x, y }; }

typedef struct {
  Slice title;
  Slice author;
  Slice subject;
  Slice creator;
} PdfMetadata;

typedef struct {
  uint8_t r, g, b;
} PdfColor;

static inline PdfColor pdfColorNew(uint8_t r, uint8_t g, uint8_t b) { return (PdfColor) { r, g, b }; }

Pdf *pdfNew(PdfPageSize pageSize, PdfVersion version);

void pdfSetMetadata(Pdf *pdf, PdfMetadata metadata);
PdfFont *pdfAddFont(Pdf *pdf, Slice name);
PdfPage *pdfAddPage(Pdf *pdf);

void pdfPageDrawText(PdfPage *page, Slice text, PdfPosition position, PdfFont *font, float size, PdfColor color);

bool pdfWrite(Pdf *pdf, const char *path);
void pdfFree(Pdf *pdf);
