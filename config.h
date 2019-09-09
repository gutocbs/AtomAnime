#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>
#include "configpc.h"

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    ~Config();
    void IniciaThread(QThread &cThread);

    void ParseTaiga();
    void ParseCompleted();
    void ParseOnHold();
    void ParseDropped();
    void ParsePlanToWatch();

    void organizaArquivo();

    QString arrumaSeason(QString);

private:
    configPC *pastas;

        //Reescrevendo função
        QString qId;
        QString qProgresso;
        QString qEpiCount;
        QString qScore;
        QString qImagem;
        QString qGenero;
        QString qNome;
        QString qNomeIngles;
        QString qSinopse;
        QString qProdutores;
        QString qScoreGeral;
        QString qSeason;
        bool qFinished;
        bool qWatching;
        bool qAnimeId;
        bool qPrimeiraEntrada;

signals:
    void terminouSetArquivo();
    void terminouCompleted();
    void terminouOnHold();
    void terminouDropped();
    void terminouPlanToWatch();
    void terminouWatching();
public slots:
    void run();
};

#endif // CONFIG_H
