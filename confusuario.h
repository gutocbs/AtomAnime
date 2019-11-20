#ifndef CONFUSUARIO_H
#define CONFUSUARIO_H

#include <QDebug>
#include <QObject>
#include <QVector>
#include <QDir> //Buscar diretorios em busca de pastas de animes
#include <QDirIterator> //Iterar diretorios de animes
#include <QMap> //Salva e lê as pastas especificas de cada anime
#include <QThread>

#include "anime.h"
#include "anitomy/anitomy.h"
#include "leitorlistaanimes.h"
#include "janeladeconfig.h"

class confUsuario : public QObject
{
    Q_OBJECT
public:
    explicit confUsuario(QObject *parent = nullptr);
    QString fsimplificaNomeArquivo(QString);
    QString fretornaDiretorioEspecifico(int);
    QVector<QString> fretornaDiretoriosAnimes();
    void fbuscaDiretoriosAnimes();
    void frecebeListaAnime(leitorlistaanimes*);
    void frecebeConfigs(QStringList);
    void fmostraPastas();
    void fbuscaPastasThread(QThread &dThread);
    void fsalvaPastasArquivos();
    void flePastasArquivos();

public slots:
    void fsetupListasPraBusca();

signals:
    void schecouPastas();

private:
    QVector<anime*> vlistaAnimes;
    janeladeconfig *jconfig;
    QPointer<leitorlistaanimes> cleitorlistaanimes;

    QVector<QString> vdiretorioAnimes;

    QMap<QString, QString> vdiretorioEspecificoAnime;

    int vlista;
    bool vterminouChecagem;
};

#endif // CONFUSUARIO_H
