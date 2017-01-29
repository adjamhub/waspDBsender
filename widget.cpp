#include "widget.h"
#include "ui_widget.h"

#include <QSqlError>
#include <QFileDialog>
#include <QTextStream>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString db_name = "";
    QString user = "";
    QString pass = "";
    QString host = "";

    _db = QSqlDatabase::addDatabase("QMYSQL");
    _db.setHostName(host);
    _db.setDatabaseName(db_name);
    _db.setUserName(user);
    _db.setPassword(pass);

    _filePath = "";

    // Connect objects
    connect(ui->connectButton, &QPushButton::clicked, this, &Widget::connectToDB);
    connect(ui->loadDataButton, &QPushButton::clicked, this, &Widget::loadDataInDB);
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::connectToDB()
{
    ui->dbLabel->setText("Connecting...");
    if (_db.open())
    {
        ui->dbLabel->setText("OK! Connected!!!");
        return true;
    }

    ui->dbLabel->setText("Error: " + _db.lastError().text());
    return false;
}

bool Widget::loadDataInDB()
{
    if (!_db.isOpen())
    {
        ui->fileLabel->setText("Connection to DB closed");
        return false;
    }

    if (_filePath.isEmpty())
    {
        _filePath = QFileDialog::getOpenFileName(this, "Apri file");
    }

    if (_filePath.isEmpty())
    {
        ui->fileLabel->setText("Ahi ahi ahi...");
        return false;
    }

    ui->fileLabel->setText("FILE: " + _filePath);

    QFile file(_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->fileLabel->setText("Problem opening the file");
        return false;
    }

    QTextStream in(&file);
    while (!file.atEnd()) {
        QString line = in.readLine();
        // now the query :)
    }

    return true;
}
