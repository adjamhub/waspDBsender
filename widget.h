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

    // puntatore alla classe in grado di gestire le connessioni di rete
    // Guarda la documentazione
    QNetworkAccessManager* _manager;

    // Il proxy serve per collegarsi a Internet DENTRO la rete scolastica
    QNetworkProxy _proxy;

    // puntatore alla porta seriale
    QSerialPort* _serialPort;

    // stringa che rappresenta l'URL del sito ove inviare i dati
    QString _siteUrl;
};

#endif // WIDGET_H
