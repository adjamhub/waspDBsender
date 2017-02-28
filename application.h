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
    Application(int &argc, char** argv);
    ~Application();

private:
    // L'icona nel vassoio di sistema
    SystemTrayIcon* _trayIcon;

    // le azioni presenti nella system tray icon
    QAction* _quitAction;
    QAction* _showWindowAction;

    // la finestra principale del programma
    Widget* _mainWidget;
};

#endif // APPLICATION_H
