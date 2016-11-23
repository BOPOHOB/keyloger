#include "application.h"
#include "tray.h"

int main(int argc, char *argv[])
{
    QLocale curLocale(QLocale("ru_RU"));
    QLocale::setDefault(curLocale);

    Application a(argc, argv);
    setlocale(LC_ALL,"RUS");

    Tray t;
    t.show();

    return a.exec();
}
