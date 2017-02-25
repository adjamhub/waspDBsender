#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>

#include <QSerialPort>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private Q_SLOTS:
    void activateSerialPort(bool p);
    void handleReadyRead();
    void manageReply(QNetworkReply* reply);
    void updateSerialPortList();

    void apply();
    void load();
    void save();


private:
    Ui::Widget *ui;

    // manage network connections. See docs
    QNetworkAccessManager* _manager;

    // proxy is necessary inside school network
    QNetworkProxy _proxy;

    // the serial port pointer
    QSerialPort* _serialPort;

    QString _siteUrl;
};

#endif // WIDGET_H
