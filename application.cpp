
#include "application.h"


Application::Application(int &argc, char **argv)
    : QApplication(argc,argv)
{
    // first, initialize the main widget
    _mainWidget = new Widget;

    // Application icon
    QIcon appIcon = QIcon(":/icons/waspicon.svg");
    _mainWidget->setWindowIcon(appIcon);

    // then think to the system tray icon
    _trayIcon = new SystemTrayIcon;

    // create and add the needed actions
    QIcon showIcon = QIcon(":/icons/window.svg");
    _showWindowAction = new QAction(showIcon, "Show Window", this);
    connect(_showWindowAction, &QAction::triggered, _mainWidget, &Widget::show);
    _trayIcon->addAction(_showWindowAction);

    QIcon quitIcon = QIcon(":/icons/window-close.svg");
    _quitAction = new QAction(quitIcon, "Quit", this);
    connect(_quitAction, &QAction::triggered, this, &Application::quit);
    _trayIcon->addAction(_quitAction);

    // finally show tray icon
    _trayIcon->show();
}


Application::~Application()
{
    // Remember to delete un-parented widgets
    _mainWidget->deleteLater();
    _trayIcon->deleteLater();
}
