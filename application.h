#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <Windows.h>
#pragma comment(lib, "user32.lib")

class Application : public QApplication
{
public:
    Application(int, char**);
    ~Application();

    const QString fName;
    HHOOK hHook;
    bool shift;
};

#endif // APPLICATION_H
