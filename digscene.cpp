#include "digscene.h"
#include "searchscene.h"
#include "museumscene.h"


DigScene::DigScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
{}

void DigScene::initializePointers(SearchScene &searchScene, MuseumScene &museumScene){
    searchPtr = &searchScene;
    museumPtr = &museumScene;
}

QPixmap DigScene::buildScene(){
    switch (player->getInput()) {
    case KeyStroke::museumKey:
        qDebug() << "museum key: dig";
        break;
    case KeyStroke::moveLeftKey:
        qDebug() << "left key: dig";
        break;
    case KeyStroke::moveRightKey:
        qDebug() << "right key: SWITCHING FROM DIG TO SEARCH";
        //player->lock.lock();
        *currentScene = searchPtr;
        //player->lock.unlock();
        break;
    case KeyStroke::interactKey:
        qDebug() << "interact key:dig";
        break;
    default:
        break;
    }
    player->setInput(KeyStroke::none);

    return frame;
}
