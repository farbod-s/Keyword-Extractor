#include <QStringList>
#include <QTextStream>
#include "TextAnalyzer.h"

QStringList TextAnalyzer::_stopWordsList;

TextAnalyzer::TextAnalyzer(QString documentPath, int coocurredWordsMaxDistance) {
    QFile document(documentPath);
    if(!document.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    _createStopWords();

    QTextStream inStream(&document);
    inStream.setCodec("UTF-8");
    QStringList tokens = ((QString) inStream.readAll()).split(QRegExp("\\W+"), QString::SkipEmptyParts);
    _documentLength = tokens.length();
    for(int i = 0; i < tokens.length(); i++) {
        if(_isStopWord(tokens[i]))
            continue;
        _documentFrequency[tokens[i]] = 1;
        _termFrequency[tokens[i]] += 1;
        for(int j = 1; j <= coocurredWordsMaxDistance; j++) {
            if(i + j >= tokens.length() || tokens[i] == tokens[i + j])
                continue;
            QString lowerWord = (tokens[i] < tokens[i + j] ? tokens[i] : tokens[i + j]);
            QString greaterWord = (tokens[i] > tokens[i + j] ? tokens[i] : tokens[i + j]);
            _coocurrence[lowerWord][greaterWord] += 1;
        }
    }
    _termsList = _termFrequency.keys();
    document.close();
}

void TextAnalyzer::_createStopWords() {
    QFile file("resource/stop.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    in.setCodec("UTF-8");
    while(!in.atEnd())
        _stopWordsList << in.readLine();

    file.close();
}

bool TextAnalyzer::_isStopWord(QString word) {
    return _stopWordsList.contains(word, Qt::CaseInsensitive);
}

QList<QString> &TextAnalyzer::termsList() {
    return _termsList;
}

QHash<QString, QHash<QString, int> > &TextAnalyzer::coocurrence() {
    return _coocurrence;
}

QHash<QString, int> &TextAnalyzer::documentFrequency() {
    return _documentFrequency;
}

QHash<QString, int> &TextAnalyzer::termFrequency() {
    return _termFrequency;
}

int TextAnalyzer::termFrequency(QString word) {
    return _termFrequency[word];
}

int TextAnalyzer::termsNumber() {
    return _termFrequency.keys().length();
}

int TextAnalyzer::documentLength() {
return _documentLength;
}
