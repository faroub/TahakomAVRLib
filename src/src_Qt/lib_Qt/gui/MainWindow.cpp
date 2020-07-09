#include <QDebug>
#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QIcon>

#include "MainWindow.h"
#include "Setting.h"
#include "SerialPort.h"






gui::MainWindow::MainWindow(QWidget *ap_applicationGUI)
                : mp_settingDialog(new Setting(this)),
                  mp_serialPort(new io::SerialPort()),
                  m_statusMessage(new QLabel("Not connected ...",this))

{





    setupMenuBar();
    setupToolBar();
    statusBar()->addWidget(m_statusMessage);



    // set a dummy wideget if none is defined
    if (ap_applicationGUI == nullptr)
    {
        ap_applicationGUI = new QWidget();
        QLabel *lp_noApplicationText = new QLabel(ap_applicationGUI);
        lp_noApplicationText->setText("No Application ...");
        ap_applicationGUI->setMinimumSize(400, 400);
    }

    setWindowTitle("I/O Application GUI");
    setCentralWidget(ap_applicationGUI);



}

gui::MainWindow::~MainWindow()
{

}


void gui::MainWindow::showStatusMessage(const QString &message)
{
        m_statusMessage->setText(message);

}
void gui::MainWindow::about()
{
    QMessageBox::about(this, tr("About"),tr("This Simple Qt Application allows you to communicate\n"
                                            " with an avr chip using the serial port."
                                            "\n"
                                            "\n"
                                            "Farid Oubbati\n"
                                            "(https://github.com/faroub)"));
}


void gui::MainWindow::setupMenuBar()
{
    QMenu *lp_menuIODevice = menuBar()->addMenu(tr("IO&Device"));

    lp_menuIODevice->addAction(*(new QIcon(":/connect.png")),tr("C&onnect"), mp_serialPort, &io::SerialPort::openSerialPort);
    lp_menuIODevice->addAction(*(new QIcon(":/disconnect.png")),tr( "D&isconnect"), mp_serialPort, &io::SerialPort::closeSerialPort);
    lp_menuIODevice->addSeparator();
    lp_menuIODevice->addAction(*(new QIcon(":/application-exit.png")),tr("&Quit"),this, &MainWindow::close);

    QMenu *lp_menuSetting = menuBar()->addMenu(tr("&Port"));

    lp_menuSetting->addAction(*(new QIcon(":/settings.png")),tr("S&etting"), mp_settingDialog, &Setting::exec);
    lp_menuSetting->addAction(tr("&Local echo"), mp_settingDialog, &Setting::enableLocalEcho);


    QMenu *lp_menuHelp = menuBar()->addMenu(tr("&Help"));
    lp_menuHelp->addAction(tr("About"), this, &MainWindow::about);



}


void gui::MainWindow::setupToolBar()
{
#ifdef Q_OS_MACOS
    setUnifiedTitleAndToolBarOnMac(true);
#endif
    QToolBar *lp_fileToolBar = addToolBar(tr("IODevice"));

    lp_fileToolBar->addAction(*(new QIcon(":/connect.png")),tr("C&onnect"), mp_serialPort, &io::SerialPort::openSerialPort);
    lp_fileToolBar->addAction(*(new QIcon(":/disconnect.png")),tr( "D&isconnect"), mp_serialPort, &io::SerialPort::closeSerialPort);
    lp_fileToolBar->addAction(*(new QIcon(":/settings.png")),tr("S&etting"), mp_settingDialog, &Setting::exec);
    addToolBar(lp_fileToolBar);
}
