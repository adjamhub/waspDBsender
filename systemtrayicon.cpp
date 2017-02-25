#include "systemtrayicon.h"

#include <QIcon>


SystemTrayIcon::SystemTrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    QIcon waspIcon = QIcon(":/icons/waspicon.svg");
    setIcon(waspIcon);

    _menu = new QMenu;
    setContextMenu(_menu);
}

SystemTrayIcon::~SystemTrayIcon()
{
    _menu->deleteLater();
}

void SystemTrayIcon::addAction(QAction *ac)
{
    _menu->addAction(ac);
}
