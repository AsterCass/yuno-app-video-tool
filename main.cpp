
#include "admin_widget.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    // app
    QApplication app(argc, argv);
    AdminWidget  w;
    w.show();
    // config
    QApplication::setQuitOnLastWindowClosed(false);
    // ret
    return app.exec();
}
