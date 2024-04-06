#include "window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Window window;

    window.setWindowTitle("SpeedyDLP v0.1");

    window.show();

    a.setWindowIcon(QIcon(":/placeholder_icon.ico"));

    //default width is 400 pixels, default height is done automatically
    window.resize(400, window.height());

    return a.exec();
}
