#ifndef TRAY_H
#define TRAY_H

#include <QSystemTrayIcon>

class Tray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit Tray(QObject *parent = 0);
    ~Tray();

private slots:
    void openFile();
};

#endif // TRAY_H
