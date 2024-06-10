#include "window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icon.png"));

    Window window;

    window.setWindowTitle("SpeedyDLP v0.2.1");

    window.resize(200, 200);

    window.show();

    return a.exec();
}
