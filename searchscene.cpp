#include "searchscene.h"
#include "digscene.h"
#include "museumscene.h"

#include <QDebug>

SearchScene::SearchScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
    , background(":/background.png")
    , foreground(":/foreground.png")
    , otherForeground(":/foreground.png")
{
    background = background.scaled(1080, 720);
    foreground = foreground.scaled(1080, 720, Qt::IgnoreAspectRatio);
    otherForeground = foreground.scaled(1080, 720, Qt::IgnoreAspectRatio);
}


void SearchScene::initializePointers(DigScene &digScene, MuseumScene &museumScene){
    digPtr = &digScene;
    museumPtr = &museumScene;
}

QPixmap SearchScene::buildScene(){

    //
    // if (deactivated){
    //
    // WE MAY WANT TO DETECT IF CONTROL FLOW IS BEING HANDED TO THIS SCENE AGAIN
    //activate();
    //}


    switch (player->getInput()) {
        case KeyStroke::museumKey:
            // PRESS M TO CYCLE THROUGH THE DIFFERENT BONES
            // qDebug() << "**************CURRENT BONE****************";
            // printDinosaur();
            // player->nextBone();
            *currentScene = museumPtr;
            break;
        case KeyStroke::moveLeftKey:
            moveLeft();
            break;
        case KeyStroke::moveRightKey:
            moveRight();
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

void SearchScene::moveRight(){
    direction = right;
    otherForegroundX-=5;
    foregroundX-=5;
}

void SearchScene::moveLeft(){
    direction = left;
    otherForegroundX+=5;
    foregroundX+=5;
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
    //otherForegroundX = 1080 + foregroundX;
    qDebug() << "FOREGROUND X: " << foregroundX;
    qDebug() << "OTHER FOREGROUND X: " << otherForegroundX;
    // if(foregroundX == 5 || otherForegroundX == 5 || foregroundX == -5 || otherForegroundX == -5){
    //     if(direction == right){
    //         if(foregroundX < -1075){
    //             foregroundX = 1075;
    //         }
    //         if(otherForegroundX < -1075){
    //             otherForegroundX = 1075;
    //         }
    //     }
    //     if(direction == left){
    //         if(foregroundX > 0){
    //             otherForegroundX = -1075;
    //         }
    //         if(otherForegroundX > 0){


    //         }

    //     }
    // }

    // painter.drawPixmap(0, 0, background);
    // painter.drawPixmap(otherForegroundX, 0, otherForeground);
    // painter.drawPixmap(foregroundX, 0, foreground);

    if (foregroundX == 5){
        otherForegroundX = -1075;
    }

    if (foregroundX == -5){
        otherForegroundX = 1075;
    }

    if (otherForegroundX == -5){
        foregroundX = 1075;
    }

    if (otherForegroundX == 5){
        foregroundX = -1075;
    }

    painter.drawPixmap(0, 0, background);
    painter.drawPixmap(otherForegroundX, 0, otherForeground);
    painter.drawPixmap(foregroundX, 0, foreground);


}
