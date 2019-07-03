#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();

   QString savePath;
    double minWater;

    double VS1_vsmode;
    double VS2_vsmode;
    double range_vsmode;

    double flow_jingdu;
    double threshold1;
    double threshold2;
    double range1;
    double range2;
    double range3;
    double range4;


    void readValueToWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Setting *ui;
};

#endif // SETTING_H
