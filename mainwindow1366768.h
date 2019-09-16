#ifndef MAINWINDOW1366768_H
#define MAINWINDOW1366768_H

#include <QWidget>

namespace Ui {
class mainwindow1366768;
}

class mainwindow1366768 : public QWidget
{
    Q_OBJECT

public:
    explicit mainwindow1366768(QWidget *parent = nullptr);
    ~mainwindow1366768();

private:
    Ui::mainwindow1366768 *ui;
};

#endif // MAINWINDOW1366768_H
