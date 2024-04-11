#include "museumscene.h"
#include "searchscene.h"


MuseumScene::MuseumScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
{}

void MuseumScene::initializePointers(SearchScene &searchScene){
    searchptr = &searchScene;
}

QPixmap MuseumScene::buildScene(){
    switch (player->getInput()) {
    case KeyStroke::museumKey:
        qDebug() << "museum key: museum";
        break;
    case KeyStroke::moveLeftKey:
        qDebug() << "left key: museum";
        break;
    case KeyStroke::moveRightKey:
        qDebug() << "right key: museum";
        break;
    case KeyStroke::interactKey:
        qDebug() << "interact key: SWITCHING FROM MUSEUM TO SEARCH";
        //player->lock.lock();
        *currentScene = searchptr;
        //player->lock.unlock();
        break;
    default:
        break;
    }
    player->setInput(KeyStroke::none);

    return frame;
}

// void SearchScene::keyPress(KeyStroke key)
// {
//     switch (key) {
//     case KeyStroke::museumKey:
//         qDebug() << "museum key";
//         break;
//     case KeyStroke::moveLeftKey:
//         qDebug() << "left key";
//         break;
//     case KeyStroke::moveRightKey:
//         qDebug() << "right key";
//         break;
//     case KeyStroke::interactKey:
//         qDebug() << "interact key";
//         break;
//     default:
//         return;
//     }
// }
