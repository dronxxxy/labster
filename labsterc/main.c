#include "labster/core/slice.h"
#include "labster/pdf/document.h"

int main(int argc, char **argv) {
    Pdf *pdf = pdfNew(pdfPageSizeNew(800, 600), PDF_VERSION_14);

    PdfFont *font = pdfAddFont(pdf, sliceFromString("Helvetica"));

    PdfPage *page = pdfAddPage(pdf);
    pdfPageDrawText(page, sliceFromString("Hello, world"), pdfPositionNew(100, 100),
                    font, 24, pdfColorNew(0, 0, 0));
    
    return pdfWrite(pdf, "output.pdf") ? 0 : 1;
}
