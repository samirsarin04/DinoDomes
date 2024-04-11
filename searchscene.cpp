#include "searchscene.h"
#include "digscene.h"
#include "museumscene.h"

#include <QDebug>

SearchScene::SearchScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
{
    digptr = nullptr;
    museumptr = nullptr;
}


void SearchScene::initializePointers(DigScene &digScene, MuseumScene &museumScene){
    digptr = &digScene;
    museumptr = &museumScene;
}

QPixmap SearchScene::buildScene(){
    switch (player->getInput()) {
        case KeyStroke::museumKey:
            qDebug() << "museum key";
            break;
        case KeyStroke::moveLeftKey:
            qDebug() << "left key";
            break;
        case KeyStroke::moveRightKey:
            qDebug() << "right key: SWITCHING FROM SEARCH TO DIG";
            *currentScene = digptr;
            break;
        case KeyStroke::interactKey:
            qDebug() << "interact key: SWITCHING FROM SEARCH TO MUSEUM";
            //player->lock.lock();
            *currentScene = museumptr;
            //player->lock.unlock();
            break;
        default:
            break;
        }
        player->setInput(KeyStroke::none);

    return frame;
}
