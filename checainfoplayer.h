#ifndef CHECAINFOPLAYER_H
#define CHECAINFOPLAYER_H

#include "janeladeconfig.h"
#include <QObject>

class ChecaInfoPlayer : public QObject {
  Q_OBJECT
public:
  explicit ChecaInfoPlayer(QObject *parent = nullptr);
  void fgetStreamLanguages();
  void fchecaStream(const QString &player, const QString &janela);
  void fremoveReprodutor();

  void fchecaCrunchyroll();
  void fchecaAnimeLab();
  void fchecaFunimation();
  void fchecaKissAnime();
  void fchecaWinamp();
  void fchecaLocalPlayer();

  QString fretornaAnime();
  QString fretornaEpisodio();
signals:

private:
  QString vanime;
  QString vepisodio;
  QString vnomeDaJanela;
  QStringList vStreamLanguages;
};

#endif // CHECAINFOPLAYER_H
