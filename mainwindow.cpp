#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEvent>

/* MainWindow(QWidget *parent)
 * Author: 		Priadi
 * Function: 	MainWindow Class Contructor
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(setBatteryBar()));
    timer->start(100);

    // Left Plot Initialization
    ui->leftPlot->addGraph(); // blue line
    ui->leftPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->leftPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    ui->leftPlot->xAxis->setTicker(timeTicker);
    ui->leftPlot->axisRect()->setupFullAxesBox();
    ui->leftPlot->yAxis->setRange(-1.2, 1.2);

    // Right Plot Initialization
    ui->rightPlot->addGraph(); // blue line
    ui->rightPlot->graph(0)->setPen(QPen(Qt::blue)); // line color red for first graph
    ui->rightPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent red

    ui->rightPlot->xAxis->setTicker(timeTicker);
    ui->rightPlot->axisRect()->setupFullAxesBox();
    ui->rightPlot->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->rightPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->rightPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->rightPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->rightPlot->yAxis2, SLOT(setRange(QCPRange)));

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->leftPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->leftPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->leftPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->leftPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::createLeftPlot and createRightPlot
    QTimer *dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(createLeftPlot()));
    dataTimer->start(100); // Interval 0 means to refresh as fast as possible

    QTimer *dataTimer2 = new QTimer(this);
    connect(dataTimer2, SIGNAL(timeout()), this, SLOT(createRightPlot()));
    dataTimer2->start(100); // Interval 0 means to refresh as fast as possible

    // Compass Widget
    createCompass();

    // setup a timer that repeatedly update compass
    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(setCompassWidget()));
    timer2->start(100);

    // setting up Attitude Indicator Widget
    createAttitudeMeter();

    // setup a timer that repeatedly update compass
    QTimer *timer3 = new QTimer(this);
    connect(timer3, SIGNAL(timeout()), this, SLOT(setAttitude()));

    // setup a timer that repeatedly update temperature
    connect(timer3, SIGNAL(timeout()), this, SLOT(setTemperature()));

    // setup timer that repeatedly update mode
    connect(timer3, SIGNAL(timeout()), this, SLOT(setMode()));

    // setup timer that repeatedly update motor
    connect(timer3, SIGNAL(timeout()), this, SLOT(setMotorBar()));

    // setup timer that repeatedly update Roll Pitch Yaw Lift
    connect(timer3, SIGNAL(timeout()), this, SLOT(setRPYLText()));

    timer3->start(100);

    // load thermometer image
    QPixmap pic_thermometer("/home/priaditeguh/GUI/Tequila-Drone-GUI/temperature.png");
    int w = ui->label_thermometer->width();
    int h = ui->label_thermometer->height();
    ui->label_thermometer->setPixmap(pic_thermometer.scaled(w,h,Qt::KeepAspectRatio));

    // load Group Name
    QPixmap pic_Name("/home/priaditeguh/GUI/Tequila-Drone-GUI/title.png");
    w = ui->label_groupName->width();
    h = ui->label_groupName->height();
    ui->label_groupName->setPixmap(pic_Name.scaled(w,h,Qt::KeepAspectRatio));

    // load Tequila Drone Image
    QPixmap pic_Log("/home/priaditeguh/GUI/Tequila-Drone-GUI/topLogo.png");
    w = ui->label_Logo->width();
    h = ui->label_Logo->height();
    ui->label_Logo->setPixmap(pic_Log.scaled(w,h,Qt::KeepAspectRatio));

    // set up Redis Connection
    setRedisConnection();

    // ComboBox LeftPlot Init
    ui->comboBox_PlotLeft->addItem("Motor1");
    ui->comboBox_PlotLeft->addItem("Motor2");
    ui->comboBox_PlotLeft->addItem("Motor3");
    ui->comboBox_PlotLeft->addItem("Motor4");
    ui->comboBox_PlotLeft->addItem("Phi");
    ui->comboBox_PlotLeft->addItem("Theta");
    ui->comboBox_PlotLeft->addItem("Psi");
    ui->comboBox_PlotLeft->addItem("GyroX");
    ui->comboBox_PlotLeft->addItem("GyroY");
    ui->comboBox_PlotLeft->addItem("GyroZ");
    ui->comboBox_PlotLeft->addItem("AccX");
    ui->comboBox_PlotLeft->addItem("AccY");
    ui->comboBox_PlotLeft->addItem("AccZ");
    ui->comboBox_PlotLeft->addItem("Battery");
    ui->comboBox_PlotLeft->addItem("Temperature");
    ui->comboBox_PlotLeft->addItem("Pressure");

    // ComboBox RightPlot Init
    ui->comboBox_PlotRight->addItem("Motor1");
    ui->comboBox_PlotRight->addItem("Motor2");
    ui->comboBox_PlotRight->addItem("Motor3");
    ui->comboBox_PlotRight->addItem("Motor4");
    ui->comboBox_PlotRight->addItem("Phi");
    ui->comboBox_PlotRight->addItem("Theta");
    ui->comboBox_PlotRight->addItem("Psi");
    ui->comboBox_PlotRight->addItem("GyroX");
    ui->comboBox_PlotRight->addItem("GyroY");
    ui->comboBox_PlotRight->addItem("GyroZ");
    ui->comboBox_PlotRight->addItem("AccX");
    ui->comboBox_PlotRight->addItem("AccY");
    ui->comboBox_PlotRight->addItem("AccZ");
    ui->comboBox_PlotRight->addItem("Battery");
    ui->comboBox_PlotRight->addItem("Temperature");
    ui->comboBox_PlotRight->addItem("Pressure");

    // change engine progress bar color
    QPalette p = palette();
    p.setColor(QPalette::Highlight, Qt::darkGreen);
    ui->progressBar_motor1->setPalette(p);
    ui->progressBar_motor2->setPalette(p);
    ui->progressBar_motor3->setPalette(p);
    ui->progressBar_motor4->setPalette(p);

    // intall EventFilter used for keyboard input
    qApp->installEventFilter(this);
}

/* ~MainWindow()
 * Author: 		Priadi
 * Function: 	MainWindow class destructor
*/
MainWindow::~MainWindow()
{
    /* Delete all the keys of all the existing databases */
    //redisReply* reply = (redisReply*)redisCommand(context,"FLUSHALL");

    /* Disconnects and frees the context */
    redisFree(context);

    delete ui;
}

