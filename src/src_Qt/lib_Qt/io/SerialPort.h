/**
 * @file SerialPort.h
 * @brief Header file of the SerialPort class
 *
 * SerialPort class provides functions to access the serial ports
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


namespace io
{

class SerialPort : public QObject
{
    Q_OBJECT

public slots:

    void openSerialPort();

    void closeSerialPort();



public:
    SerialPort();

    ~SerialPort();


    void sendFrame(const QByteArray &data);

    void readFrame();

    void handleError(QSerialPort::SerialPortError l_error);

protected:
private:

    QSerialPort *mp_serialPort = nullptr;

    QSerialPortInfo *mp_serialPortInfo = nullptr;


};





}





#endif
