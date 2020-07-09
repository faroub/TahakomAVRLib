/**
 * @file Setting.h
 * @brief Header file of the Setting class
 *
 * Setting GUI allows to define the communication port settings
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef SETTING_H
#define SETTING_H
#include <QDialog>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

namespace gui
{

class Setting : public QDialog
{
public:

    Setting(QWidget *ap_parent = nullptr);

    ~Setting();

    void enableLocalEcho();

protected:


private:



};



}




#endif
