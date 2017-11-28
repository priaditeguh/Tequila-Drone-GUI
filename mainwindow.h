#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcgaugewidget.h"
#include <hiredis/qt.h>

#include <QTimer>
#include <QPixmap>

#include <iostream>
using namespace std;
#include <QCoreApplication>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void setBatteryBar();
    void createLeftPlot();
    void createRightPlot();

    void createCompass();
    void setCompassWidget();
    void createAttitudeMeter();
    void setAttitude();
    void setTemperature();

    void setRedisConnection();

    void setMode();

    float selectVarLeftPlot();
    float selectVarRightPlot();

    void setMotorBar();

    float getBatteryVal();
    float getTempVal();
    float getPsiVal();
    float getPhiVal();
    float getThetaVal();
    void on_pushButton_savePlotLeft_clicked();

    void on_comboBox_PlotLeft_currentIndexChanged(int index);

    void setRPYLText();
    int getPitchVal();
    int getYawVal();
    int getLiftVal();
    int getRollVal();

    void on_pushButton_savePlotRight_clicked();

    void on_comboBox_PlotRight_currentIndexChanged(int index);

private:
    redisContext *context;
    bool flag_reset_timeLeft = false;
    bool flag_reset_timeRight = false;

    Ui::MainWindow *ui;

    QcGaugeWidget * mCompassGauge;
    QcNeedleItem *mCompassNeedle;

    QcGaugeWidget * mAttitudeGauge;
    QcNeedleItem * mAttitudeNeedle;
    QcAttitudeMeter *mAttMeter;
};

#endif // MAINWINDOW_H
