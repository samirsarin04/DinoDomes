#include "museumscene.h"
#include "searchscene.h"


MuseumScene::MuseumScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
{}

void MuseumScene::initializePointers(SearchScene &searchScene){
    searchPtr = &searchScene;
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
        *currentScene = searchPtr;
        //player->lock.unlock();
        break;
    default:
        break;
    }
    player->setInput(KeyStroke::none);

    QPixmap background(":/foreground.png");
    background = background.scaled(1080, 720);

    return background;
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
