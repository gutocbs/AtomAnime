#ifndef TORRENTINFO_H
#define TORRENTINFO_H

#include <QCheckBox> //Pra poder por as caixas e tickar elas
#include <QObject>

class torrentinfo : public QObject {
  Q_OBJECT
public:
  explicit torrentinfo(QObject *parent = nullptr);
  QString vnomeTorrent;
  QString vnomeAnime;
  QString vlinkTorrent;
  QString vdescricaoTorrent;
  QString vepisodioAnime;
  QString vfansub;
  QString vresolucao;
  QString vsize;
  QString vtorrentInfoLink;
  QCheckBox vbox;
signals:

public slots:
};

#endif // TORRENTINFO_H
