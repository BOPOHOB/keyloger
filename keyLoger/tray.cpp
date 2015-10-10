#include "tray.h"
#include <QIcon>
#include <QAction>
#include <QMenu>
#include "application.h"
#include <QDesktopServices>
#include <QUrl>

Tray::Tray(QObject* parent)
    : QSystemTrayIcon(parent)
{
    setIcon(QIcon(":/file/png.png"));

    setContextMenu(new QMenu(0));
    this->contextMenu()->addAction("File", this, SLOT(openFile()));
    this->contextMenu()->addAction("Exit", qApp, SLOT(quit()));
}

void Tray::openFile() {
    QDesktopServices::openUrl(static_cast<Application*>(qApp)->fName);
}

Tray::~Tray()
{

}

