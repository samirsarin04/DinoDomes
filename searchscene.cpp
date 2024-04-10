#include "searchscene.h"

#include <QDebug>

SearchScene::SearchScene(QObject *parent)
    : Scene{parent}
{}

void SearchScene::keyPress(KeyStroke key)
{
    switch (key) {
    case KeyStroke::museumKey:
        qDebug() << "museum key";
        break;
    case KeyStroke::moveLeftKey:
        qDebug() << "left key";
        break;
    case KeyStroke::moveRightKey:
        qDebug() << "right key";
        break;
    case KeyStroke::interactKey:
        qDebug() << "interact key";
        break;
    default:
        return;
    }
}
