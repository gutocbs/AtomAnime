#ifndef JANELACONFIGURACAO_H
#define JANELACONFIGURACAO_H

#include <QDialog>

namespace Ui {
class JanelaConfiguracao;
}

class JanelaConfiguracao : public QDialog
{
    Q_OBJECT

public:
    explicit JanelaConfiguracao(QWidget *parent = nullptr);
    ~JanelaConfiguracao();

private:
    Ui::JanelaConfiguracao *ui;
};

#endif // JANELACONFIGURACAO_H
