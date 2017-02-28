#include "widget.h"
#include "ui_widget.h"


#include <QFileDialog>
#include <QSerialPortInfo>
#include <QSettings>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // just to start well :)
    ui->notificationLabel->setText("Welcome :)");

    // first things first: load data!
    load();

    updateSerialPortList();

    // proxy
    ui->proxyGroupBox->setEnabled(ui->proxyEnabledCheckBox->isChecked());
    connect(ui->proxyEnabledCheckBox, &QCheckBox::toggled, ui->proxyGroupBox, &QGroupBox::setEnabled);
    connect(ui->updateSerialPortButton, &QPushButton::clicked, this, &Widget::updateSerialPortList);

    // connect buttons to proper slots
    connect(ui->saveButton, &QPushButton::clicked, this, &Widget::save);

    ui->startButton->setCheckable(true);
    connect(ui->startButton, &QPushButton::clicked, this, &Widget::activateSerialPort);

    connect(ui->closeButton, &QPushButton::clicked, this, &Widget::hide);

    // last things, the objects
    _serialPort = new QSerialPort(this);

    _siteUrl = "http://www.liceodavincijesi.gov.it/pages/IoT/insert.php";

    //
    _manager = new QNetworkAccessManager(this);
    connect(_manager, &QNetworkAccessManager::finished, this, &Widget::manageReply);
}


Widget::~Widget()
{
    delete ui;
}


void Widget::activateSerialPort(bool p)
{
    if (p)
    {
        // just to be sure...
        apply();

        if (ui->serialPortsComboBox->count() == 1)
        {
            // ERROR
            ui->notificationLabel->setText("no serial ports detected");
            ui->startButton->setChecked(false);
            return;
        }

        QString serialPortName = ui->serialPortsComboBox->currentText();

        if (serialPortName == "(none)")
        {
            // ERROR
            ui->notificationLabel->setText("Please, select a serial port");
            ui->startButton->setChecked(false);
            return;
        }

        _serialPort->setPortName(serialPortName);
        _serialPort->setBaudRate(QSerialPort::Baud38400);
        if (!_serialPort->open(QIODevice::ReadOnly))
        {
            // ERROR
            ui->notificationLabel->setText("cannot open serial port");
            ui->startButton->setChecked(false);
            return;
        }
        connect(_serialPort, &QSerialPort::readyRead, this, &Widget::handleReadyRead);

        ui->startButton->setText("Stop");
        ui->notificationLabel->setText("Serial Port opened");
        return;
    }

    disconnect(_serialPort, &QSerialPort::readyRead, this, &Widget::handleReadyRead);
    _serialPort->close();
    ui->startButton->setText("Start");
    ui->notificationLabel->setText("Serial Port Closed");
    return;
}


void Widget::handleReadyRead()
{
    // wait until ALL line has been read...
    if (!_serialPort->canReadLine())
        return;

    // we don't need more than 256 bytes...
    char buffer[256];

    int n = _serialPort->readLine(buffer, 256);
    _serialPort->flush();

    ui->notificationLabel->setText("reading data...");

    if (n < 10)
    {
        ui->logsTextEdit->append("short string read");
        return;
    }

    QString p(buffer);
    ui->logsTextEdit->append(p);

    QStringList splittedString = p.split('#');

    // controllo di sicurezza...
    if (splittedString.count() < 10)
    {
        ui->logsTextEdit->append("Unreadable string");
        ui->logsTextEdit->append(p);
        return;
    }

    // leggo i parametri da caricare...
    QString node = splittedString.at(2);

    QString index = splittedString.at(3);

    QString temp = splittedString.at(4);
    temp.remove("TC:");

    QString hum = splittedString.at(5);
    hum.remove("HUM:");

    QString pres = splittedString.at(6);
    pres.remove("PRES:");

    QString ac = splittedString.at(7);
    ac.remove("ACC:");
    QStringList acl = ac.split(';');
    QString acX = acl.at(0);
    QString acY = acl.at(1);
    QString acZ = acl.at(2);

    QString data = splittedString.at(8);
    data.remove("TIME:");
    data = "NOW()"; // easier to read. Easier to load :)

    QString bat = splittedString.at(9);
    bat.remove("BAT:");

    QString co = "'NULL'";
    QString no = "'NULL'";
    QString unaltr = "'NULL'";

    //
    QString queryUrl = "";
    queryUrl += "node='" + node + "'&"; // strings in SQL need apexes
    queryUrl += "index=" + index + "&";
    queryUrl += "temp=" + temp + "&";
    queryUrl += "hum=" + hum + "&";
    queryUrl += "pres=" + pres + "&";
    queryUrl += "acX=" + acX + "&";
    queryUrl += "acY=" + acY + "&";
    queryUrl += "acZ=" + acZ + "&";
    queryUrl += "data=" + data + "&";
    queryUrl += "bat=" + bat + "&";
    queryUrl += "co=" + co + "&";
    queryUrl += "no=" + no + "&";
    queryUrl += "unaltr=" + unaltr;

    // notify url data sent
    ui->logsTextEdit->append("URL DATA SENT");
    ui->logsTextEdit->append(queryUrl);

    // l'URL da creare per l'invio DEVE essere fatto in questo modo
    QString url = _siteUrl + "&" + queryUrl;
    ui->logsTextEdit->append(url);

    _manager->get(QNetworkRequest(QUrl(url)));
}


