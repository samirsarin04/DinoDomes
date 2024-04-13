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
    direction = idleRight;
    prevDirection = idleRight;


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
        *currentScene = digPtr;
        break;
    default:
        break;
    }

    player->setInput(KeyStroke::none);

    updateWorld();

    return frame;
}

void SearchScene::moveRight(){
    prevDirection = direction;
    direction = right;
}

void SearchScene::moveLeft(){
    prevDirection = direction;
    direction = left;
}

void SearchScene::updatePlayerMovement(){

    // Detects idle state and returns
    if(direction == idleRight || direction == idleLeft){
        isMoving = false;
        currentCharacter = direction == idleRight ? rightIdleCharacter : leftIdleCharacter;
        spriteMovementIndex = 0;
        movementFrameCounter = 0;
        // qDebug() << "RESET ANIMATION";
        return;
    }

    // Detects changes in direction for flipping the sprite
    if (((prevDirection == right || prevDirection == idleRight) && direction == left) || ((prevDirection == left || prevDirection == idleLeft) && direction == right)){
        //qDebug() << "triggered change in dir";
        currentCharacter = prevDirection == right || prevDirection == idleRight ? leftStep1Character : rightStep1Character;
        spriteMovementIndex = 0;
        prevDirection = direction;
    }

    // Adjusts the foreground x coordinates
    foregroundX = direction == right ? foregroundX -= 2 : foregroundX += 2;
    otherForegroundX = direction == right ? otherForegroundX -= 2 : otherForegroundX += 2;

    // sets moving to true and resets index
    if (!isMoving){
        spriteMovementIndex = 0;
        movementFrameCounter = 0;
        isMoving = true;
    }

    // increments the sprite index if 15 frames have passed
    if (movementFrameCounter != 0 && movementFrameCounter % 15 == 0){
        spriteMovementIndex += 1;
    }

    movementFrameCounter++;
    // qDebug() << spriteMovementIndex;

    if (spriteMovementIndex == 2){
        isMoving = false;
        direction = direction == right ? idleRight : idleLeft;
        stepCounter = direction == idleRight ? stepCounter+= 1 : stepCounter-= 1;
        return;
    }

    switch(spriteMovementIndex){
    case 0:
        currentCharacter = direction == right ? rightStep1Character : leftStep1Character;
        break;
    case 1:
        currentCharacter = direction == right ? rightStep2Character : leftStep2Character;
        break;
    default:
        break;
    }
}

void SearchScene::updateForeground(){
    if (foregroundX == 2){
        otherForegroundX = -1078;
    }

    if (foregroundX == -2){
        otherForegroundX = 1078;
    }

    if (otherForegroundX == -2){
        foregroundX = 1078;
    }

    if (otherForegroundX == 2){
        foregroundX = -1078;
    }
}

void SearchScene::updateWorld(){
    updatePlayerMovement();
    updateForeground();

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
