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

    currentDinosaur = player.currentDinosaur;
    currentBone = player.currentBone;

    spawnBone();

    isMoving = false;
    digSpot = false;
    bonePassed = false;
    digSoundPlayed = false;

    digImage = QPixmap(":/placeholder.jpg");
    digImage = digImage.scaled(50, 50);

    QFont body("Copperplate Gothic Bold", 20);
    painter.setFont(body);
    painter.setPen(QColor(255, 215, 0));
    painter.setBrush(Qt::white);
}


void SearchScene::initializePointers(DigScene &digScene, MuseumScene &museumScene){
    digPtr = &digScene;
    museumPtr = &museumScene;
}

QPixmap SearchScene::buildScene(){

    activate();

    processPlayerInput();

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

void SearchScene::activate(){
    if(!deactivated) {
        return;
    }
    qDebug() << "activating search scene";
    printDinosaur();


    QMap<DinosaurBone, QPixmap> foundBones = player->getAllFoundBoneImages(player->currentDinosaur);


    qDebug() << "Size of found bones" << foundBones.size();




    digSoundPlayed = false;
    deactivated = false;
    direction = idleRight;
    prevDirection = idleRight;
    currentCharacter = rightIdleCharacter;
    spriteMovementIndex = 0;
    movementFrameCounter = 0;

    if (currentDinosaur != player->currentDinosaur){
        qDebug() << "DINO CHANGED";
        currentDinosaur = player->currentDinosaur;
        currentBone = player->currentBone;
        spawnBone();
        return;
    }

    if (currentBone != player->currentBone){
        qDebug() << "BONE CHANGED";
        currentBone = player->currentBone;
        spawnBone();
    }
}

void SearchScene::spawnBone(){
    digLocationX = rand() % 1000 + 1000;
    //digLocationX = 700;
    qDebug() << digLocationX;
}

void SearchScene::deactivate(){
    deactivated= true;
}

void SearchScene::processPlayerInput(){
    switch (player->getInput()) {
    case KeyStroke::museumKey:
        *currentScene = museumPtr;
        deactivate();
        break;
    case KeyStroke::moveLeftKey:
        moveLeft();
        break;
    case KeyStroke::moveRightKey:
        moveRight();
        break;
    case KeyStroke::interactKey:
        if (digSpot){
            *currentScene = digPtr;
            deactivate();
        }
        break;
    default:
        break;
    }
    player->setInput(KeyStroke::none);
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



    // sets moving to true and resets index
    if (!isMoving){
        player->soundEffects.enqueue(SoundEffect::walk);
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

    // Adjusts the foreground x coordinates
    foregroundX = direction == right ? foregroundX -= 2 : foregroundX += 2;
    otherForegroundX = direction == right ? otherForegroundX -= 2 : otherForegroundX += 2;
    digLocationX = direction == right ? digLocationX -= 2 : digLocationX += 2;

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

void SearchScene::checkDigCollision(){
    if (abs(540 - digLocationX) <= 50) {

        if(!digSoundPlayed){
            digSoundPlayed = true;
            //qDebug() << "adding sound effect";
            player->soundEffects.enqueue(SoundEffect::digSpot);
        }

        digSpot = true;
        return;
    }

    digSoundPlayed = false;
    digSpot = false;

    if (digLocationX <= -50){
        bonePassed = true;
        return;
    }

    bonePassed = false;
}

void SearchScene::updateWorld(){
    updatePlayerMovement();
    updateForeground();

    checkDigCollision();

    painter.drawPixmap(0, 0, background);
    painter.drawPixmap(otherForegroundX, 0, otherForeground);
    painter.drawPixmap(foregroundX, 0, foreground);
    painter.drawPixmap(520, 320, currentCharacter);

    painter.drawPixmap(digLocationX, 450, digImage);

    if (digSpot){
        painter.drawText(380, 630, "DINOSAUR DETECTED!");
        painter.drawText(425, 665, "PRESS F TO DIG!");
    }

    if (bonePassed){
        painter.drawText(100, 630, "DINOSAUR BONE PASSED!");
        painter.drawText(125, 665, "<= GO BACK!");
    }

    painter.drawText(825, 630, "YOUR BONES:");

    drawUI();
}

void SearchScene::drawUI(){


    int count = 0;
    int xVal = 800;

    QMap<DinosaurBone, QPixmap> foundBones = player->getAllFoundBoneImages(player->currentDinosaur);


    for(auto i = foundBones.begin(); i != foundBones.end(); i++){
        i.value() = i.value().scaled(50,50);
        painter.drawPixmap((xVal + 55 * count), 640, i.value());
        count++;
        //qDebug() << "drawing mini dino";
    }

    while (count < 4){
        painter.drawPixmap((xVal + 55 * count), 640, digImage.scaled(50, 50));
        count++;
    }

}

void SearchScene::printDinosaur(){

    if (player->gameOver){
        qDebug() << "Game Over";
        return;
    }

    if (player->currentDinosaur == DinosaurName::brontosaurus){
        qDebug() << "brontosaurus";
    }

    if (player->currentDinosaur == DinosaurName::triceratops){
        qDebug() << "triceratops";
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
