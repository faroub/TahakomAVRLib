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
                  mp_serialPort(new io::SerialPort(this)),
                  mp_statusMessage(new QLabel("Not connected ...",this))

{




    setupActions();
    setupMenuBar();
    setupToolBar();
    statusBar()->addWidget(mp_statusMessage);



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

void gui::MainWindow::enableConnectionAction(const bool a_enable)
{
    if (a_enable)
    {
        mp_connectAction->setEnabled(true);

    } else {
        mp_connectAction->setEnabled(false);
    }
}

void gui::MainWindow::enableSetSettingAction(const bool a_enable)
{
    if (a_enable)
    {
        mp_setSettingAction->setEnabled(true);

    } else {
        mp_setSettingAction->setEnabled(false);
    }
}

void gui::MainWindow::enableDisconnectionAction(const bool a_enable)
{
    if (a_enable)
    {
        mp_disconnectAction->setEnabled(true);

    } else {
        mp_disconnectAction->setEnabled(false);
    }
}

void gui::MainWindow::showStatusMessage(const QString &message)
{
    mp_statusMessage->setText(message);

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

    lp_menuIODevice->addAction(mp_connectAction);
    lp_menuIODevice->addAction(mp_disconnectAction);
    lp_menuIODevice->addSeparator();
    lp_menuIODevice->addAction(*(new QIcon(":/application-exit.png")),tr("&Quit"),this, &MainWindow::close);

    QMenu *lp_menuSetting = menuBar()->addMenu(tr("&Port"));

    lp_menuSetting->addAction(mp_setSettingAction);
    lp_menuSetting->addAction(mp_setLocalEcho);


    QMenu *lp_menuHelp = menuBar()->addMenu(tr("&Help"));
    lp_menuHelp->addAction(tr("About"), this, &MainWindow::about);



}

void gui::MainWindow::setupActions()
{
    mp_connectAction = new QAction(QIcon(":/connect.png"),tr("&Connect"), this);
    connect(mp_connectAction, SIGNAL(triggered()), mp_serialPort, SLOT(openSerialPort()));
    mp_disconnectAction = new QAction(QIcon(":/disconnect.png"),tr( "&Disconnect"), this);
    connect(mp_disconnectAction, SIGNAL(triggered()), mp_serialPort, SLOT(closeSerialPort()));
    mp_setSettingAction = new QAction(QIcon(":/settings.png"),tr("&Setting"), this);
    connect(mp_setSettingAction, SIGNAL(triggered()), mp_settingDialog, SLOT(exec()));
    mp_setLocalEcho = new QAction(tr("Local &echo"), this);
    mp_setLocalEcho->setCheckable(true);
    connect(mp_setLocalEcho, SIGNAL(toggled(bool)), mp_serialPort, SLOT(enableLocalEcho(const bool)));


}

void gui::MainWindow::setupToolBar()
{
#ifdef Q_OS_MACOS
    setUnifiedTitleAndToolBarOnMac(true);
#endif
    QToolBar *lp_fileToolBar = addToolBar(tr("IODevice"));

    lp_fileToolBar->addAction(mp_connectAction);
    lp_fileToolBar->addAction(mp_disconnectAction);
    lp_fileToolBar->addAction(mp_setSettingAction);
    addToolBar(lp_fileToolBar);
}
