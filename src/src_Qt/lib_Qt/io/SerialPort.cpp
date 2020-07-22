#include <QDebug>
#include "SerialPort.h"
#include "MainWindow.h"
#include "Setting.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


io::SerialPort::SerialPort(gui::MainWindow *ap_mainWindow, gui::Setting *ap_setting)
{
    mp_mainWindow = ap_mainWindow;
    mp_setting = ap_setting;
    mp_serialPort = new QSerialPort;



}

io::SerialPort::~SerialPort()
{

}


void io::SerialPort::openSerialPort()
{
    mp_mainWindow->enableConnectionAction(false);
    mp_mainWindow->enableDisconnectionAction(true);
    mp_serialPort->setPortName(mp_setting->getPortParameters()->m_name);
    mp_serialPort->setBaudRate(mp_setting->getPortParameters()->m_baudRate);
    mp_serialPort->setDataBits(mp_setting->getPortParameters()->m_frameSize);
    mp_serialPort->setParity(mp_setting->getPortParameters()->m_parityMode);
    mp_serialPort->setStopBits(mp_setting->getPortParameters()->m_stopBits);
    mp_serialPort->setFlowControl(mp_setting->getPortParameters()->m_flowControl);

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



