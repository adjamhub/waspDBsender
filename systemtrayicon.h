#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H


#include <QSystemTrayIcon>
#include <QMenu>

class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit SystemTrayIcon(QObject* parent = nullptr);
    ~SystemTrayIcon();

    // add action pointed by ac to context menu
    void addAction(QAction* ac);

private:
    QMenu* _menu;
};

#endif // SYSTEMTRAYICON_H
