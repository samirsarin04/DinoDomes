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

    //
    // if (deactivated){
    // this syncs the local state with player state
    // prevents accessing data structures a billion times while creating frames
    // for example the bone the player is looking for will not change while this scene is running
    //activate();
    //}


    switch (player->getInput()) {
        case KeyStroke::museumKey:
            // PRESS M TO CYCLE THROUGH THE DIFFERENT BONES
            qDebug() << "**************CURRENT BONE****************";
            printDinosaur();
            player->nextBone();
            break;
        case KeyStroke::moveLeftKey:
             // PRESS A TO SKIP TO THE NEXT DINOSAUR
            qDebug() << "**************DINO BEFORE SKIP****************";
            printDinosaur();
            player->nextDinosaur();
            qDebug() << "**************DINO AFTER SKIP****************";
            printDinosaur();
            break;
        case KeyStroke::moveRightKey:
            qDebug() << "right key: SWITCHING FROM SEARCH TO DIG";
            // deactivate() deactivate when switching scenes
            *currentScene = digPtr;
            break;
        case KeyStroke::interactKey:
            qDebug() << "interact key: SWITCHING FROM SEARCH TO MUSEUM";
            *currentScene = museumPtr;
            break;
        default:
            break;
        }

    player->setInput(KeyStroke::none);

    updateWorld();

    return frame;
}


void SearchScene::printDinosaur(){

    if (player->gameOver){
        qDebug() << "Game Over";
        return;
    }

    if (player->currentDinosaur == DinosaurName::dino1){
        qDebug() << "dino1";
    }

    if (player->currentDinosaur == DinosaurName::dino2){
        qDebug() << "dino2";
    }

    if (player->currentDinosaur == DinosaurName::tRex){
        qDebug() << "tRex";
    }

    if (player->currentBone == DinosaurBone::arms){
        qDebug() << "arms";
    }

    if (player->currentBone == DinosaurBone::head){
        qDebug() << "head";
    }

    if (player->currentBone == DinosaurBone::body){
        qDebug() << "body";
    }

    if (player->currentBone == DinosaurBone::legs){
        qDebug() << "legs";
    }
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
