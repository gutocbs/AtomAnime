#ifndef JANELACONFIGURACAO_H
#define JANELACONFIGURACAO_H

#include <QDialog>
#include <QDir>
#include <QVector>
#include <QDebug>
#include <QFileDialog>

namespace Ui {
class JanelaConfiguracao;
}

class JanelaConfiguracao : public QDialog
{
    Q_OBJECT

public:
    explicit JanelaConfiguracao(QWidget *parent = nullptr);
    ~JanelaConfiguracao();
    void setDiretoriosAnimes();
    void leArquivoConf();
    QVector<QString> retornaDiretorioAnime();
    QString returnUserAnilist();
    QString returnOrdemLista();
    QString returnRSS();
private:
    Ui::JanelaConfiguracao *ui;
    QVector<QString> diretorioAnime;
    QString userAnilist;
    QString ordem;
    QString rss;

signals:
    void save();
    void cancelado();
    void user();
    void dirAdd(QString);
    void dirRem(QString);
public slots:
    void mensagem(QString);
private slots:
    void on_BotaoCancelar_clicked();
    void on_DiretorioAnimesAdiciona_clicked();
    void on_BotaoAutorizar_clicked();
    void on_DiretorioAnimesRemove_clicked();
};

#endif // JANELACONFIGURACAO_H
