/// reviewed by Samir Sarin
#include "model.h"
#include "view.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    //Used for RNG generation
    srand((unsigned) time(nullptr));

    //Q application object
    QApplication a(argc, argv);
    Model m;
    View w(m);
    w.show();
    return a.exec();
}
