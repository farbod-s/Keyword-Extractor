#include <cmath>
#include <QVariant>
#include <QFile>
#include <QDataStream>

#include "CorpusData.h"

CorpusData::CorpusData() {
    QFile coocurrenceFile("db/coocurrence.ked");
    if(!coocurrenceFile.open(QIODevice::ReadOnly))
        coocurrenceFile.open(QIODevice::WriteOnly);
    else {
        QDataStream coocurrenceOut(&coocurrenceFile);
        coocurrenceOut >> _coocurrence;
    }
    coocurrenceFile.close();

    QFile termFrequencyFile("db/termFrequency.ked");
    if(!termFrequencyFile.open(QIODevice::ReadOnly))
        termFrequencyFile.open(QIODevice::WriteOnly);
    else {
        QDataStream termFrequencyOut(&termFrequencyFile);
        termFrequencyOut >> _termFrequency;
    }
    termFrequencyFile.close();

    QFile documentFrequencyFile("db/documentFrequency.ked");
    if(!documentFrequencyFile.open(QIODevice::ReadOnly))
        documentFrequencyFile.open(QIODevice::WriteOnly);
    else {
        QDataStream documentFrequencyOut(&documentFrequencyFile);
        documentFrequencyOut >> _documentFrequency;
    }
    documentFrequencyFile.close();

    QFile documentsNumberFile("db/documentsNumber.ked");
    if(!documentsNumberFile.open(QIODevice::ReadOnly))
        documentsNumberFile.open(QIODevice::WriteOnly);
    else {
        QDataStream documentsNumberOut(&documentsNumberFile);
        documentsNumberOut >> _documentsNumber;
    }
    documentsNumberFile.close();
}

void CorpusData::eraseData() {
    _coocurrence.clear();
    _termFrequency.clear();
    _documentFrequency.clear();
}

int CorpusData::coocurrence(QString firstWord, QString secondWord) {
    if(firstWord == secondWord)
        return -1;
    if(firstWord < secondWord)
        return _coocurrence[firstWord][secondWord];
    else
        return _coocurrence[secondWord][firstWord];
}

int CorpusData::termFrequency(QString word) {
    return _termFrequency[word];
}

int CorpusData::documentFrequency(QString word) {
    return _documentFrequency[word];
}

int CorpusData::documentsNumber() {
    return _documentsNumber;
}

void CorpusData::updateCoocurrence(QHash<QString, QHash<QString, int> > &newData) {
    for(QHash<QString, QHash<QString, int> >::ConstIterator firstIter = newData.constBegin(); firstIter != newData.constEnd(); ++firstIter)
        for(QHash<QString, int>::ConstIterator secondIter = firstIter->constBegin(); secondIter != firstIter->constEnd(); ++secondIter)
            _coocurrence[firstIter.key()][secondIter.key()] += newData[firstIter.key()][secondIter.key()];
}

void CorpusData::updateTermFrequency(QHash<QString, int> &newData) {
    for(QHash<QString, int>::ConstIterator iter = newData.constBegin(); iter != newData.constEnd(); ++iter)
        _termFrequency[iter.key()] += newData[iter.key()];
}

void CorpusData::updateDocumentFrequency(QHash<QString, int> &newData) {
    for(QHash<QString, int>::ConstIterator iter = newData.constBegin(); iter != newData.constEnd(); ++iter)
        _documentFrequency[iter.key()] += newData[iter.key()];
}

void CorpusData::updateDocumentsNumber(int newData) {
    _documentsNumber = newData;
}

void CorpusData::saveData() {
    QFile coocurrenceFile("db/coocurrence.ked");
    if(!coocurrenceFile.open(QIODevice::WriteOnly))
        return;
    QDataStream coocurrenceOut(&coocurrenceFile);
    coocurrenceOut << _coocurrence;
    coocurrenceFile.close();

    QFile termFrequencyFile("db/termFrequency.ked");
    if(!termFrequencyFile.open(QIODevice::WriteOnly))
        return;
    QDataStream termFrequencyOut(&termFrequencyFile);
    termFrequencyOut << _termFrequency;
    termFrequencyFile.close();

    QFile documentFrequencyFile("db/documentFrequency.ked");
    if(!documentFrequencyFile.open(QIODevice::WriteOnly))
        return;
    QDataStream documentFrequencyOut(&documentFrequencyFile);
    documentFrequencyOut << _documentFrequency;
    documentFrequencyFile.close();

    QFile documentsNumberFile("db/documentsNumber.ked");
    if(!documentsNumberFile.open(QIODevice::WriteOnly))
        return;
    QDataStream documentsNumberOut(&documentsNumberFile);
    documentsNumberOut << _documentsNumber;
    documentsNumberFile.close();
}
