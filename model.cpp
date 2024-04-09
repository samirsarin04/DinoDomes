#include "model.h"
#include "digscene.h"
#include "museumscene.h"
#include "searchscene.h"
#include "keystroke.h"


#include <QDebug>

Model::Model(QObject *parent)
    : QObject{parent}
{
    // PASS STATE VARIABLES BY REFERENCE INTO CONSTRUCTORS FROM MODEL SO THEY ARE SHARED BETWEEN ALL CLASSES
    // DigScene digScene(currentDinosaur, bonesArray, currentScene, currentFrame etc...);
    //MuseumScene museumScene(currentDinosaur, bonesArray, currentScene, currentFrame etc...);
    //SearchScene searchScene(currentDinosaur, bonesArray, currentScene, currentFrame etc...);

    currentScene = search;

    connect(&timer, &QTimer::timeout, this, &Model::newFrameTick);

    timer.setInterval(34);
    timer.start();
}

void Model::handleKeyPress(KeyStroke key){
    Scene before = currentScene;

    switch (currentScene){
    case dig:
        digScene.keyPress(key);
        break;
    case search:
        searchScene.keyPress(key);
        break;
    case musuem:
        museumScene.keyPress(key);
        break;
    default:
        return;
    }

    // Sets state for new scene if scene detection set...maybe not neccessary?
    // I think (hope) this won't be neccessary
    detectSceneChange(before);
}

void Model::detectSceneChange(Scene before){
    if (before == currentScene){
        //qDebug() << "No scene change";
        return;
    }

    switch (currentScene){
    case dig:
        digScene.activate();
        break;
    case search:
        searchScene.activate();
        break;
    case musuem:
        museumScene.activate();
        break;
    default:
        return;
    }
}


// EVERY SCENE SHARES A SINGLE FRAME WHICHEVER ONE IS ACTIVE IS THE ONE UPDATING / BROADCASTING THE QIMAGE
// THIS IS ACTIVELY TICKING AT 30FPS
void Model::newFrameTick(){

    // Possible switch needed to generate frames when no actions are occurring

    emit sendFrameToView(currentFrame);
}
