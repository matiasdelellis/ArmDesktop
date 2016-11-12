#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString serialPortName;
    bool serialIsAvailable;

    ui->setupUi(this);

    serial = new QSerialPort(this);

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if (serialPortInfo.vendorIdentifier() == arduinoVendorId)
            {
                serialPortName = serialPortInfo.portName();
                serialIsAvailable = true;
            }
        }
    }

    if (serialIsAvailable)
    {
        serial->setPortName(serialPortName);
        serial->open(QSerialPort::WriteOnly);
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(readSerial()));

        ui->statusBar->showMessage("Connected on " + serialPortName);
    }
    else
    {
        QMessageBox *warn = new QMessageBox(QMessageBox::Warning, "Serial", "Couldn't find the serial port!", QMessageBox::Ok);
        warn->show();
        ui->statusBar->showMessage(tr("Disconnected"));
        this->setDisabled(true);
    }
}

MainWindow::~MainWindow()
{
    if (serial->isOpen())
    {
        serial->close();
    }

    delete ui;
    delete serial;
}

void MainWindow::readSerial()
{
    if (serial->canReadLine())
        QByteArray bytes = serial->readAll();

    //this->setDisabled(false);
}

void MainWindow::on_sliderElbow_sliderMoved(int position)
{
    QString value = QString::number(position);
    QByteArray valueArray = value.toLatin1();

    this->setDisabled(true);

    serial->write("E:");
    serial->write(valueArray);
}

void MainWindow::on_sliderGripper_sliderMoved(int position)
{
    QString value = QString::number(position);
    QByteArray valueArray = value.toLatin1();

    this->setDisabled(true);

    serial->write("G:");
    serial->write(valueArray);
}

void MainWindow::on_pushMotor_pressed()
{
    serial->write("M:1");
}

void MainWindow::on_pushMotor_released()
{
    serial->write("M:0");
}

void MainWindow::on_checkLighting_toggled(bool checked)
{
    if (checked == true)
        serial->write("L:1");
    else
        serial->write("L:0");
}

void MainWindow::on_sliderShoulder_valueChanged(int value)
{
    QString value2 = QString::number(value);
    QByteArray valueArray = value2.toLatin1();

    //this->setDisabled(true);

    serial->write("S");
    serial->write(valueArray);
}

void MainWindow::on_dialBase_valueChanged(int value)
{
    QString value2 = QString::number(value);
    QByteArray valueArray = value2.toLatin1();

    //this->setDisabled(true);

    serial->write("B");
    serial->write(valueArray);
}
