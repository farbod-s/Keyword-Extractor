#include "Interface.h"

#include <QDir>
#include <QFileDialog>
#include <QString>
#include "KeywordExtractor.h"

Interface::Interface(KeywordExtractor *extractor, QWidget *parent): QWidget(parent) {
    _extractor = extractor;
    _lastDocument = "";

    documentPath = new QLineEdit;
    resultText = new QTextEdit;
    resultText->setReadOnly(true);
    browseButton = new QPushButton("Browse");
    learnButton = new QPushButton("Learn");
    computeButton = new QPushButton("Compute");
    leftLayout = new QVBoxLayout;
    rightLayout = new QVBoxLayout;
    mainLayout = new QHBoxLayout;

    leftLayout->addWidget(documentPath);
    leftLayout->addWidget(resultText);
    rightLayout->addWidget(browseButton);
    rightLayout->addWidget(computeButton);
    rightLayout->addWidget(learnButton);
    rightLayout->addStretch();
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    this->setLayout(mainLayout);

    connect(browseButton, SIGNAL(clicked()), this, SLOT(browseSlot()));
    connect(learnButton, SIGNAL(clicked()), this, SLOT(learnSlot()));
    connect(computeButton, SIGNAL(clicked()), this, SLOT(computeSlot()));
}


void Interface::browseSlot() {
    QString document = QFileDialog::getOpenFileName(this, "Select a document", QDir::currentPath(), "*.txt");
    documentPath->setText(document);
    _lastDocument = document;
}

void Interface::learnSlot() {
    QString corpus = QFileDialog::getExistingDirectory(this, "Select corpus directory", QDir::currentPath());
    if(corpus != "")
        _extractor->learn(corpus);
}

void Interface::computeSlot() {
    if(_lastDocument != "")
        resultText->setText(QStringList(_extractor->extractKeywords(_lastDocument)).join("\n"));
}
