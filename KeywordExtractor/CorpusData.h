#ifndef CORPUSDATA_H
#define CORPUSDATA_H

#include <QHash>
#include <QString>

class CorpusData {
public:
    CorpusData();
    void saveData();
    void eraseData();
    int coocurrence(QString firstWord, QString secondWord);
    int termFrequency(QString word);
    int documentFrequency(QString word);
    int documentsNumber();
    void updateCoocurrence(QHash<QString, QHash<QString, int> > &newData);
    void updateTermFrequency(QHash<QString, int> &newData);
    void updateDocumentFrequency(QHash<QString, int> &newData);
    void updateDocumentsNumber(int newData);
private:
    QHash<QString, QHash<QString, int> > _coocurrence;
    QHash<QString, int> _termFrequency;
    QHash<QString, int> _documentFrequency;
    int _documentsNumber;
};

#endif
