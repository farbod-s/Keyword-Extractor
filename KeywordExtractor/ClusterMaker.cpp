#include "ClusterMaker.h"
#include "CorpusData.h"

#include <float.h>
#include <QSqlQuery>

struct SimilarityMatrixEntry {
    int i;
    int j;
    double sim;
};

bool compareEntries(SimilarityMatrixEntry *a, SimilarityMatrixEntry *b) {
    return a->sim > b->sim;
}

ClusterMaker::ClusterMaker(QList<QString> &items, CorpusData *db, int clustersNumber) {
    _db = db;
        for(QList<QString>::ConstIterator iter = items.constBegin(); iter != items.constEnd(); ++iter) {
            QList<QString> list;
            list.append(*iter);
            _clusters.append(list);
        }
        int wordsNumber = _clusters.length();
        SimilarityMatrixEntry similarityMatrix[wordsNumber][wordsNumber];
        QList<QList<SimilarityMatrixEntry *> > sortedSimilarityMatrix;
        for(int i = 0; i < wordsNumber; i++) {
            sortedSimilarityMatrix.append(QList<SimilarityMatrixEntry *>());
            for(int j = 0; j < wordsNumber; j++) {
                similarityMatrix[i][j].i = i;
                similarityMatrix[i][j].j = j;
                if(i == j)
                    similarityMatrix[i][j].sim = -1;
                else
                    similarityMatrix[i][j].sim = _compareWords(_clusters[i][0], _clusters[j][0]);
                sortedSimilarityMatrix[i].append(&(similarityMatrix[i][j]));
            }
            qSort(sortedSimilarityMatrix[i].begin(), sortedSimilarityMatrix[i].end(), compareEntries);
        }
        for(int i = 0; i < (_clusters.length() - clustersNumber); i++) {
            SimilarityMatrixEntry maxEntry; maxEntry.sim = -1;
            for(int i = 0; i < sortedSimilarityMatrix.length(); i++)
                if(sortedSimilarityMatrix[i][0]->sim > maxEntry.sim) {
                    maxEntry.i = sortedSimilarityMatrix[i][0]->i;
                    maxEntry.j = sortedSimilarityMatrix[i][0]->j;
                    maxEntry.sim = sortedSimilarityMatrix[i][0]->sim;
                }
            _clusters[maxEntry.i].append(_clusters[maxEntry.j]);
            _clusters[maxEntry.j].clear(); _clusters[maxEntry.j].append("#");
            for(int i = 0; i < wordsNumber; i++) {
                similarityMatrix[i][maxEntry.j].sim = similarityMatrix[maxEntry.j][i].sim = -1;
                if(_clusters[i][0] == "#" || i == maxEntry.i)
                    similarityMatrix[i][maxEntry.i].sim = similarityMatrix[maxEntry.i][i].sim = -1;
                else
                    similarityMatrix[i][maxEntry.i].sim = similarityMatrix[maxEntry.i][i].sim = _compareClusters(_clusters[i], _clusters[maxEntry.i]);
                qSort(sortedSimilarityMatrix[i].begin(), sortedSimilarityMatrix[i].end(), compareEntries);
            }
        }
        int bound = wordsNumber;
        for(int i = 0; i < bound; i++)
            if(_clusters[i][0] == "#") {
                _clusters.removeAt(i);
                i--; bound--;
            }
}

QList<QList<QString> > &ClusterMaker::returnClusters() {
    return _clusters;
}

double ClusterMaker::_compareWords(QString firstWord, QString secondWord) {
    return (double) _db->coocurrence(firstWord, secondWord) / (double) (_db->termFrequency(firstWord) + _db->termFrequency(secondWord));
}

double ClusterMaker::_compareClusters(QList<QString> firstCluster, QList<QString> secondCluster) {
    // centroid

    /* double sim = 0, count = 0;
    for(QList<QString>::ConstIterator firstIter = firstCluster.constBegin(); firstIter != firstCluster.constEnd(); ++firstIter)
        for(QList<QString>::ConstIterator secondIter = secondCluster.constBegin(); secondIter != secondCluster.constEnd(); ++secondIter) {
            sim += _compareWords(*firstIter, *secondIter);
            count++;
        }
    return sim / count; */

    // complete link

    double sim = DBL_MAX;
    for(QList<QString>::ConstIterator firstIter = firstCluster.constBegin(); firstIter != firstCluster.constEnd(); ++firstIter)
        for(QList<QString>::ConstIterator secondIter = secondCluster.constBegin(); secondIter != secondCluster.constEnd(); ++secondIter) {
            double currentSim = _compareWords(*firstIter, *secondIter);
            if(currentSim < sim && currentSim > 0)
                sim = currentSim;
    }
    return sim;
}