/* getBatteryVal()
 * Author: 		Priadi
 * Function: 	Get updated battery value from local database
 * OUT:         Scaling the input into 0V~12V
*/
float MainWindow::getBatteryVal()
{
    float value = 0;
    int val;

    redisReply* reply = (redisReply*)redisCommand(context,"GET BATT");

    if (reply->type != REDIS_REPLY_NIL) {
       val = stoi(reply->str);
    } else {
        val = 0;
    }

    freeReplyObject(reply);

    value = val/1000;
    if (value > 12)
        value = 12;
    else if (value < 0)
        value = 0;

    return value;
}

/* setBatteryBar()
 * Author: 		Priadi
 * Function: 	set the battery bar according battery voltage
*/
void MainWindow::setBatteryBar()
{
    static QTime time(QTime::currentTime());

    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    float valueBar = 0;
    QPalette p = palette();

    if (key-lastPointKey > 0.1) // at most add point every 0.1 s
    {
      // change progressbar value
      valueBar = getBatteryVal()*100 / 12.0;

      if (valueBar < 87.5)
      {
          p.setColor(QPalette::Highlight, Qt::red);
          ui->batteryBar->setPalette(p);
      } else {
          p.setColor(QPalette::Highlight, Qt::blue);
          ui->batteryBar->setPalette(p);
      }

      ui->batteryBar->setValue(valueBar);
      lastPointKey = key;
    }
}

