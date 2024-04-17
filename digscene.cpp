#include "digscene.h"
#include "searchscene.h"
#include "museumscene.h"


DigScene::DigScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
{

    tRexFacts[DinosaurBone::head] = QPixmap(":/placeholder.jpg");
    tRexFacts[DinosaurBone::body] = QPixmap(":/placeholder.jpg");
    tRexFacts[DinosaurBone::legs] = QPixmap(":/placeholder.jpg");
    tRexFacts[DinosaurBone::arms] = QPixmap(":/placeholder.jpg");
    animationFrame = -1;
    brushPosition = 0;

}

void DigScene::initializePointers(SearchScene &searchScene, MuseumScene &museumScene){
    searchPtr = &searchScene;
    museumPtr = &museumScene;
}

QPixmap DigScene::buildScene(){

    player->boneFound = true;

    switch (player->getInput()) {
    case KeyStroke::museumKey:
        *currentScene = museumPtr;
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
    player->setInput(KeyStroke::none);

    //replace background with a dirt png or something
    QPixmap background(":/background.png");
    background = background.scaled(1080, 720);
    painter.drawPixmap(0, 0, background);

    if(animationFrame > -1){
        animationFrame++;
        (animationFrame%60<30)?brushPosition--:brushPosition++;

        QPixmap brush(":/brush.png");
        brush = brush.scaled(150, 150);
        int xpos = 1080/2-50+brushPosition*2;
        int ypos = 720/2-50+brushPosition*2;
        painter.drawPixmap(xpos, ypos, brush);
        if(animationFrame > 120){   //display animation for 120 frames (2 seconds)
            animationFrame = -1;
            //show bone that was found and handle that stuff
            displayBone(DinosaurName::tRex, DinosaurBone::arms);
        }
    }

    return frame;
}


void DigScene::displayBone(DinosaurName name, DinosaurBone bone){
    //get image for the bone and paint it on frame
}
