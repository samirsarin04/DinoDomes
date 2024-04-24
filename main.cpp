#include "model.h"
#include "view.h"

#include <QApplication>

/**
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    //Used for RNG generation
    srand((unsigned) time(nullptr));

    QApplication a(argc, argv);
    Model m;
    View w(m);
    w.show();
    return a.exec();
}