/* getBatteryVal()
 * Author: 		Priadi
 * Function: 	Initializing up the PSI widget/animation)
*/
void MainWindow::createCompass()
{
    mCompassGauge = new QcGaugeWidget;

     mCompassGauge->addBackground(99);
     QcBackgroundItem *bkg1 = mCompassGauge->addBackground(92);
     bkg1->clearrColors();
     bkg1->addColor(0.1,Qt::black);
     bkg1->addColor(1.0,Qt::white);

     QcBackgroundItem *bkg2 = mCompassGauge->addBackground(88);
     bkg2->clearrColors();
     bkg2->addColor(0.1,Qt::white);
     bkg2->addColor(1.0,Qt::black);

     QcLabelItem *w = mCompassGauge->addLabel(80);
     w->setText("W");
     w->setAngle(0);
     w->setColor(Qt::white);

     QcLabelItem *n = mCompassGauge->addLabel(80);
     n->setText("N");
     n->setAngle(90);
     n->setColor(Qt::white);

     QcLabelItem *e = mCompassGauge->addLabel(80);
     e->setText("E");
     e->setAngle(180);
     e->setColor(Qt::white);

     QcLabelItem *s = mCompassGauge->addLabel(80);
     s->setText("S");
     s->setAngle(270);
     s->setColor(Qt::white);

     QcDegreesItem *deg = mCompassGauge->addDegrees(70);
     deg->setStep(5);
     deg->setMaxDegree(270);
     deg->setMinDegree(-75);
     deg->setColor(Qt::white);
     mCompassNeedle = mCompassGauge->addNeedle(60);
     mCompassNeedle->setNeedle(QcNeedleItem::CompassNeedle);
     mCompassNeedle->setValueRange(0,360);
     mCompassNeedle->setMaxDegree(360);
     mCompassNeedle->setMinDegree(0);
     mCompassGauge->addBackground(7);
     mCompassGauge->addGlass(88);

     ui->verticalLayout->addWidget(mCompassGauge);
}

/* getPsiVal()
 * Author: 		Priadi
 * Function: 	Getting the PSI value from local database
 * OUT:         psi angle
*/
float MainWindow::getPsiVal()
{
    float valCompass = 0;
    redisReply* reply = (redisReply*)redisCommand(context,"GET PSI");

    if (reply->type != REDIS_REPLY_NIL) {
       valCompass = stoi(reply->str);
    } else {
       valCompass = 0;
    }

    freeReplyObject(reply);

    valCompass = valCompass/32767 * 180;

    // -180 to 0 is made into positive
    if (valCompass < 0)
        valCompass = valCompass + 360;

    return (360 - valCompass);
}

/* void MainWindow::setCompassWidget()
 * Author: 		Priadi
 * Function: 	Updating the PSI widget
*/
void MainWindow::setCompassWidget()
{
    static QTime time(QTime::currentTime());

    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    float valueCompass = 0;

    if (key-lastPointKey > 0.1) // at most add point every 100 ms
    {
        valueCompass = getPsiVal();

        // to make zero position in north
        valueCompass += 90;
        if (valueCompass > 360)
            valueCompass -= 360;

        mCompassNeedle->setCurrentValue(valueCompass);
        lastPointKey = key;
    }
}

/* createAttitudeMeter()
 * Author: 		Priadi
 * Function: 	Initializing the widget or animation for PHI and THETA
*/
void MainWindow::createAttitudeMeter()
{
    mAttitudeGauge = new QcGaugeWidget;
    mAttitudeGauge->addBackground(99);
    QcBackgroundItem *bkg = mAttitudeGauge->addBackground(92);
    bkg->clearrColors();
    bkg->addColor(0.1,Qt::black);
    bkg->addColor(1.0,Qt::white);
    mAttMeter = mAttitudeGauge->addAttitudeMeter(88);

    mAttitudeNeedle = mAttitudeGauge->addNeedle(70);
    mAttitudeNeedle->setMinDegree(0);
    mAttitudeNeedle->setMaxDegree(180);
    mAttitudeNeedle->setValueRange(0,180);
    mAttitudeNeedle->setCurrentValue(90);
    mAttitudeNeedle->setColor(Qt::white);
    mAttitudeNeedle->setNeedle(QcNeedleItem::AttitudeMeterNeedle);
    mAttitudeGauge->addGlass(80);
    ui->verticalLayout_2->addWidget(mAttitudeGauge);
}

