#ifndef INTERFACE_H
#define INTERFACE_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

class KeywordExtractor;

class Interface: public QWidget {
Q_OBJECT
public:
    Interface(KeywordExtractor *extractor, QWidget *parent = 0);

    QLineEdit *documentPath;
    QTextEdit *resultText;
    QPushButton *browseButton;
    QPushButton *learnButton;
    QPushButton *computeButton;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QHBoxLayout *mainLayout;
private slots:
    void browseSlot();
    void learnSlot();
    void computeSlot();

private:
    KeywordExtractor *_extractor;
    QString _lastDocument;
};

#endif
