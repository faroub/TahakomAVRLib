/**
 * @file MainWindow.h
 * @brief Header file of the MainWindow class
 *
 * MainWindow GUI holds all PC <-> MCU communication modules
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

namespace gui
{
class Setting;
}
namespace io
{
class SerialPort;
}
namespace gui
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void about();

public:
    /** Constructor. Builds the MainWindow GUI
         *
         *  @param ap_applicationGUI defines pointer to application GUI
         */
    MainWindow(QWidget *ap_applicationGUI = nullptr);
    /** Destructor.
        */
    ~MainWindow();

    void enableConnectionAction(const bool a_enable);

    void enableDisconnectionAction(const bool a_enable);

    void enableSetSettingAction(const bool a_enable);

    void showStatusMessage(const QString &message);



protected:


private:

    void setupActions();

    void setupMenuBar();

    void setupToolBar();


    Setting *mp_settingDialog = nullptr;

    io::SerialPort *mp_serialPort = nullptr;

    QLabel *mp_statusMessage = nullptr;

    QAction *mp_connectAction = nullptr;

    QAction *mp_disconnectAction = nullptr;

    QAction *mp_setSettingAction = nullptr;

    QAction *mp_setLocalEcho = nullptr;



};
}

#endif