/* getPhiVal()
 * Author: 		Priadi
 * Function: 	Getting PHI value from local database
 * OUT:         PHI angle -180~180
*/
float MainWindow::getPhiVal()
{
    float valRoll = 0;
    redisReply* reply = (redisReply*)redisCommand(context,"GET PHI");

    if (reply->type != REDIS_REPLY_NIL) {
       valRoll = stoi(reply->str);
    } else {
       valRoll = 0;
    }

    freeReplyObject(reply);

    valRoll = valRoll/32767 * 180;

    return valRoll;
}

/* createAttitudeMeter()
 * Author: 		Priadi
 * Function: 	Getting THETA value from local database
 * OUT:         THETA angle -180~180
*/
float MainWindow::getThetaVal()
{
    float valPitch = 0;
    redisReply* reply = (redisReply*)redisCommand(context,"GET THETA");
    valPitch = stoi(reply->str);
    freeReplyObject(reply);

    valPitch = valPitch/32767 * 180;

    return valPitch;
}

/* createAttitudeMeter()
 * Author: 		Priadi
 * Function: 	Updating the Attitude Indicator based on PHI and THETA value
*/
void MainWindow::setAttitude()
{
    static QTime time(QTime::currentTime());

    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    float valuePitch = 0;
    float valueRoll = 0;

    if (key-lastPointKey > 0.1) // at most add point every 0.1 second
    {
        valuePitch = getThetaVal();
        valueRoll = getPhiVal();

        mAttMeter->setCurrentPitch(valuePitch);
        mAttMeter->setCurrentRoll(valueRoll);
        lastPointKey = key;
    }
}

/* createAttitudeMeter()
 * Author: 		Priadi
 * Function: 	Updating the Attitude Indicator based on PHI and THETA value
*/
float MainWindow::getTempVal()
{
    float valueTemp = 0.0;
    redisReply* reply = (redisReply*)redisCommand(context,"GET TEMP");

    if (reply->type != REDIS_REPLY_NIL) {
       valueTemp = stoi(reply->str);
    } else {
       valueTemp = 0;
    }

    freeReplyObject(reply);

    valueTemp = valueTemp / 100;

    return valueTemp;
}

/* setTemperature()
 * Author: 		Priadi
 * Function: 	Updating the temperature value
*/
void MainWindow::setTemperature()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    float valueTemp = 0.0;

    if (key-lastPointKey > 0.1) // at most add point every 0.1 seconds
    {
        valueTemp = getTempVal();

        QString printable = QString::number(valueTemp) + " C";
        ui->label_temp->setText(printable);

        lastPointKey = key;
    }
}

/* setMode()
 * Author: 		Priadi
 * Function: 	Updating board mode to display
*/
void MainWindow::setMode()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    int modeNumber = 0;
    QString modeName;

    if (key-lastPointKey > 0.1) // at most add point every 0.1
    {
        redisReply* reply = (redisReply*)redisCommand(context,"GET MODE");
        modeNumber = stoi(reply->str);
        freeReplyObject(reply);

        QString printable = QString::number(modeNumber);
        ui->label_modeNumber->setText(printable);
        switch(modeNumber)
        {
        case 0:
            modeName = "Safe Mode";
            break;
        case 1:
            modeName = "Panic Mode";
            break;
        case 2:
            modeName = "Manual Mode";
            break;
        case 3:
            modeName = "Calibration Mode";
            break;
        case 4:
            modeName = "Yaw Control Mode";
            break;
        case 5:
            modeName = "Full Control Mode";
            break;
        case 6:
            modeName = "Raw Mode";
            break;
        case 7:
            modeName = "Height Control Mode";
            break;
        case 8:
            modeName = "Wireless Mode";
            break;
        default:
            modeName = "Error";
            break;
        }

        ui->label_modeName->setText(modeName);
        lastPointKey = key;
    }
}

