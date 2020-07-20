#include <QDebug>
#include "Setting.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>




gui::Setting::Setting(QWidget *ap_parent) :
    QDialog(ap_parent)
{

    mp_portParameters = new portParameters();

    QGridLayout *lp_settingGridLayout = new QGridLayout();


    QGroupBox *lp_selectPortGroupBox = new QGroupBox("Select Port");
    QGridLayout *lp_selectPortGridLayout = new QGridLayout(lp_selectPortGroupBox);

    QGroupBox *lp_selectPortParametersGroupBox = new QGroupBox("Select Port Parameters");
    QGridLayout *lp_selectPortParametersGridLayout = new QGridLayout(lp_selectPortParametersGroupBox);

    mp_portComboBox = new QComboBox();

    mp_refreshButton = new QPushButton("Refresh");

    mp_descriptionLabel = new QLabel();

    mp_manufacturerLabel = new QLabel();

    mp_serialNumberLabel = new QLabel();

    mp_locationLabel = new QLabel();

    mp_vendorIDLabel = new QLabel();

    mp_productIDLabel = new QLabel();


    lp_selectPortGridLayout->addWidget(mp_portComboBox,0,0);
    lp_selectPortGridLayout->addWidget(mp_refreshButton,0,1);

    lp_selectPortGridLayout->addWidget(mp_descriptionLabel,1,0);
    lp_selectPortGridLayout->addWidget(mp_manufacturerLabel,2,0);
    lp_selectPortGridLayout->addWidget(mp_serialNumberLabel,3,0);
    lp_selectPortGridLayout->addWidget(mp_locationLabel,4,0);
    lp_selectPortGridLayout->addWidget(mp_vendorIDLabel,5,0);
    lp_selectPortGridLayout->addWidget(mp_productIDLabel,6,0);


    mp_baudRateComboBox = new QComboBox();

    QLabel *lp_baudRateLabel = new QLabel("Baud rate");

    mp_frameSizeComboBox = new QComboBox();

    QLabel *lp_frameSizeLabel = new QLabel("Frame size");

    mp_parityModeComboBox = new QComboBox();

    QLabel *lp_parityModeLabel = new QLabel("Parity mode");

    mp_stopBitsComboBox = new QComboBox();

    QLabel *lp_stopBitsLabel = new QLabel("Stop bits");

    mp_flowControlComboBox = new QComboBox();

    QLabel *lp_flowControlLabel = new QLabel("Flow control");


    lp_selectPortParametersGridLayout->addWidget(lp_baudRateLabel,0,0);
    lp_selectPortParametersGridLayout->addWidget(mp_baudRateComboBox,0,1);
    lp_selectPortParametersGridLayout->addWidget(lp_frameSizeLabel,1,0);
    lp_selectPortParametersGridLayout->addWidget(mp_frameSizeComboBox,1,1);
    lp_selectPortParametersGridLayout->addWidget(lp_parityModeLabel,2,0);
    lp_selectPortParametersGridLayout->addWidget(mp_parityModeComboBox,2,1);
    lp_selectPortParametersGridLayout->addWidget(lp_stopBitsLabel,3,0);
    lp_selectPortParametersGridLayout->addWidget(mp_stopBitsComboBox,3,1);
    lp_selectPortParametersGridLayout->addWidget(lp_flowControlLabel,4,0);
    lp_selectPortParametersGridLayout->addWidget(mp_flowControlComboBox,4,1);

    QHBoxLayout *lp_hLayout = new QHBoxLayout();
    mp_okButton = new QPushButton("OK");
    mp_cancelButton = new QPushButton("Cancel");

    lp_hLayout->addWidget(mp_okButton);
    lp_hLayout->addWidget(mp_cancelButton);

    lp_settingGridLayout->addWidget(lp_selectPortGroupBox,0,0,1,3);
    lp_settingGridLayout->addWidget(lp_selectPortParametersGroupBox,0,3,1,3);
    lp_settingGridLayout->addLayout(lp_hLayout,1,4,1,2);

    connect(mp_okButton, SIGNAL(clicked()), this, SLOT(update()));
    connect(mp_cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(mp_refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));
    connect(mp_portComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Setting::showPortInfo);

    fillPortParameters();
    updatePortsInfo();
    updatePortParameters();

    setModal(true);

    setLayout(lp_settingGridLayout);

    setWindowTitle("Setting");

}


gui::Setting::~Setting()
{


}

int gui::Setting::exec()
{
    qDebug()<< "exec";

    //updatePortsInfo();
    return QDialog::exec();
}
void gui::Setting::refresh()
{
    updatePortsInfo();

}

void gui::Setting::update()
{
    updatePortParameters();
    close();

}

