#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <QDebug>
#include <QObject>
#include <QPointer>
#include <QDir>
#include <QDirIterator>//Itera a pasta
#include <QFile> // Ler arquivos na pasta
#include <QDesktopServices> //Abre episódio
#include <QProcess> //Tentando abrir o epi por aqui
#include <QUrl> //Também é usado pra abrir o episódio
#include <QLoggingCategory> //Cria classe de log

#include "anime.h"
#include "confusuario.h" //Retorna os diretorios de animes
#include "logger.h" //Mensagens de log customizadas
#include "anitomy/anitomy.h"

class arquivos : public QObject
{
    Q_OBJECT
public:
    explicit arquivos(QObject *parent = nullptr);
    QString fprocuraEpisodio(anime*);
    QString fprocuraEpisodioEspecifico(anime*, int);
    QString fremoveCaracteresDiferentes(QString);
    bool fcomparaDadosAnime(QString, QString, QString, QStringList, int, int);
    bool fabreEpisodio(QByteArray);
    void frecebeDiretorios(confUsuario*);
    void frecebeAnimes(leitorlistaanimes*);
    int fcomparaSeasons(QString, int, int);

public slots:

private:
    QPointer<confUsuario> cconfUsuario;
    QPointer<leitorlistaanimes> cleitorlistaanimes;
    QVector<anime*> vlistaSelecionada;
};

#endif // ARQUIVOS_H
