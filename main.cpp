#include <QApplication>
#include "dialog.h"
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Dialog a;
    a.show();
    return app.exec();
}
