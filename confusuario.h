#ifndef CONFUSUARIO_H
#define CONFUSUARIO_H

#include <QDebug>
#include <QObject>
#include <QVector>
#include <QDir> //Buscar diretorios em busca de pastas de animes
#include <QDirIterator> //Iterar diretorios de animes
#include <QMap> //Salva e lê as pastas especificas de cada anime

#include "anime.h"
#include "anitomy/anitomy.h"

class confUsuario : public QObject
{
    Q_OBJECT
public:
    explicit confUsuario(QObject *parent = nullptr);
    QString fsimplificaNomeArquivo(QString);
    QString fretornaDiretorioEspecifico(int);
    QVector<QString> fretornaDiretoriosAnimes();
    void fbuscaDiretoriosAnimes();
    void frecebeListaAnime(QVector<anime*>);

signals:

public slots:

private:
    QVector<anime*> vlistaAnimes;

    QVector<QString> vdiretorioAnimes;

    QMap<QString, QString> vdiretorioEspecificoAnime;
};

#endif // CONFUSUARIO_H