/* setMotorBar()
 * Author: 		Priadi
 * Function: 	Updating motor bar
*/
void MainWindow::setMotorBar()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    redisReply* reply;
    float value = 0;

    if (key-lastPointKey > 0.1) // at most add point every 1
    {
        reply = (redisReply*)redisCommand(context,"GET AE0");
        value = stoi(reply->str);
        freeReplyObject(reply);
        ui->progressBar_motor1->setValue(value);

        reply = (redisReply*)redisCommand(context,"GET AE1");
        value = stoi(reply->str);
        freeReplyObject(reply);
        ui->progressBar_motor2->setValue(value);

        reply = (redisReply*)redisCommand(context,"GET AE2");
        value = stoi(reply->str);
        freeReplyObject(reply);
        ui->progressBar_motor3->setValue(value);

        reply = (redisReply*)redisCommand(context,"GET AE3");
        value = stoi(reply->str);
        freeReplyObject(reply);
        ui->progressBar_motor4->setValue(value);

        lastPointKey = key;
    }
}

/* getRollVal()
 * Author: 		Priadi
 * Function: 	Getting ROLL Joystick value
*/
int MainWindow::getRollVal()
{
    int valRoll = 0;
    redisReply* reply = (redisReply*)redisCommand(context,"GET ROLL");

    if (reply->type != REDIS_REPLY_NIL) {
       valRoll = stoi(reply->str);
    } else {
       valRoll = 0;
    }

    freeReplyObject(reply);
    return valRoll;
}

/* getRollVal()
 * Author: 		Priadi
 * Function: 	Getting PITCH Joystick value
*/
int MainWindow::getPitchVal()
{
    int valPitch = 0;
    redisReply* reply = (redisReply*)redisCommand(context,"GET PITCH");

    if (reply->type != REDIS_REPLY_NIL) {
       valPitch = stoi(reply->str);
    } else {
       valPitch = 0;
    }

    freeReplyObject(reply);
    return valPitch;
}

/* getRollVal()
 * Author: 		Priadi
 * Function: 	Getting YAW Joystick value
*/
int MainWindow::getYawVal()
{
    int valYaw = 0;
    redisReply* reply = (redisReply*)redisCommand(context,"GET YAW");

    if (reply->type != REDIS_REPLY_NIL) {
       valYaw = stoi(reply->str);
    } else {
       valYaw = 0;
    }

    freeReplyObject(reply);
    return valYaw;
}

/* getRollVal()
 * Author: 		Priadi
 * Function: 	Getting LIFT Joystick value
*/
int MainWindow::getLiftVal()
{
    int valLift = 0;
    redisReply* reply = (redisReply*)redisCommand(context,"GET LIFT");

    if (reply->type != REDIS_REPLY_NIL) {
       valLift = stoi(reply->str);
    } else {
       valLift = 0;
    }

    freeReplyObject(reply);
    return valLift;
}

/* getRollVal()
 * Author: 		Priadi
 * Function: 	Displaying Joystick value
*/
void MainWindow::setRPYLText()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.1) // at most add point every 1
    {
        QString printable = "Roll: " + QString::number(getRollVal());
        printable += "              Pitch: " + QString::number(getPitchVal()) + "\n\n";
        printable += "Yaw: " + QString::number(getYawVal());
        printable += "              Lift: " + QString::number(getLiftVal()) + "\n";

        ui->label_RPYL->setText(printable);

        lastPointKey = key;
    }
}

/* createLeftPlot()
 * Author: 		Priadi
 * Function: 	Updating the upper plot based on the selected menu
*/
void MainWindow::createLeftPlot()
{
    static QTime time(QTime::currentTime());
    static double lastPointKey = 0;

    if(flag_reset_timeLeft){
        time.restart();
        lastPointKey = 0;
        flag_reset_timeLeft = false;
    }

    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    float value = 0 ;

    if (key-lastPointKey > 0.1) // at most add point every 100 ms
    {
        value = selectVarLeftPlot();

        ui->leftPlot->graph(0)->addData(key,value);

      // rescale value (vertical) axis to fit the current data:
      // ui->leftPlot->graph(0)->rescaleValueAxis(true);

      lastPointKey = key;
    }

    // make key axis range scroll with the data (at a constant range):
    ui->leftPlot->xAxis->setRange(key, 100, Qt::AlignRight);
    ui->leftPlot->replot();
}

