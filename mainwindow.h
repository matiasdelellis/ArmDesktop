#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readSerial();

    void onSerialPortError(QSerialPort::SerialPortError error);

    void on_sliderShoulder_valueChanged(int value);

    void on_dialBase_valueChanged(int value);

    void on_sliderElbow_valueChanged(int value);

    void on_sliderGripper_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QSerialPort    *serial;

    static const quint16 arduinoVendorId = 9025;
};

#endif // MAINWINDOW_H
