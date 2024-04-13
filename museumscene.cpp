#include "museumscene.h"
#include "searchscene.h"


MuseumScene::MuseumScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
{

    QPixmap tRexMuseumFact(":/background.png");

}

void MuseumScene::initializePointers(SearchScene &searchScene){
    searchPtr = &searchScene;
}

QPixmap MuseumScene::buildScene(){
    // handle input
    switch (player->getInput()) {
    case KeyStroke::museumKey:
        qDebug() << "interact key: SWITCHING FROM MUSEUM TO SEARCH";
        *currentScene = searchPtr;
        break;
    case KeyStroke::moveLeftKey:
        qDebug() << "left key: museum";
        break;
    case KeyStroke::moveRightKey:
        qDebug() << "right key: museum";
        break;
    case KeyStroke::interactKey:
        qDebug() << "interact key: museum";
        break;
    default:
        break;
    }
    player->setInput(KeyStroke::none);

    QPixmap background(":/museum_background.png");
    background = background.scaled(1080, 720);

    return background;
}
