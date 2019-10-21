#ifndef JANELADECONFIG_H
#define JANELADECONFIG_H

#include <QWidget>

namespace Ui {
class janeladeconfig;
}

class janeladeconfig : public QWidget
{
    Q_OBJECT

public:
    explicit janeladeconfig(QWidget *parent = nullptr);
    ~janeladeconfig();

private:
    Ui::janeladeconfig *ui;
};

#endif // JANELADECONFIG_H
