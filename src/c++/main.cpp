#include "tcFlashCards.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tcFlashCards w;
    w.show();

    return a.exec();
}