/* createRightPlot()
 * Author: 		Priadi
 * Function: 	Updating the lower plot based on the selected menu
*/
void MainWindow::createRightPlot()
{
    static QTime time(QTime::currentTime());
    static double lastPointKey = 0;

    if(flag_reset_timeRight){
        time.restart();
        lastPointKey = 0;
        flag_reset_timeRight = false;
    }

    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    float value = 0 ;

    if (key-lastPointKey > 0.1) // at most add point every 100 ms
    {
        value = selectVarRightPlot();

        ui->rightPlot->graph(0)->addData(key,value);

      // rescale value (vertical) axis to fit the current data:
      // ui->leftPlot->graph(0)->rescaleValueAxis(true);

      lastPointKey = key;
    }

    // make key axis range scroll with the data (at a constant range):
    ui->rightPlot->xAxis->setRange(key, 100, Qt::AlignRight);
    ui->rightPlot->replot();
}

/* selectVarLeftPlot()
 * Author: 		Priadi
 * Function: 	Getting value from local database to update the upper plot
 * OUT:         various value depending on the key
*/
float MainWindow::selectVarLeftPlot()
{
    redisReply *reply;
    int idx;
    float value = 0;

    idx = ui->comboBox_PlotLeft->currentIndex();
    switch(idx)
    {
        case 0:
            reply = (redisReply*)redisCommand(context,"GET AE0");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->leftPlot->yAxis->setRange(-10,1050);
            break;
        case 1:
            reply = (redisReply*)redisCommand(context,"GET AE1");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->leftPlot->yAxis->setRange(-10,1050);
            break;
        case 2:
            reply = (redisReply*)redisCommand(context,"GET AE2");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->leftPlot->yAxis->setRange(-10,1050);
            break;
        case 3:
            reply = (redisReply*)redisCommand(context,"GET AE3");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->leftPlot->yAxis->setRange(-10,1050);
            break;
        case 4:
            value = getPhiVal();
            ui->leftPlot->yAxis->setRange(-180,180);
            break;
        case 5:
            value = getThetaVal();
            ui->leftPlot->yAxis->setRange(-180,180);
            break;
        case 6:
            value = getPsiVal();
            ui->leftPlot->yAxis->setRange(-5,370);
            break;
        case 7:
            reply = (redisReply*)redisCommand(context,"GET SP");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
             ui->leftPlot->yAxis->setRange(-32767,32767);
            break;
        case 8:
            reply = (redisReply*)redisCommand(context,"GET SQ");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->leftPlot->yAxis->setRange(-32767,32767);
            break;
        case 9:
            reply = (redisReply*)redisCommand(context,"GET SR");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
             ui->leftPlot->yAxis->setRange(-32767,32767);
            break;
        case 10:
            reply = (redisReply*)redisCommand(context,"GET SAX");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->leftPlot->yAxis->setRange(-32767,32767);
            break;
        case 11:
            reply = (redisReply*)redisCommand(context,"GET SAY");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->leftPlot->yAxis->setRange(-32767,32767);
            break;
        case 12:
            reply = (redisReply*)redisCommand(context,"GET SAZ");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->leftPlot->yAxis->setRange(-32767,32767);
            break;
        case 13:
            value = getBatteryVal();
            ui->leftPlot->yAxis->setRange(0,12);
            break;
        case 14:
            value = getTempVal();
            ui->leftPlot->yAxis->setRange(0,100);
            break;
        case 15:
            reply = (redisReply*)redisCommand(context,"GET PRESS");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->leftPlot->yAxis->setRange(-32767,32767);
            break;
        default:
            value = 0;
            break;
    }

    return value;
}

