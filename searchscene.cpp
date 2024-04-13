#include "searchscene.h"
#include "digscene.h"
#include "museumscene.h"

#include <QDebug>

SearchScene::SearchScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
    , background(":/background.png")
    , foreground(":/foreground.png")
    , otherForeground(":/foreground.png")
    , rightIdleCharacter(":/idle.png")
    , rightStep1Character(":/step1.png")
    , rightStep2Character(":/step2.png")
{
    background = background.scaled(1080, 720);
    foreground = foreground.scaled(1080, 720, Qt::IgnoreAspectRatio);
    otherForeground = foreground.scaled(1080, 720, Qt::IgnoreAspectRatio);


    rightIdleCharacter = rightIdleCharacter.scaled(200, 120, Qt::KeepAspectRatio);
    rightStep1Character = rightStep1Character.scaled(200, 120, Qt::KeepAspectRatio);
    rightStep2Character = rightStep2Character.scaled(200, 120, Qt::KeepAspectRatio);

    //flip all 3 states of the character to be used when the character is moving left
    QImage rightIdleFlipped = rightIdleCharacter.toImage().mirrored(true, false);
    leftIdleCharacter = QPixmap::fromImage(rightIdleFlipped);
    QImage rightStep1Flipped = rightStep1Character.toImage().mirrored(true, false);
    leftStep1Character = QPixmap::fromImage(rightStep1Flipped);
    QImage rightStep2Flipped = rightStep2Character.toImage().mirrored(true, false);
    leftStep2Character = QPixmap::fromImage(rightStep2Flipped);

    currentCharacter = rightIdleCharacter;
    direction = idle;


    isMoving = false;
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
}

void SearchScene::moveLeft(){
    direction = left;
}

void SearchScene::updatePlayerMovement(){
    //  qDebug() << "SPRITE MOVEMENT INDEX: " << spriteMovementIndex;
    qDebug() << "FRAME COUNTER: " << movementFrameCounter;

    // if(movementFrameCounter++ % 60 == 0){
    //     direction = idle;
    //     currentCharacter = rightIdleCharacter;
    //     qDebug() << "idle";
    //     return;
    // }

    if(direction == idle){
        isMoving = false;
        currentCharacter = rightIdleCharacter;
        return;
    }

    if (direction == right){
        if (isMoving){
            if (movementFrameCounter++ % 20 == 0){
                spriteMovementIndex = spriteMovementIndex  == 2 ? 0 : ++spriteMovementIndex;
                if (spriteMovementIndex == 0){
                    isMoving = false;
                    direction = idle;
                    movementFrameCounter = 0;
                }
            }
        } else{
            isMoving = true;
            spriteMovementIndex = 0;
            movementFrameCounter++;
        }
        foregroundX -= 5;
        otherForegroundX -=5;
    }

    if (direction == left){
        if (isMoving){

        } else{

        }
    }


    qDebug() << " not idle";
    switch(spriteMovementIndex){
    case 0:
        currentCharacter = direction == left ? leftIdleCharacter : rightIdleCharacter;
        break;
    case 1:
        currentCharacter = direction == left ? leftStep1Character : rightStep1Character;
        break;
    case 2:
        currentCharacter = direction == left ? leftStep2Character : rightStep2Character;
        break;
    default:
        break;
    }

    if (movementFrameCounter % 20 == 0){
        spriteMovementIndex = spriteMovementIndex  == 2 ? 0 : ++spriteMovementIndex;
        // movementFrameCounter = 0;
    }
}

void SearchScene::updateWorld(){
    updatePlayerMovement();


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

    painter.drawPixmap(520, 320, currentCharacter);
}

void SearchScene::printDinosaur(){

    if (player->gameOver){
        qDebug() << "Game Over";
        return;
    }

    // if (player->currentDinosaur == DinosaurName::dino1){
    //     qDebug() << "dino1";
    // }

    // if (player->currentDinosaur == DinosaurName::dino2){
    //     qDebug() << "dino2";
    // }

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
