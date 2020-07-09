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
protected:


private:

    void setupMenuBar();

    void setupToolBar();

    void showStatusMessage(const QString &message);

    Setting *mp_settingDialog = nullptr;

    io::SerialPort *mp_serialPort = nullptr;

    QLabel *m_statusMessage = nullptr;



};
}

#endif
