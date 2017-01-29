#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    bool connectToDB();
    bool loadDataInDB();

private:
    Ui::Widget *ui;

    QSqlDatabase _db;
    QString _filePath;
};

#endif // WIDGET_H
