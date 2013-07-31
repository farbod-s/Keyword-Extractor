#include "KeywordExtractor.h"

#include <float.h>
#include <cmath>
#include <QDir>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QPair>

KeywordExtractor::KeywordExtractor() {
    _corpus = new CorpusData();
    _interface = new Interface(this);
}

void KeywordExtractor::run() {
    _interface->show();
}

void KeywordExtractor::learn(QString corpus) {
    _corpus->eraseData();

    QDir dir(corpus);
    QStringList filters;
    filters << "*.txt";
    dir.setNameFilters(filters);
    QStringList files = dir.entryList();
    int documentNumber = files.length();
    _corpus->updateDocumentsNumber(documentNumber);
    for(QStringList::ConstIterator iter = files.constBegin(); iter != files.constEnd(); ++iter) {
        QString documentPath = corpus + "\\" + *iter;
        TextAnalyzer fileAnalyzer(documentPath);
        _corpus->updateCoocurrence(fileAnalyzer.coocurrence());
        _corpus->updateDocumentFrequency(fileAnalyzer.documentFrequency());
        _corpus->updateTermFrequency(fileAnalyzer.termFrequency());
    }
    _corpus->saveData();
}

bool sortTF_IDF(QPair<QString, double> firstWord, QPair<QString, double> secondWord) {
    return firstWord.second > secondWord.second;
}

QList<QString> KeywordExtractor::extractKeywords(QString document) {
    TextAnalyzer textAnalyzer(document);
    int clustersNumber = (int) sqrt((double) (textAnalyzer.termsNumber()));
    ClusterMaker clusterMaker(textAnalyzer.termsList(), _corpus, clustersNumber);
    QList<QString> keywords;
    QList<QList<QString> > clusters = clusterMaker.returnClusters();
    int i = 0;
    double *clustersTF_IDF = new double [clusters.length()];
    double maxClusterTF_IDF = -1, minClusterTF_IDF = DBL_MAX;
    for(QList<QList<QString> >::ConstIterator firstIter = clusters.constBegin(); firstIter != clusters.constEnd(); ++firstIter) {
        double clusterTF_IDF = 0, count = 0;
        for(QList<QString>::ConstIterator secondIter = firstIter->constBegin(); secondIter != firstIter->constEnd(); ++secondIter) {
            double allDocs = (double) _corpus->documentsNumber() + 1;
            double wordDocs = (double) _corpus->documentFrequency(*secondIter) + 1;
            double keywordTF_IDF = (double) textAnalyzer.termFrequency(*secondIter) * (log(allDocs / wordDocs));
            clusterTF_IDF += keywordTF_IDF; count++;
        }
        clustersTF_IDF[i] = (clusterTF_IDF / count);
        if(clustersTF_IDF[i] < minClusterTF_IDF)
            minClusterTF_IDF = clustersTF_IDF[i];
        if(clustersTF_IDF[i] > maxClusterTF_IDF)
            maxClusterTF_IDF = clustersTF_IDF[i];
        i++;
    }
    for(int i = 0; i < clusters.length(); i++) {
        if(clustersTF_IDF[i] > ((maxClusterTF_IDF + minClusterTF_IDF) / 2)) {
            double maxTF_IDF = -1, secondMaxTF_IDF = -1;
            QString maxKeyword, secondMaxKeyword;
            for(int j = 0; j < clusters[i].length(); j++) {
                double allDocs = (double) _corpus->documentsNumber() + 1;
                double wordDocs = (double) _corpus->documentFrequency(clusters[i][j]) + 1;
                double currentKeywordTF_IDF = (double) textAnalyzer.termFrequency(clusters[i][j]) * (log(allDocs / wordDocs));
                if(currentKeywordTF_IDF > maxTF_IDF) {
                    secondMaxTF_IDF = maxTF_IDF;
                    secondMaxKeyword = maxKeyword;
                    maxTF_IDF = currentKeywordTF_IDF;
                    maxKeyword = clusters[i][j];
                }
                else if(currentKeywordTF_IDF > secondMaxTF_IDF) {
                    secondMaxTF_IDF = currentKeywordTF_IDF;
                    secondMaxKeyword = clusters[i][j];
                }
            }
            keywords.append(maxKeyword);
            if(secondMaxTF_IDF != -1)
                keywords.append(secondMaxKeyword);
        }
        else {
            double maxTF_IDF = -1;
            QString maxKeyword;
            for(int j = 0; j < clusters[i].length(); j++) {
                double allDocs = (double) _corpus->documentsNumber() + 1;
                double wordDocs = (double) _corpus->documentFrequency(clusters[i][j]) + 1;
                double currentKeywordTF_IDF = (double) textAnalyzer.termFrequency(clusters[i][j]) * (log(allDocs / wordDocs));
                if(currentKeywordTF_IDF > maxTF_IDF) {
                    maxTF_IDF = currentKeywordTF_IDF;
                    maxKeyword = clusters[i][j];
                }
            }
            keywords.append(maxKeyword);
        }
    }
    return keywords;
}

