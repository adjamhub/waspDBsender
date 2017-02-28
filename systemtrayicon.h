#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H


#include <QSystemTrayIcon>
#include <QMenu>

class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    SystemTrayIcon(QObject* parent = nullptr);
    ~SystemTrayIcon();

    // Funzione di utilità
    // Aggiunge l'azione puntata da ac al menù contestuale
    void addAction(QAction* ac);

private:
    QMenu* _menu;
};

#endif // SYSTEMTRAYICON_H
