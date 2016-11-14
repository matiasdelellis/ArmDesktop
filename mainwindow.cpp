#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>
#include <QtWidgets>

/*
 * Main Window
 */
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
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->open(QSerialPort::ReadWrite);

        QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(readSerial()));
        QObject::connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(onSerialPortError(QSerialPort::SerialPortError)));

        // Apply settings
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

/*
 * Serial handlers.
 */
void MainWindow::readSerial()
{
    if (serial->canReadLine()) {
        QByteArray bytes = serial->readLine();
        ui->statusBar->showMessage("Message: " + bytes);

        this->setEnabled(true);
    }
}

void MainWindow::onSerialPortError(QSerialPort::SerialPortError error)
{
    static QSerialPort::SerialPortError previousError;

    if (error != QSerialPort::NoError && error != previousError) {
        previousError = error;
        ui->statusBar->showMessage("Serial port error :" + QString::number(error) + ": " + serial->errorString());
        if (serial->isOpen()) {
            serial->close();
            this->setDisabled(true);
        }
    }
}

/*
 * Arm sliders
 */
void MainWindow::on_dialBase_valueChanged(int value)
{
    QString value2 = QString::number(value);
    QByteArray valueArray = value2.toLatin1();

    this->setDisabled(true);

    serial->write("B");
    serial->write(valueArray);
}

void MainWindow::on_sliderShoulder_valueChanged(int value)
{
    QString value2 = QString::number(value);
    QByteArray valueArray = value2.toLatin1();

    this->setDisabled(true);

    serial->write("S");
    serial->write(valueArray);
}

void MainWindow::on_sliderElbow_valueChanged(int value)
{
    QString value2 = QString::number(value);
    QByteArray valueArray = value2.toLatin1();

    this->setDisabled(true);

    serial->write("E");
    serial->write(valueArray);
}

void MainWindow::on_sliderGripper_valueChanged(int value)
{
    QString value2 = QString::number(value);
    QByteArray valueArray = value2.toLatin1();

    this->setDisabled(true);

    serial->write("G");
    serial->write(valueArray);
}
