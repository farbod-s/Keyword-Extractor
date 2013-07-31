#ifndef CLUSTERMAKER_H
#define CLUSTERMAKER_H

#include <QList>
#include <QString>
#include "CorpusData.h"

class ClusterMaker {
public:
    ClusterMaker(QList<QString> &items, CorpusData *db, int clustersNumber);
    QList<QList<QString> > &returnClusters();
private:
    double _compareWords(QString firstWord, QString secondWord);
    double _compareClusters(QList<QString> firstCluster, QList<QString> secondCluster);
    QList<QList<QString> > _clusters;
    CorpusData *_db;
};

#endif
