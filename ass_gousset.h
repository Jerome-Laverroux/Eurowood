#ifndef ASS_GOUSSET_H
#define ASS_GOUSSET_H

#include <QWidget>
#include "ui_ass_gousset.h"
#include "QMessageBox"
#include "qmath.h"
#include "QDebug"
#include "libeurocod2.h"

namespace Ui {
class Ass_gousset;
}

class Ass_gousset : public QWidget
{
    Q_OBJECT

public:
    explicit Ass_gousset(QWidget *parent = 0);
    ~Ass_gousset();

private slots:
    void on_Calculer_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::Ass_gousset *ui;
    QString cast(double d);
    double arrondi(double d);
};

#endif // ASS_GOUSSET_H
