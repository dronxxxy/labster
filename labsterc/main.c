#include "labster/core/filePosition.h"
#include "labster/core/logger.h"
#include "labster/core/memPool.h"
#include "labster/core/slice.h"
#include "labster/core/stdLogger.h"
#include "labster/lang/lexer/lexer.h"
#include "labster/lang/lexer/token.h"
#include "labster/pdf/document.h"

bool buildPdf() {
    Pdf *pdf = pdfNew(pdfPageSizeNew(800, 600), PDF_VERSION_14);

    PdfFont *font = pdfAddFont(pdf, sliceFromString("Helvetica"));

    PdfPage *page = pdfAddPage(pdf);
    pdfPageDrawText(page, sliceFromString("Hello, world"), pdfPositionNew(100, 100),
                    font, 24, pdfColorNew(0, 0, 0));

    return pdfWrite(pdf, "output.pdf");
}

bool lexLangFile() {
    MemPool *mempool = mempoolNew();
    Logger *logger = stdLoggerNewAt(mempool);
    FileContent *content = fileContentFromFile(mempool, logger, "examples/syntax.ls");
    if (!content) {
        mempoolFree(mempool);
        return false;
    }
    LangLexer *lexer = langLexerNewIn(mempool, logger, content);
    while (langLexerNext(lexer).kind != LANG_TOKEN_EOI) {}
    bool status = !loggerWasError(logger);
    mempoolFree(mempool);
    return status;
}

int main(int argc, char **argv) {
    return lexLangFile() ? 0 : 1;
}
