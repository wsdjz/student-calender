#include <QApplication>
#include "login.h"

int main(int argc, char *argv[]) {
QApplication::addLibraryPath("./plugins");
    QApplication app(argc, argv);


    LoginWidget loginWidget;
    loginWidget.show();

    return app.exec();
}
