#ifndef ANIME_H
#define ANIME_H

#include <QObject>
#include <QStringList>
#include <QDate> //Pega data de estreia e season

class anime : public QObject
{
    Q_OBJECT
public:
    explicit anime(QObject *parent = nullptr);
    bool operator<(const anime& other) const {
            return vnome < other.vnome; // sort by namefile
    }
    QString vnome;
    QString vnomeIngles;
    QString vnomeJapones;
    QStringList vnomeAlternativo; //StringList por que pode ter vários nomes alternativos
    QString vnumEpisodiosTotais;
    QString vnumEpisodiosAssistidos;
    QString vnotaMediaSite;
    QString vLinkImagemMedia;
    QString vnotaMediaPessoal;
    QString vstatus;
    QString vsinopse;
    QString vid;
    QString vseason;
    QString vlista; //Esse atributo será usado para a função de busca e para função de mudar a lista do anime
    QDate vdataEstreia;
    bool vfinalizado;
    //Adicionar mais depois

signals:

public slots:
};

#endif // ANIME_H