void Widget::manageReply(QNetworkReply* reply)
{
    // TODO
    if (reply->error() != QNetworkReply::NoError)
    {
        ui->logsTextEdit->append("REPLY ERROR");
        ui->logsTextEdit->append("URL: " + reply->url().toString());
        ui->logsTextEdit->append(reply->errorString());
        return;
    }

    ui->logsTextEdit->append("invio avvenuto correttamente!!!");
}


void Widget::updateSerialPortList()
{
    ui->serialPortsComboBox->clear();
    ui->serialPortsComboBox->addItem("(none)");

    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    Q_FOREACH(QSerialPortInfo portInfo, infoList)
    {
        ui->serialPortsComboBox->addItem(portInfo.portName());
    }
}


void Widget::apply()
{
    if (!ui->proxyEnabledCheckBox->isChecked())
    {
        _proxy.setType(QNetworkProxy::NoProxy);
        _manager->setProxy(_proxy);
        ui->logsTextEdit->append("NO proxy SET");
        return;
    }

    _proxy.setType(QNetworkProxy::HttpCachingProxy);
    _proxy.setHostName(ui->proxyHostLineEdit->text());
    _proxy.setPort(ui->proxyPortSpinBox->value());
    _proxy.setUser(ui->proxyUserLineEdit->text());
    _proxy.setPassword(ui->proxyPassLineEdit->text());

    // also apply to the network manager
    _manager->setProxy(_proxy);

    ui->logsTextEdit->append("PROXY: ");
    ui->logsTextEdit->append(_proxy.hostName() + ":" + QString::number(_proxy.port()));
    return;
}


void Widget::load()
{
    QSettings settings("waspsender.conf", QSettings::IniFormat);
    settings.beginGroup("proxy");

    ui->proxyEnabledCheckBox->setChecked( settings.value("proxyEnabled").toBool() );
    ui->proxyHostLineEdit->setText( settings.value("proxyHost").toString() );
    ui->proxyPortSpinBox->setValue( settings.value("proxyPort").toInt() );
    ui->proxyUserLineEdit->setText( settings.value("proxyUser").toString() );
    ui->proxyPassLineEdit->setText( settings.value("proxyPass").toString() );
    settings.endGroup();

    ui->logsTextEdit->append(settings.fileName());
}


void Widget::save()
{
    QSettings settings("waspsender.conf", QSettings::IniFormat);

    settings.beginGroup("proxy");
    settings.setValue("proxyEnabled", ui->proxyEnabledCheckBox->isChecked());
    settings.setValue("proxyHost", ui->proxyHostLineEdit->text());
    settings.setValue("proxyPort", ui->proxyPortSpinBox->value());
    settings.setValue("proxyUser", ui->proxyUserLineEdit->text());
    settings.setValue("proxyPass", ui->proxyPassLineEdit->text());
    settings.endGroup();

    settings.sync();
}