/* selectVarRightPlot()
 * Author: 		Priadi
 * Function: 	Getting value from local database to update the lower plot
 * OUT:         various value depending on the key
*/
float MainWindow::selectVarRightPlot()
{
    redisReply *reply;
    int idx;
    float value = 0;

    idx = ui->comboBox_PlotRight->currentIndex();
    switch(idx)
    {
        case 0:
            reply = (redisReply*)redisCommand(context,"GET AE0");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->rightPlot->yAxis->setRange(-10,1050);
            break;
        case 1:
            reply = (redisReply*)redisCommand(context,"GET AE1");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->rightPlot->yAxis->setRange(-10,1050);
            break;
        case 2:
            reply = (redisReply*)redisCommand(context,"GET AE2");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->rightPlot->yAxis->setRange(-10,1050);
            break;
        case 3:
            reply = (redisReply*)redisCommand(context,"GET AE3");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->rightPlot->yAxis->setRange(-10,1050);
            break;
        case 4:
            value = getPhiVal();
            ui->rightPlot->yAxis->setRange(-180,180);
            break;
        case 5:
            value = getThetaVal();
            ui->rightPlot->yAxis->setRange(-180,180);
            break;
        case 6:
            value = getPsiVal();
            ui->rightPlot->yAxis->setRange(-5,370);
            break;
        case 7:
            reply = (redisReply*)redisCommand(context,"GET SP");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
             ui->rightPlot->yAxis->setRange(-32767,32767);
            break;
        case 8:
            reply = (redisReply*)redisCommand(context,"GET SQ");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->rightPlot->yAxis->setRange(-32767,32767);
            break;
        case 9:
            reply = (redisReply*)redisCommand(context,"GET SR");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
             ui->rightPlot->yAxis->setRange(-32767,32767);
            break;
        case 10:
            reply = (redisReply*)redisCommand(context,"GET SAX");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->rightPlot->yAxis->setRange(-32767,32767);
            break;
        case 11:
            reply = (redisReply*)redisCommand(context,"GET SAY");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->rightPlot->yAxis->setRange(-32767,32767);
            break;
        case 12:
            reply = (redisReply*)redisCommand(context,"GET SAZ");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->rightPlot->yAxis->setRange(-32767,32767);
            break;
        case 13:
            value = getBatteryVal();
            ui->rightPlot->yAxis->setRange(0,12);
            break;
        case 14:
            value = getTempVal();
            ui->rightPlot->yAxis->setRange(0,100);
            break;
        case 15:
            reply = (redisReply*)redisCommand(context,"GET PRESS");
            if (reply->type != REDIS_REPLY_NIL) {
               value = stoi(reply->str);
            } else {
               value = 0;
            }
            freeReplyObject(reply);
            ui->rightPlot->yAxis->setRange(-32767,32767);
            break;
        default:
            value = 0;
            break;
    }

    return value;
}

/* on_pushButton_savePlotLeft_clicked()
 * Author: 		Priadi
 * Function: 	Saving the upper plot as png image file
*/
void MainWindow::on_pushButton_savePlotLeft_clicked()
{
    static int i = 0;

    QString fileName("/home/priaditeguh/GUI/Tequila-Drone-GUI/");
    fileName = fileName + ui->comboBox_PlotLeft->currentText() + "LeftPlot"+ QString::number(i) + ".png";
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << file.errorString();
    } else {
        ui->leftPlot->savePng(fileName);
    }

    i++;
}

/* on_pushButton_savePlotRight_clicked()
 * Author: 		Priadi
 * Function: 	Saving the lower plot as png image file
*/
void MainWindow::on_pushButton_savePlotRight_clicked()
{
    static int i = 0;

    QString fileName("/home/priaditeguh/GUI/Tequila-Drone-GUI/");
    fileName = fileName + ui->comboBox_PlotLeft->currentText() + "RightPlot"+ QString::number(i) + ".png";
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << file.errorString();
    } else {
        ui->leftPlot->savePng(fileName);
    }

    i++;
}

