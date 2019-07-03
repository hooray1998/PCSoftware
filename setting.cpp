#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::on_pushButton_clicked(){
    savePath = ui->lineEdit->text();
    minWater = ui->doubleSpinBox_4->value();

    VS1_vsmode = ui->doubleSpinBox->value();
    VS2_vsmode = ui->doubleSpinBox_2->value();
    range_vsmode = ui->doubleSpinBox_3->value();

    flow_jingdu = ui->doubleSpinBox_12->value();
    threshold1 = ui->doubleSpinBox_9->value();
    threshold2 = ui->doubleSpinBox_10->value();
    range1 = ui->doubleSpinBox_5->value();
    range2 = ui->doubleSpinBox_6->value();
    range3 = ui->doubleSpinBox_7->value();
    range4 = ui->doubleSpinBox_8->value();

	this->hide();
}

void Setting::readValueToWidget(){
    ui->lineEdit->setText(savePath);
	ui->doubleSpinBox_4->setValue(minWater);
	ui->doubleSpinBox->setValue(VS1_vsmode);
	ui->doubleSpinBox_2->setValue(VS2_vsmode);
	ui->doubleSpinBox_3->setValue(range_vsmode);
	ui->doubleSpinBox_12->setValue(flow_jingdu);
	ui->doubleSpinBox_9->setValue(threshold1);
	ui->doubleSpinBox_10->setValue(threshold2);
	ui->doubleSpinBox_5->setValue(range1);
	ui->doubleSpinBox_6->setValue(range2);
	ui->doubleSpinBox_7->setValue(range3);
	ui->doubleSpinBox_8->setValue(range4);
}
