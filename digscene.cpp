#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "digscene.h"
#include "searchscene.h"
#include "museumscene.h"

///Reviewed by: Hudson Bowman

DigScene::DigScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
    , digPrompt(":/images/digPrompt.png")
    , goToMuseum(":/images/goToMuseum.png")
    , fontMetrics(QFont("Copperplate Gothic Bold", 20))
{

    //initalize animation frame as -1 meaning not playing
    animationFrame = -1;
    //start brush animation position at the beginning
    brushPosition = 0;

    //setup text font properties
    QFont body("Copperplate Gothic Bold", 18);
    painter.setFont(body);
    painter.setPen(QColor(255, 215, 0));

    //loads all of the json facts to be displayed
    loadFacts(":/settingsFiles/json/tRexFacts.json", tRexFacts);
    loadFacts(":/settingsFiles/json/brontosaurusFacts.json", brontosaurusFacts);
    loadFacts(":/settingsFiles/json/triceratopsFacts.json", triceratopsFacts);
}

void DigScene::initializePointers(SearchScene &searchScene, MuseumScene &museumScene){
    //allows for control flow to be passed from other scenes into the dig scene
    searchPtr = &searchScene;
    museumPtr = &museumScene;
}

QPixmap DigScene::buildScene(){

    activate();
    //when scene is built the player has found a bone
    player->boneFound = true;

    //handle player input
    switch (player->getInput()) {
    case KeyStroke::museumKey:
        //dont allow player to go to the museum until they have dug up the bone and seen the fact
        if (!showBone){
            break;
        }

        //else switch the scene to the museum and deactivate this scene
        *currentScene = museumPtr;
        deactivate();
        break;
        case KeyStroke::interactKey:
        //if the player presses interact and the animation has not begun, start the animation
        if(!animationLock){
            //set animationFrame to 0 meaning it has just started
            animationFrame = 0;
            brushPosition = 0;
            showBone = false;
        }
        break;
    default:
        break;
    }
    //reset input to nothing
    player->setInput(KeyStroke::none);

    QPixmap background(":/images/dirt.png");
    //scale backgound with screen size and draw to the screen
    background = background.scaled(1080, 720, Qt::IgnoreAspectRatio);
    painter.drawPixmap(0, 0, background);

    //on the first frame of the animation play the brush sound
    if (animationFrame == 0){
        player->soundEffects.enqueue(SoundEffect::brush);
    }

    //if the animation is playing
    if(animationFrame > -1){
        //lock the player input
        animationLock = true;
        //increase the frame
        animationFrame++;
        //moves the brush position up or down based on the current frame
        (animationFrame%60<30)?brushPosition--:brushPosition++;

        QPixmap brush(":/images/brush.png");
        brush = brush.scaled(250, 250);

        //calculates the position of the brush on the screen
        int xpos = 1080/2-125+brushPosition*5;
        int ypos = 720/2-125+brushPosition*5;
        //calculates how transparent to make the dinosaur
        //this makes it appear to become less transparent ans the animation progresses, causing a dig affect
        //for the first 60 frames (1 second) it will not show, and for the next 2 seconds it will gradually appear
        float transparency = animationFrame > 60?((double)animationFrame-60)/120.0:0;

        //displays the bone with the given transparency
        displayBone(transparency);

        //displays the brush over the bone
        painter.drawPixmap(xpos, ypos, brush);

        //if the end of the animation is reached, stop the animation
        if(animationFrame > 180){   //display animation for 180 frames (3 seconds)
            //sets animation frame to -1 meaning not playing
            animationFrame = -1;
            //show the bone
            showBone = true;
            //un-lock the player input
            animationLock = true;
        }
    }
    //else if the animation is not playing
    else{
        //if the bone is being shown, the animation has already been played
        if(showBone){
            //paint the bone
            painter.setPen(QColor(255, 215, 0));
            QPixmap bone = player->getDigBone();
            bone = bone.scaled(400, 400);
            painter.drawPixmap(340, 160, bone);

            //paint go to musuem text prompt
            painter.drawPixmap(240, 575, goToMuseum);

            //initalize the fact as error in case it cant load
            QString fact = "Error loading fact";

            //try to get the fact for the current dinosaur/bone
            switch(player->currentDinosaur){
            case DinosaurName::brontosaurus:
                fact = brontosaurusFacts[player->currentBone];
                break;
            case DinosaurName::tRex:
                fact = tRexFacts[player->currentBone];
                break;
            case DinosaurName::triceratops:
                fact = triceratopsFacts[player->currentBone];
                break;
            default:break;
            }

            //creates a bounding box for the text fact
            QRect textRect(50, 25, 980, 125);

            //draws a black background for the fact box
            painter.setBrush(QColor(Qt::black));
            painter.setPen(QColor(Qt::black));
            painter.drawRect(46, 21, textRect.width()+8,textRect.height()+8);

            //creates a white backdrop for the fact
            painter.setBrush(QColor(Qt::white));
            painter.setPen(QColor(Qt::white));
            painter.fillRect(textRect, Qt::white);

            //paint the fact to the box
            painter.setPen(QColor(Qt::black));
            painter.drawText(textRect, Qt::TextWordWrap, fact);

        } else {
            //if the animation has not been played, display text to prompt the user to dig
            painter.setPen(QColor(255, 215, 0));
            painter.drawPixmap(340, 50, digPrompt);
        }
    }

    return frame;
}

void DigScene::activate(){
    //if the scene is already activated, do nothing
    if (activated){
        return;
    }

    //sets up the scene
    activated = true;
    showBone = false;
    animationLock = false;

    animationFrame = -1;
    brushPosition = 0;
}

void DigScene::deactivate(){
    activated = false;
}


void DigScene::displayBone(float percentTransparency){
    //get image for the bone and paint it on frame
    QPixmap bone = player->getDigBone();

    QPixmap img(bone.size());
    img.fill(Qt::transparent);
    //  https://www.qtcentre.org/threads/51158-setting-QPixmap-s-alpha-channel

    //logic for making the pixmap transparent
    QPainter p(&img);
    p.setOpacity(percentTransparency);
    p.drawPixmap(0, 0, bone);
    p.end();

    painter.drawPixmap(340, 160, img.scaled(400, 400));
}

void DigScene::loadFacts(QString resourcePath, QMap<DinosaurBone, QString> &result) {
    //loads the facts from the json file into the arrays
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray fileContent = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileContent);
    if (doc.isNull() || !doc.isObject()) {
        return;
    }
    try {
        QJsonObject obj = doc.object();
        result[DinosaurBone::body] = obj["body"].toString();
        result[DinosaurBone::head] = obj["head"].toString();
        result[DinosaurBone::legs] = obj["legs"].toString();
        result[DinosaurBone::tail] = obj["tail"].toString();
    } catch (...) {
        result.clear();
    }
}
