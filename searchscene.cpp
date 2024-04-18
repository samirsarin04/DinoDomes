#include "searchscene.h"
#include "digscene.h"
#include "museumscene.h"
#include <Box2D/Box2D.h>
#include <QDebug>
#include <set>

SearchScene::SearchScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
    , dinoDomes(":/startResources/dinodomes.png")
    , startBackdrop(":/startResources/start_backdrop_sign.png")
    , background(":/background.png")
    , foreground(":/foreground.png")
    , otherForeground(":/foreground.png")
    , cactus(":/cactus.png")
    , rightIdleCharacter(":/idle.png")
    , rightStep1Character(":/step1.png")
    , rightStep2Character(":/step2.png")
    , museum(":/museum.png")
{
    background = background.scaled(1080, 720);
    foreground = foreground.scaled(1080, 720, Qt::IgnoreAspectRatio);
    otherForeground = foreground.scaled(1080, 720, Qt::IgnoreAspectRatio);
    cactus = cactus.scaled(130, 220, Qt::IgnoreAspectRatio);
    museum = museum.scaled(450, 550, Qt::KeepAspectRatio);
    dinoDomes = dinoDomes.scaled(1000, 1000, Qt::KeepAspectRatio);
    startBackdrop = startBackdrop.scaled(950, 200, Qt::IgnoreAspectRatio);

    setupBox2D();

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

    digImage = QPixmap(":/digsite.png");
    digImage = digImage.scaled(80, 75, Qt::IgnoreAspectRatio);

    QFont body("Copperplate Gothic Bold", 20);
    painter.setFont(body);
    painter.setPen(QColor(255, 215, 0));

}

void SearchScene::setupBox2D(){
    b2Vec2 gravity(0.0f, -10.0f);
    world = new b2World(gravity);


    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);


    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 20.0f);
    startBody = world->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.57f;
    startBody->CreateFixture(&fixtureDef);
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
    if(activated) {
        return;
    }
    qDebug() << "activating search scene";
    printDinosaur();


    QMap<DinosaurBone, QPixmap> foundBones = player->getAllFoundBoneImages(player->currentDinosaur);
    qDebug() << "Size of found bones" << foundBones.size();
    digSoundPlayed = false;
    activated = true;
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
    // SPAWN THE BONE CLOSE FOR TESTING
    digLocationX = 700;
    qDebug() << digLocationX;
}

void SearchScene::deactivate(){
    activated= false;
}

void SearchScene::processPlayerInput(){
    if(startAllowed && !startPressed){
        if (player->getInput() == KeyStroke::interactKey) {
            startPressed = true;
        }
    }
    else if(startPressed){
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
}

void SearchScene::updatePlayerMovement(){

    // Detects idle state and returns
    if(direction == idleRight || direction == idleLeft){
        isMoving = false;
        currentCharacter = direction == idleRight ? rightIdleCharacter : leftIdleCharacter;
        spriteMovementIndex = 0;
        movementFrameCounter = 0;
        return;
    }

    // Detects changes in direction for flipping the sprite
    if (((prevDirection == right || prevDirection == idleRight) && direction == left) || ((prevDirection == left || prevDirection == idleLeft) && direction == right)){
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
    int directionX = direction == right ? -2 : 2;

    foregroundX += directionX;
    otherForegroundX += directionX;
    digLocationX += directionX;

    museumX += directionX;

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

void SearchScene::checkMuseumCollision(){
     if (abs(540 - museumX) <= 300) {
        museumX = 180;
        *currentScene = museumPtr;
        player->soundEffects.enqueue(SoundEffect::door);
        deactivate();
    }
}


void SearchScene::checkDigCollision(){
    if (abs(540 - digLocationX) <= 50) {

        if(!digSoundPlayed){
            digSoundPlayed = true;
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
    if(startPressed){
        updatePlayerMovement();
        updateForeground();
        checkDigCollision();

        painter.drawPixmap(0, 0, background);
        painter.drawPixmap(otherForegroundX, 0, otherForeground);
        painter.drawPixmap(foregroundX, 0, foreground);
        painter.drawPixmap(otherForegroundX, 415, otherForeground);
        painter.drawPixmap(foregroundX, 415, foreground);
        int cactiSpacerX = foregroundX + 700;
        int secondCactiSpacerX = otherForegroundX + 560;
        painter.drawPixmap(cactiSpacerX, 265, cactus);
        painter.drawPixmap(secondCactiSpacerX, 265, cactus);

        painter.drawPixmap(museumX, 110, museum);

        painter.drawPixmap(520, 320, currentCharacter);


        painter.drawPixmap(digLocationX, 390, digImage);

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
        checkMuseumCollision();
    }
    else{
        painter.drawPixmap(0, 0, background);
        painter.drawPixmap(otherForegroundX, 0, otherForeground);
        painter.drawPixmap(foregroundX, 0, foreground);
        painter.drawPixmap(otherForegroundX, 415, otherForeground);
        painter.drawPixmap(60, 240, startBackdrop);

        // Get position of startBody and draw the dinodomes image there
        b2Vec2 startVec = startBody->GetPosition();
        if (isBodyStill(startBody, 0.01)) {
            painter.drawText(380, 630, "Press F to start");
            startAllowed = true;
        }
        float scaleFactor = 55;
        int drawPosX = startVec.x * scaleFactor;
        int drawPosY = (startVec.y * scaleFactor) + 360;
        painter.drawPixmap(drawPosX + 40, -drawPosY + 400, dinoDomes);
        world->Step(1/60.0f, 8, 3);
    }
}

bool SearchScene::isBodyStill(b2Body* body, float threshold) {
    b2Vec2 linearVelocity = body->GetLinearVelocity();
    float speed = linearVelocity.Length();
    return (speed < threshold);
}

void SearchScene::drawUI(){
    int count = 0;
    int xVal = 800;

    QMap<DinosaurBone, QPixmap> foundBones = player->getAllFoundBoneImages(player->currentDinosaur);


    for(auto i = foundBones.begin(); i != foundBones.end(); i++){
        i.value() = i.value().scaled(50,50);
        painter.drawPixmap((xVal + 55 * count), 640, i.value());
        count++;
    }

    while (count < 4){
        //PLACEHOLDER DLT LATER
        QPixmap placeholder(":/placeholder.jpg");
        painter.drawPixmap((xVal + 55 * count), 640, placeholder.scaled(50, 50));
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

    if (player->currentBone == DinosaurBone::tail){
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
