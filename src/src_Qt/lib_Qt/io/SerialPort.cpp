#include <QDebug>
#include "SerialPort.h"
#include "MainWindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


io::SerialPort::SerialPort(gui::MainWindow *ap_mainWindow)
{
    mp_mainWindow = ap_mainWindow;
    mp_serialPort = new QSerialPort;



}

io::SerialPort::~SerialPort()
{

}


void io::SerialPort::openSerialPort()
{
    mp_mainWindow->enableConnectionAction(false);
    mp_mainWindow->enableDisconnectionAction(true);
//    mp_serialPort->setPortName(p.name);
//    mp_serialPort->setBaudRate(p.baudRate);
//    mp_serialPort->setDataBits(p.dataBits);
//    mp_serialPort->setParity(p.parity);
//    mp_serialPort->setStopBits(p.stopBits);
//    mp_serialPort->setFlowControl(p.flowControl);

}


void io::SerialPort::closeSerialPort()
{
    mp_mainWindow->enableConnectionAction(true);
    mp_mainWindow->enableDisconnectionAction(false);

}

void io::SerialPort::sendFrame(const QByteArray &data)
{

}

void io::SerialPort::readFrame()
{

}


//void io::SerialPort::handleError(QSerialPort::SerialPortError l_error)
//{

//}


void io::SerialPort::enableLocalEcho(const bool a_enable)
{
    if (a_enable)
    {
        qDebug()<< "enable local echo";

    } else {
        qDebug()<< "disable local echo";
    }

}



