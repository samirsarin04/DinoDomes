#include "digscene.h"
#include "searchscene.h"
#include "museumscene.h"


DigScene::DigScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
{

    tRexFacts[DinosaurBone::head] = QPixmap(":/placeholder.jpg");
    tRexFacts[DinosaurBone::body] = QPixmap(":/placeholder.jpg");
    tRexFacts[DinosaurBone::legs] = QPixmap(":/placeholder.jpg");
    tRexFacts[DinosaurBone::tail] = QPixmap(":/placeholder.jpg");
    animationFrame = -1;
    brushPosition = 0;

}

void DigScene::initializePointers(SearchScene &searchScene, MuseumScene &museumScene){
    searchPtr = &searchScene;
    museumPtr = &museumScene;
}

QPixmap DigScene::buildScene(){

    activate();

    player->boneFound = true;

    if(!animationLock){
        switch (player->getInput()) {
        case KeyStroke::museumKey:
            *currentScene = museumPtr;
            deactivate();
            qDebug() << "museum key: dig";
            break;
        case KeyStroke::moveLeftKey:
            qDebug() << "left key: dig";
            break;
        case KeyStroke::moveRightKey:
            qDebug() << "right key: SWITCHING FROM DIG TO SEARCH";
            //player->lock.lock();
            // TEMP FOR TESTING
            // player->nextBone();
            //*currentScene = searchPtr;
            //player->lock.unlock();
            break;
        case KeyStroke::interactKey:
            qDebug() << "interact key:dig";
            animationFrame = 0;
            brushPosition = 0;
            break;
        default:
            break;
        }
    }
    player->setInput(KeyStroke::none);
    //replace background with a dirt png or something
    QPixmap background(":/dirt.png");
    background = background.scaled(1080, 720, Qt::IgnoreAspectRatio);
    painter.drawPixmap(0, 0, background);

    if(animationFrame > -1){
        animationLock = true;
        animationFrame++;
        (animationFrame%60<30)?brushPosition--:brushPosition++;
        QPixmap brush(":/brush.png");
        brush = brush.scaled(250, 250);
        int xpos = 1080/2-125+brushPosition*5;
        int ypos = 720/2-125+brushPosition*5;
        painter.drawPixmap(xpos, ypos, brush);
        if(animationFrame > 180){   //display animation for 180 frames (3 seconds)
            animationFrame = -1;
            //show bone that was found and handle that stuff
            displayBone(DinosaurName::tRex, DinosaurBone::tail);
        }
    }
    else{
        animationLock = false;
    }

    return frame;
}

void DigScene::activate(){
    if (activated){
        return;
    }

    activated = true;
    //logic for initializing the scene
}

void DigScene::deactivate(){
    activated = false;
}


void DigScene::displayBone(DinosaurName name, DinosaurBone bone){
    //get image for the bone and paint it on frame
}
