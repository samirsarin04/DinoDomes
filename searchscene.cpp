#include "searchscene.h"
#include "digscene.h"
#include "museumscene.h"

#include <QDebug>

SearchScene::SearchScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
{
    digPtr = nullptr;
    museumPtr = nullptr;
}


void SearchScene::initializePointers(DigScene &digScene, MuseumScene &museumScene){
    digPtr = &digScene;
    museumPtr = &museumScene;
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
            *currentScene = digPtr;
            break;
        case KeyStroke::interactKey:
            qDebug() << "interact key: SWITCHING FROM SEARCH TO MUSEUM";
            //player->lock.lock();
            *currentScene = museumPtr;
            //player->lock.unlock();
            break;
        default:
            break;
        }
        player->setInput(KeyStroke::none);

        QPixmap background(":/background.png");
        background = background.scaled(1080, 720);
        //ui->gameLabel->setPixmap(background);

    return background;
}
