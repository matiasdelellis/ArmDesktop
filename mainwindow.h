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
    void on_sliderElbow_sliderMoved(int position);

    void on_sliderGripper_sliderMoved(int position);

    void on_pushMotor_pressed();

    void on_pushMotor_released();

    void on_checkLighting_toggled(bool checked);

    void readSerial();

    void on_sliderShoulder_valueChanged(int value);

    void on_dialBase_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QSerialPort    *serial;

    static const quint16 arduinoVendorId = 9025;
};

#endif // MAINWINDOW_H
