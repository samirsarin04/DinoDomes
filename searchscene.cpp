#include "searchscene.h"
#include "digscene.h"
#include "museumscene.h"
#include <Box2D/Box2D.h>
#include <QDebug>
#include <set>

SearchScene::SearchScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
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
    painter.setBrush(Qt::white);
}

#define CHECK_MAT_VS_MAT(mat1, mat2)\
bool contactIs##mat1##Vs##mat2(b2Contact* contact) {\
        b2Fixture* fA = contact->GetFixtureA();\
        b2Fixture* fB = contact->GetFixtureB();\
        if ( fixtureIs##mat1(fA) && fixtureIs##mat2(fB) )\
        return true;\
        if ( fixtureIs##mat1(fB) && fixtureIs##mat2(fA) )\
        return true;\
        return false;\
}

void SearchScene::setupBox2D(){
    characterBody.type = b2_staticBody; //this will be a static body
    characterBody.position.Set(520, 320); //set the starting position
    characterBody.angle = 0; //set the starting angle
    staticCharBody = world.CreateBody(&characterBody); //creates the body within the world

    foreground1Body.type = b2_dynamicBody; //this will be a dynamic body
    foreground1Body.position.Set(0, 415); //set starting position
    foreground1Body.angle = 0; //set the starting angle
    dynForegrnd1Body = world.CreateBody(&foreground1Body); //creates the body within the world

    foreground2Body.type = b2_dynamicBody; //this will be a dynamic body
    foreground2Body.position.Set(foregroundX, 415); //set starting position
    foreground2Body.angle = 0; //set the starting angle
    dynForegrnd2Body = world.CreateBody(&foreground1Body); //creates the body within the world

    charBox.SetAsBox(200, 120);
    staticCharBody->CreateFixture(&charBox, 0.02f);

    foregrnd1Box.SetAsBox(1080, 720);
    foregrnd1Fixture.shape = &foregrnd1Box;
    foregrnd1Fixture.friction = 0.4f;
    foregrnd1Fixture.density = 0.0f;
    dynForegrnd1Body->CreateFixture(&foregrnd1Fixture);

    foregrnd2Box.SetAsBox(1080, 720);
    foregrnd2Fixture.shape = &foregrnd2Box;
    foregrnd2Fixture.friction = 0.4f;
    foregrnd2Fixture.density = 0.01f;
    dynForegrnd2Body->CreateFixture(&foregrnd2Fixture);
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
    //digLocationX = rand() % 1000 + 1000;

    // SPAWN THE BONE CLOSE FOR TESTING
    digLocationX = 700;
    qDebug() << digLocationX;
}

void SearchScene::deactivate(){
    activated= false;
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
        dynForegrnd1Body->SetLinearVelocity( b2Vec2(0, 0)); //moving up and left 0 units per second
        dynForegrnd2Body->SetLinearVelocity( b2Vec2(0, 0)); //moving up and left 0 units per second
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
    int directionX = direction == right ? -2 : 2;

    foregroundX += directionX;

    dynForegrnd1Body->SetLinearVelocity( b2Vec2(0, directionX));

    otherForegroundX += directionX;

    dynForegrnd2Body->SetLinearVelocity( b2Vec2(0, directionX));

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

bool contactIsCharVsFG1(b2Contact* contact) {
    b2Fixture* fA = contact->GetFixtureA();
    b2Fixture* fB = contact->GetFixtureB();
    if (fA->GetDensity() == 0.02f && fB->GetDensity() == 0.0f)
        return true;
    if (fB->GetDensity() == 0.02f && fA->GetDensity() == 0.0f)
        return true;
    return false;
}

bool contactIsCharVsFG2(b2Contact* contact) {
    b2Fixture* fA = contact->GetFixtureA();
    b2Fixture* fB = contact->GetFixtureB();
    if (fA->GetDensity() == 0.02f && fB->GetDensity() == 0.01f)
        return true;
    if (fB->GetDensity() == 0.02f && fA->GetDensity() == 0.01f)
        return true;
    return false;
}

void BeginContact(b2Contact* contact){
    if (contactIsCharVsFG1(contact)){}
        // charToFG1Contacts.insert(contact);
    if (contactIsCharVsFG2(contact)){}
        // charToFG2Contacts.insert(contact);
}

void EndContact(b2Contact* contact){
    if (contactIsCharVsFG1(contact)){}
        // charToFG1Contacts.erase(contact);
    if (contactIsCharVsFG2(contact)){}
        // charToFG2Contacts.erase(contact);
}

void SearchScene::updateWorld(){
    updatePlayerMovement();
    updateForeground();

    checkDigCollision();

    painter.drawPixmap(0, 0, background);
    painter.drawPixmap(otherForegroundX, 0, otherForeground);
    painter.drawPixmap(foregroundX, 0, foreground);
    painter.drawPixmap(otherForegroundX, 415, otherForeground);



    foreground1Body.position.Set(otherForegroundX, 415);
    painter.drawPixmap(foregroundX, 415, foreground);
    foreground2Body.position.Set(foregroundX, 415);
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

    // world.Step(timeStep, 6, 2);

    drawUI();
    checkMuseumCollision();
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
