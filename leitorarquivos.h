#ifndef LEITORARQUIVOS_H
#define LEITORARQUIVOS_H

#include <QString>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <string>
#include <QDebug>
#include <QRegularExpression>
#include <QDate>
//#include "organizador.h"

class leitorarquivos
{
public:

    leitorarquivos();
    ~leitorarquivos();

    void abreEpisodio(int);
    void Script();
    void leLinha(QString);
    void arrumaProgresso(int);
    void arrumaSinopse(int);

    QString imagem(int, QString);
    QString imagemBig(int);
    QString retornaNome(int);
    QString retornaId(int);
    QString retornaLink(int);
    QString retornaNumEpi(int);
    QString retornaSinopse(int);
    QString retornaDiretorio(int);
    QString retornaNomeIngles(int);
    QString retornaSeason(int);
    QString retornaNotaGeral(int);
    QString retornaStatus(int);
    QString retornaNumEpisodiosLancados(int);

    QStringList retornaProdutores(int);
    QStringList retornaGeneros(int);

    int retornaTamanhoLista();

    int retornaProgresso(int);
    int setProgress(int);
    int retornaScore(int);
    int busca(QString);

public slots:

    void OrdenaVetor(QString);
    void OrdenaVetorNome();
    void OrdenaVetorNota();
    void OrdenaVetorProgresso();
    void OrdenaVetorData();

    void OrdenaVetorNomeInverso();
    void OrdenaVetorNotaInverso();
    void OrdenaVetorProgressoInverso();
    void OrdenaVetorDataInverso();

private:

    QVector<QString> nomeAnime;
    QVector<QString> nomeIngles;
    QVector<QString> progresso;
    QVector<QString> numEpi;
    QVector<QString> score;
    QVector<QString> scoreGeral;
    QVector<QString> season;
    QVector<QString> date;
    QVector<QString> status;
    QVector<QString> link;
    QVector<QString> sinopse;
    QVector<QString> id;
    QVector<QStringList> produtores;
    QVector<QStringList> genero;
    QVector<QString> diretorio;
    int NomeLista;
};

#endif // LEITORARQUIVOS_H