/* on_comboBox_PlotLeft_currentIndexChanged()
 * Author: 		Priadi
 * Function: 	Selecting data to display in upper plot
*/
void MainWindow::on_comboBox_PlotLeft_currentIndexChanged(int index)
{
    ui->leftPlot->graph(0)->data()->clear();
    ui->leftPlot->replot();
    flag_reset_timeLeft = true;
}

/* on_comboBox_PlotRight_currentIndexChanged()
 * Author: 		Priadi
 * Function: 	Selecting data to display in upper plot
*/
void MainWindow::on_comboBox_PlotRight_currentIndexChanged(int index)
{
    ui->rightPlot->graph(0)->data()->clear();
    ui->rightPlot->replot();
    flag_reset_timeRight = true;
}


/* setRedisConnection()
 * Author: 		Priadi
 * Function: 	Initializing the connection with local database
*/
void MainWindow::setRedisConnection()
{
    const char *hostname = "127.0.0.1";
    int port = 6379;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    context = redisConnectWithTimeout(hostname, port, timeout);
    if (context == NULL || context->err) {
        if (context) {
            printf("Connection error: %s\n", context->errstr);
            redisFree(context);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }
}

/* eventFilter(QObject *watched, QEvent *event)
 * Author: 		Priadi
 * Function: 	keypress event to receive input from keyboard
*/
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        redisReply* reply;
        int c = keyEvent->key();

        switch(c)
        {
        case Qt::Key_A:
            ui->textEdit_dspKeyB->setText("Lift Up");
            break;
        case Qt::Key_Z:
            ui->textEdit_dspKeyB->setText("Lift Down");
            break;
        case Qt::Key_Left:
            ui->textEdit_dspKeyB->setText("Roll Down");
            break;
        case Qt::Key_Right:
            ui->textEdit_dspKeyB->setText("Roll Up");
            break;
        case Qt::Key_Up:
            ui->textEdit_dspKeyB->setText("Pitch Down");
            break;
        case Qt::Key_Down:
            ui->textEdit_dspKeyB->setText("Pitch Up");
            break;
        case Qt::Key_Q:
            ui->textEdit_dspKeyB->setText("Yaw Down");
            break;
        case Qt::Key_W:
            ui->textEdit_dspKeyB->setText("Yaw Up");
            break;

        case Qt::Key_U:
            ui->textEdit_dspKeyB->setText("Pyaw Up");
            break;
        case Qt::Key_J:
            ui->textEdit_dspKeyB->setText("Pyaw Down");
            break;
        case Qt::Key_I:
            ui->textEdit_dspKeyB->setText("P1 Up");
            break;
        case Qt::Key_K:
            ui->textEdit_dspKeyB->setText("P2 Down");
            break;
        case Qt::Key_O:
            ui->textEdit_dspKeyB->setText("P2 Up");
            break;
        case Qt::Key_L:
            ui->textEdit_dspKeyB->setText("P2 Down");
            break;

        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
            ui->textEdit_dspKeyB->setText("Change State");
            break;

        case Qt::Key_Period:
            ui->textEdit_dspKeyB->setText("Ping board");
            break;

        case Qt::Key_Escape:
            ui->textEdit_dspKeyB->setText("Exit Application");
            this->close();
            break;

        case Qt::Key_B:
            ui->textEdit_dspKeyB->setText("Start Logging, wait...");
            break;
        case Qt::Key_N:
            ui->textEdit_dspKeyB->setText("Send Logging data to PC, wait...");
            break;
        default:
            break;
        }
        if(c>=65 && c<=90)
            c+=32; // convert capital letter to small letter

        reply = (redisReply*)redisCommand(context,"SET KEYBOARD %d",c);
        freeReplyObject(reply);
        reply = (redisReply*)redisCommand(context,"PEXPIRE KEYBOARD 100"); // expire in 0.1 second
        freeReplyObject(reply);
    }
    return QObject::eventFilter(watched, event);
}
