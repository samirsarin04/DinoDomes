#include "searchscene.h"
#include "digscene.h"
#include "museumscene.h"

#include <QDebug>

SearchScene::SearchScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
    , background(":/background.png")
    , foreground(":/foreground.png")
{
    background = background.scaled(1080, 720);
    foreground = foreground.scaled(200, 200, Qt::IgnoreAspectRatio);
}


void SearchScene::initializePointers(DigScene &digScene, MuseumScene &museumScene){
    digPtr = &digScene;
    museumPtr = &museumScene;
}

QPixmap SearchScene::buildScene(){
    //frame.fill(Qt::transparent);
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


        //painter.end();
        updateWorld();
        //ui->gameLabel->setPixmap(background);

    return frame;
}

void SearchScene::updateWorld(){

    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    world.Step(timeStep, velocityIterations, positionIterations);

    b2Vec2 position = body->GetPosition();
    float32 angle = body->GetAngle();
    //qDebug() << position.x << " " << position.y;
    double height = position.y * 150;

    painter.drawPixmap(0, 0, background);
    painter.drawPixmap(500, 620 - height, foreground);

}
