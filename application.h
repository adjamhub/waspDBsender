#ifndef APPLICATION_H
#define APPLICATION_H


#include <QApplication>
#include <QAction>

#include "systemtrayicon.h"
#include "widget.h"


class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application(int &argc, char** argv);
    ~Application();

private:
    SystemTrayIcon* _trayIcon;

    QAction* _quitAction;
    QAction* _showWindowAction;

    Widget* _mainWidget;
};

#endif // APPLICATION_H