void gui::Setting::updatePortParameters()
{
    mp_portParameters->m_name =  mp_portComboBox->currentText();

    if (mp_baudRateComboBox->currentIndex() == 8) {
        mp_portParameters->m_baudRate = mp_baudRateComboBox->currentText().toInt();
    } else
    {
        mp_portParameters->m_baudRate = static_cast<QSerialPort::BaudRate>(
                    mp_baudRateComboBox->itemData(mp_baudRateComboBox->currentIndex()).toInt());
    }
    mp_portParameters->m_BaudRateString = QString::number(mp_portParameters->m_baudRate);

    mp_portParameters->m_frameSize = static_cast<QSerialPort::DataBits>(
                mp_frameSizeComboBox->itemData(mp_frameSizeComboBox->currentIndex()).toInt());
    mp_portParameters->m_frameSizeString = mp_frameSizeComboBox->currentText();

    mp_portParameters->m_parityMode = static_cast<QSerialPort::Parity>(
                mp_parityModeComboBox->itemData(mp_parityModeComboBox->currentIndex()).toInt());
    mp_portParameters->m_parityModeString = mp_parityModeComboBox->currentText();

    mp_portParameters->m_stopBits = static_cast<QSerialPort::StopBits>(
                mp_stopBitsComboBox->itemData(mp_stopBitsComboBox->currentIndex()).toInt());
    mp_portParameters->m_stopBitsString = mp_stopBitsComboBox->currentText();

    mp_portParameters->m_flowControl = static_cast<QSerialPort::FlowControl>(
                mp_flowControlComboBox->itemData(mp_flowControlComboBox->currentIndex()).toInt());
    mp_portParameters->m_flowControlString = mp_flowControlComboBox->currentText();

}

void gui::Setting::showPortInfo(int a_idx)
{
    if (a_idx == -1)
    {
        return;
    }

    const QStringList list = mp_portComboBox->itemData(a_idx).toStringList();
    mp_descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr("N/A")));
    mp_manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr("N/A")));
    mp_serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr("N/A")));
    mp_locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr("N/A")));
    mp_vendorIDLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr("N/A")));
    mp_productIDLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr("N/A")));


}
void gui::Setting::updatePortsInfo()
{
    mp_portComboBox->clear();
    QList<QSerialPortInfo> l_portsInfoLists = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &l_portInfoList : l_portsInfoLists)
    {
        QStringList list;
        list << l_portInfoList.portName()
             << (l_portInfoList.description().isEmpty() ? tr("N/A") : l_portInfoList.description())
             << (l_portInfoList.manufacturer().isEmpty() ? tr("N/A") : l_portInfoList.manufacturer())
             << (l_portInfoList.serialNumber().isEmpty() ? tr("N/A") : l_portInfoList.serialNumber())
             << (l_portInfoList.systemLocation().isEmpty() ? tr("N/A") : l_portInfoList.systemLocation())
             << (l_portInfoList.vendorIdentifier() ? QString::number(l_portInfoList.vendorIdentifier(), 16) : tr("N/A") )
             << (l_portInfoList.productIdentifier() ? QString::number(l_portInfoList.productIdentifier(), 16) : tr("N/A"));

             mp_portComboBox->addItem(list.first(), list);

    }

    mp_portComboBox->addItem(tr("Custom"));



}

void gui::Setting::fillPortParameters()
{

    mp_baudRateComboBox->addItem("1200",QSerialPort::Baud1200);
    mp_baudRateComboBox->addItem("2400",QSerialPort::Baud2400);
    mp_baudRateComboBox->addItem("4800",QSerialPort::Baud4800);
    mp_baudRateComboBox->addItem("9600",QSerialPort::Baud9600);
    mp_baudRateComboBox->addItem("19200",QSerialPort::Baud19200);
    mp_baudRateComboBox->addItem("38400",QSerialPort::Baud38400);
    mp_baudRateComboBox->addItem("57600",QSerialPort::Baud57600);
    mp_baudRateComboBox->addItem("115200",QSerialPort::Baud115200);
    mp_baudRateComboBox->addItem(tr("Custom"));
    mp_baudRateComboBox->setCurrentIndex(3);


    mp_frameSizeComboBox->addItem("5",QSerialPort::Data5);
    mp_frameSizeComboBox->addItem("6",QSerialPort::Data6);
    mp_frameSizeComboBox->addItem("7",QSerialPort::Data7);
    mp_frameSizeComboBox->addItem("8",QSerialPort::Data8);
    mp_frameSizeComboBox->setCurrentIndex(3);

    mp_parityModeComboBox->addItem(tr("None"),QSerialPort::NoParity);
    mp_parityModeComboBox->addItem(tr("Even"),QSerialPort::EvenParity);
    mp_parityModeComboBox->addItem(tr("Odd"),QSerialPort::OddParity);
    mp_parityModeComboBox->addItem(tr("Mark"),QSerialPort::MarkParity);
    mp_parityModeComboBox->addItem(tr("Space"),QSerialPort::SpaceParity);
    mp_parityModeComboBox->setCurrentIndex(0);


    mp_stopBitsComboBox->addItem("1",QSerialPort::OneStop);
#ifdef Q_OS_WIN
    mp_stopBitsComboBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    mp_stopBitsComboBox->addItem("2",QSerialPort::TwoStop);
    mp_stopBitsComboBox->setCurrentIndex(0);


    mp_flowControlComboBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    mp_flowControlComboBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    mp_flowControlComboBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
    mp_flowControlComboBox->setCurrentIndex(0);



}
