#ifndef KEYWORDEXTRACTOR_H
#define KEYWORDEXTRACTOR_H

#include <QList>
#include <QString>

#include "ClusterMaker.h"
#include "CorpusData.h"
#include "Interface.h"
#include "TextAnalyzer.h"

class KeywordExtractor {
public:
    KeywordExtractor();
    void run();
    void learn(QString corpus);
    QList<QString> extractKeywords(QString document);
private:
    CorpusData *_corpus;
    Interface *_interface;
};

#endif
