#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void refreshDataRecv();
    void on_comboBox_port_currentTextChanged(const QString &state);
    void on_comboBox_baudRate_currentTextChanged(const QString &state);
    void on_comboBox_dataBits_currentIndexChanged(int index);
    void on_comboBox_stopBits_currentIndexChanged(int index);
    void on_comboBox_parity_currentIndexChanged(int index);
    void on_pushButton_connect_clicked();
    void on_pushButton_dataSend_clicked();
    void on_pushButton_refresh_clicked();
    void on_pushButton_clearRecv_clicked();
    void on_checkBox_showAppend_stateChanged(int arg1);
    void on_checkBox_showHex_stateChanged(int arg1);

    void on_checkBox_clearSend_stateChanged(int arg1);

private:
    Ui::MainWidget *ui;
    bool showAppend = false;  // 追加显示模式
    bool showHex = false;     // 十六进制显示模式
    bool clearSend = true;    // 发送后清空
    QSerialPort serialPort;

    QByteArray transStrToHex(const QByteArray& str);
    QByteArray transHexToStr(const QByteArray& hex);
};
#endif // MAINWIDGET_H
