#include "mainwidget.h"
#include "./ui_mainwidget.h"
#include <QSerialPortInfo>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    // 刷新一次端口数据
    on_pushButton_refresh_clicked();
    // 未连接时不允许发送
    ui->pushButton_dataSend->setEnabled(false);
    // 默认发送后清空
    ui->checkBox_clearSend->setCheckState(Qt::Checked);
    // 保存默认配置
    serialPort.setPortName(ui->comboBox_port->currentText());
    serialPort.setBaudRate(QSerialPort::Baud115200);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    connect(&serialPort, &QSerialPort::readyRead, this, &MainWidget::refreshDataRecv);
}

MainWidget::~MainWidget()
{
    if (serialPort.isOpen())
        serialPort.close();
    delete ui;
}

void MainWidget::on_comboBox_port_currentTextChanged(const QString &port)
{
    serialPort.setPortName(port);
}


void MainWidget::on_comboBox_baudRate_currentTextChanged(const QString &rate)
{
    serialPort.setBaudRate((QSerialPort::BaudRate)rate.toUInt());
}


void MainWidget::on_comboBox_dataBits_currentIndexChanged(int index)
{
    serialPort.setDataBits((QSerialPort::DataBits)(index + 5));
}


void MainWidget::on_comboBox_stopBits_currentIndexChanged(int index)
{
    const QSerialPort::StopBits dict[] = {
        QSerialPort::OneStop,
        QSerialPort::OneAndHalfStop,
        QSerialPort::TwoStop,
    };
    serialPort.setStopBits(dict[index]);
}


void MainWidget::on_comboBox_parity_currentIndexChanged(int index)
{
    const QSerialPort::Parity dict[] = {
        QSerialPort::NoParity,
        QSerialPort::OddParity,
        QSerialPort::EvenParity,
    };
    serialPort.setParity(dict[index]);
}


void MainWidget::on_pushButton_connect_clicked()
{
    ui->pushButton_connect->setEnabled(false);
    if (ui->pushButton_connect->text() == "连接") {
        if (serialPort.open(QIODeviceBase::ReadWrite) == true) {
            ui->pushButton_connect->setText("断开连接");
            ui->groupBox_param->setEnabled(false);
            ui->pushButton_dataSend->setEnabled(true);
            ui->pushButton_refresh->setEnabled(false);
        }
    } else {
        serialPort.close();
        ui->pushButton_connect->setText("连接");
        ui->groupBox_param->setEnabled(true);
        ui->pushButton_dataSend->setEnabled(false);
        ui->pushButton_refresh->setEnabled(true);
    }
    ui->pushButton_connect->setEnabled(true);
}


void MainWidget::on_pushButton_dataSend_clicked()
{
    QString data = ui->plainTextEdit_dataSend->toPlainText();
    if (clearSend)
        ui->plainTextEdit_dataSend->clear();
    if (data.isEmpty() || serialPort.isOpen() == false)
        return;
    serialPort.write(data.toStdString().c_str());
}

void MainWidget::refreshDataRecv()
{
    QByteArray data;
    if (showHex) {
        data = transStrToHex(serialPort.readAll());
    } else {
        data = serialPort.readAll();
    }

    if (showAppend) {
        ui->textBrowser_dataRecv->append(data);
    } else {
        ui->textBrowser_dataRecv->setText(data);
    }
}

void MainWidget::on_pushButton_refresh_clicked()
{
    ui->comboBox_port->clear();
    auto serialList = QSerialPortInfo::availablePorts();
    for (const auto &availPort : serialList) {
        ui->comboBox_port->addItem(availPort.portName());
    }
}


void MainWidget::on_pushButton_clearRecv_clicked()
{
    ui->textBrowser_dataRecv->clear();
}


void MainWidget::on_checkBox_showAppend_stateChanged(int state)
{
    showAppend = state == Qt::Checked ? true : false;
}


void MainWidget::on_checkBox_showHex_stateChanged(int state)
{
    if (state == Qt::Checked) {
        showHex = true;
        ui->textBrowser_dataRecv->setText(
            transStrToHex(ui->textBrowser_dataRecv->toPlainText().toUtf8())
        );
    } else {
        showHex = false;
        ui->textBrowser_dataRecv->setText(
            transHexToStr(ui->textBrowser_dataRecv->toPlainText().toUtf8())
        );
    }
}

QByteArray MainWidget::transStrToHex(const QByteArray &str)
{
    return str.toHex(' ');
}

QByteArray MainWidget::transHexToStr(const QByteArray &hex)
{
    return QByteArray::fromHex(hex);
}


void MainWidget::on_checkBox_clearSend_stateChanged(int state)
{
    clearSend = state == Qt::Checked ? true : false;
}

