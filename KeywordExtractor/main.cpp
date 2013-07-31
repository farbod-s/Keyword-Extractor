#include <QApplication>

#include "KeywordExtractor.h"

int main(int argc, char **argv) {
    QApplication application(argc, argv);

    KeywordExtractor extractor;
    extractor.run();

    return application.exec();
}
