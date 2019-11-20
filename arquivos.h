#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <QDebug>
#include <QObject>
#include <QPointer>
#include <QDir>
#include <QDirIterator>//Itera a pasta
#include <QFile> // Ler arquivos na pasta
#include <QDesktopServices> //Abre episódio
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
    QString fprocuraUltimoEpisodio(anime*, QString);
    QString fremoveCaracteresDiferentes(QString);
    bool fcomparaDadosAnime(QString, QString, QString, QStringList, int);
    bool fabreEpisodio(QString);
    void frecebeDiretorios(confUsuario*);
signals:

public slots:

private:
    confUsuario *cconfUsuario;
};

#endif // ARQUIVOS_H
