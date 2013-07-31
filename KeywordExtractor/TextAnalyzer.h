#ifndef TEXTANALYZER_H
#define TEXTANALYZER_H

#include <QFile>
#include <QHash>
#include <QString>
#include <QStringList>

class TextAnalyzer {
public:
    TextAnalyzer(QString documentPath, int coocurredWordsMaxDistance = 2);
    QList<QString> &termsList();
    QHash<QString, QHash<QString, int> >  &coocurrence();
    QHash<QString, int> &documentFrequency();
    QHash<QString, int> &termFrequency();
    int termFrequency(QString word);
    int termsNumber();
    int documentLength();
private:
    void _createStopWords();
    bool _isStopWord(QString word);

    QHash<QString, QHash<QString, int> > _coocurrence;
    QHash<QString, int> _documentFrequency;
    QHash<QString, int> _termFrequency;
    QList<QString> _termsList;
    int _documentLength;
    static QStringList _stopWordsList;
};

#endif
