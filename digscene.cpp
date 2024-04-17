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

        switch (player->getInput()) {
        case KeyStroke::museumKey:
            *currentScene = museumPtr;
            deactivate();
            qDebug() << "museum key: dig";
            break;
        case KeyStroke::interactKey:
            qDebug() << "interact key:dig";
            if(!animationLock){
                animationFrame = 0;
                brushPosition = 0;
                showBone = false;
            }
            break;
        default:
            break;
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
        float transparency = animationFrame > 60?((double)animationFrame-60)/120.0:0;
        displayBone(transparency);
        painter.drawPixmap(xpos, ypos, brush);
        if(animationFrame > 180){   //display animation for 180 frames (3 seconds)
            animationFrame = -1;
            //show bone that was found and handle that stuff
            showBone = true;
            animationLock = true;
        }
    }
    else{
        if(showBone){
            QPixmap bone = player->getDigBone();
            bone = bone.scaled(400, 400);
            painter.drawPixmap(340, 160, bone);
        }
    }

    return frame;
}

void DigScene::activate(){
    if (activated){
        return;
    }

    activated = true;
    showBone = false;
    //logic for initializing the scene
}

void DigScene::deactivate(){
    activated = false;
}


void DigScene::displayBone(float percentTransparency){
    //get image for the bone and paint it on frame
    QPixmap bone = player->getDigBone();

    QImage image(bone.size(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter p(&image);
    p.setOpacity(percentTransparency);
    p.drawPixmap(0, 0, bone);
    p.end();

    QPixmap output = QPixmap::fromImage(image);

    output = output.scaled(400, 400);
    painter.drawPixmap(340, 160, output);
}
