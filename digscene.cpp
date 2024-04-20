#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "digscene.h"
#include "searchscene.h"
#include "museumscene.h"


DigScene::DigScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}, fontMetrics(QFont("Copperplate Gothic Bold", 20))
{

    // tRexFacts[DinosaurBone::head] = QPixmap(":/placeholder.jpg");
    // tRexFacts[DinosaurBone::body] = QPixmap(":/placeholder.jpg");
    // tRexFacts[DinosaurBone::legs] = QPixmap(":/placeholder.jpg");
    // tRexFacts[DinosaurBone::tail] = QPixmap(":/placeholder.jpg");
    animationFrame = -1;
    brushPosition = 0;

    QFont body("Copperplate Gothic Bold", 20);
    painter.setFont(body);
    painter.setPen(QColor(255, 215, 0));

    loadFacts(":/settingsFiles/tRexFacts.json", tRexFacts);
    loadFacts(":/settingsFiles/brontosaurusFacts.json", brontosaurusFacts);
    loadFacts(":/settingsFiles/triceratopsFacts.json", triceratopsFacts);

    // --------------------- FACTS TEST CASE --------------------- //
    qDebug() <<tRexFacts[DinosaurBone::head];
    qDebug() << brontosaurusFacts[DinosaurBone::body];
    qDebug() << triceratopsFacts[DinosaurBone::legs];
    qDebug() << tRexFacts[DinosaurBone::tail] << "\n";
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
            painter.setPen(QColor(255, 215, 0));
            QPixmap bone = player->getDigBone();
            bone = bone.scaled(400, 400);
            painter.drawPixmap(340, 160, bone);

            painter.drawText(330, 630, "Congratulations! You found a bone.");
            painter.drawText(350, 660, "Press \"m\" to go to the museum");

            QString fact = "Error loading fact";
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

            //centers the fact text
            QRect textRect = fontMetrics.boundingRect(fact);
            // Calculate the coordinates to center the text
            int x = 540-textRect.width()/2; // centerX is your desired X coordinate
            int y = 100+textRect.height()/2; // centerY is your desired Y coordinate
            painter.setBrush(QColor(Qt::black));
            painter.setPen(QColor(Qt::black));
            painter.drawRect(x-8,y-textRect.height()-3,textRect.width()+16,textRect.height()+16);

            painter.setBrush(QColor(Qt::white));
            painter.setPen(QColor(Qt::white));
            painter.drawRect(x-5,y-textRect.height(),textRect.width()+10,textRect.height()+10);

            painter.setPen(QColor(Qt::black));
            painter.drawText(x, y, fact);

        } else {
            painter.setPen(QColor(255, 215, 0));
            painter.drawText(375, 120, "Press \"f\" to dig for bones.");
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
    animationLock = false;

    animationFrame = -1;
    brushPosition = 0;
    //logic for initializing the scene
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

    QPainter p(&img);
    p.setOpacity(percentTransparency);
    p.drawPixmap(0, 0, bone);
    p.end();

   // QPixmap output = QPixmap::fromImage(image);

   // output = output.scaled(400, 400);
    painter.drawPixmap(340, 160, img.scaled(400, 400));
}

void DigScene::loadFacts(QString resourcePath, QMap<DinosaurBone, QString> &result) {
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
