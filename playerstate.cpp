#include "playerstate.h"
#include <QVector>

#include <QDebug>


PlayerState::PlayerState(QObject *parent)
    : QObject{parent}
{
    currentInput = KeyStroke::none;

    gameOver = false;
    boneFound = false;

    initializeDinosaurs();
    shuffleDinosaurs();
}

void PlayerState::resetGame(){
    currentInput = KeyStroke::none;

    gameOver = false;
    boneFound = false;

    initializeDinosaurs();
    shuffleDinosaurs();
}

void PlayerState::initializeDinosaurs(){
    // Initializes the art assets for each dinosaur
    // REPEAT THREE TIMES FOR EACH DINOSAUR
    QMap<DinosaurBone, QPixmap> tRexBones;
    QMap<DinosaurBone, QPixmap> brontosaurusBones;
    QMap<DinosaurBone, QPixmap> triceratopsBones;

    QMap<DinosaurBone, QPixmap> tRexDigBones;
    QMap<DinosaurBone, QPixmap> brontosaurusDigBones;
    QMap<DinosaurBone, QPixmap> triceratopsDigBones;

    QPixmap tRexBone = QPixmap(":/images/tRexHead.png").scaled(300, 300);
    tRexBones[DinosaurBone::head] = tRexBone;

    tRexBone = QPixmap(":/images/tRexBody.png").scaled(300, 300);
    tRexBones[DinosaurBone::body] = tRexBone;

    tRexBone = QPixmap(":/images/tRexLegs.png").scaled(300, 300);
    tRexBones[DinosaurBone::legs] = tRexBone;

    tRexBone = QPixmap(":/images/tRexTail.png").scaled(300, 300);
    tRexBones[DinosaurBone::tail] = tRexBone;

    QPixmap tRexDigBone = QPixmap(":/images/tRexDigHead.png").scaled(300, 300);
    tRexDigBones[DinosaurBone::head] = tRexDigBone;

    tRexDigBone = QPixmap(":/images/tRexDigBody.png").scaled(300, 300);
    tRexDigBones[DinosaurBone::body] = tRexDigBone;

    tRexDigBone = QPixmap(":/images/tRexDigLeg.png").scaled(300, 300);
    tRexDigBones[DinosaurBone::legs] = tRexDigBone;

    tRexDigBone = QPixmap(":/images/tRexDigTail.png").scaled(300, 300);
    tRexDigBones[DinosaurBone::tail] = tRexDigBone;

    // Brontosaurus Images
    QPixmap brontosaurusBone = QPixmap(":/images/brontosaurusHead.png").scaled(400,400);
    brontosaurusBones[DinosaurBone::head] = brontosaurusBone;

    brontosaurusBone = QPixmap(":/images/brontosaurusBody.png").scaled(400,400);
    brontosaurusBones[DinosaurBone::body] = brontosaurusBone;

    brontosaurusBone = QPixmap(":/images/brontosaurusLegs.png").scaled(400,400);
    brontosaurusBones[DinosaurBone::legs] = brontosaurusBone;

    brontosaurusBone = QPixmap(":/images/brontosaurusTail.png").scaled(400,400);
    brontosaurusBones[DinosaurBone::tail] = brontosaurusBone;

    QPixmap brontosaurusDigBone = QPixmap(":/images/brontosaurusDigHead.png").scaled(300,300, Qt::KeepAspectRatio);
    brontosaurusDigBones[DinosaurBone::head] = brontosaurusDigBone;

    brontosaurusDigBone = QPixmap(":/images/brontosaurusDigBody.png");
    brontosaurusDigBone = brontosaurusDigBone.scaled(300,300, Qt::KeepAspectRatio);
    brontosaurusDigBones[DinosaurBone::body] = brontosaurusDigBone;

    brontosaurusDigBone = QPixmap(":/images/brontosaurusDigLeg.png").scaled(300,300, Qt::KeepAspectRatio);
    brontosaurusDigBones[DinosaurBone::legs] = brontosaurusDigBone;

    brontosaurusDigBone = QPixmap(":/images/brontosaurusDigTail.png").scaled(300,300, Qt::KeepAspectRatio);
    brontosaurusDigBones[DinosaurBone::tail] = brontosaurusDigBone;

    // Triceratops images

    QPixmap triceratopsBone = QPixmap(":/images/triceratopsHead.png").scaled(300,300);
    triceratopsBones[DinosaurBone::head] = triceratopsBone;

    triceratopsBone = QPixmap(":/images/triceratopsBody.png").scaled(300,300);
    triceratopsBones[DinosaurBone::body] = triceratopsBone;

    triceratopsBone = QPixmap(":/images/triceratopsLegs.png").scaled(300,300);
    triceratopsBones[DinosaurBone::legs] = triceratopsBone;

    triceratopsBone = QPixmap(":/images/triceratopsTail.png").scaled(300,300);
    triceratopsBones[DinosaurBone::tail] = triceratopsBone;

    QPixmap triceratopsDigBone = QPixmap(":/images/triceratopsDigHead.png").scaled(300,300);
    triceratopsDigBones[DinosaurBone::head] = triceratopsDigBone;

    triceratopsDigBone = QPixmap(":/images/triceratopsDigBody.png").scaled(300,300);
    triceratopsDigBones[DinosaurBone::body] = triceratopsDigBone;

    triceratopsDigBone = QPixmap(":/images/triceratopsDigLeg.png").scaled(300,300);
    triceratopsDigBones[DinosaurBone::legs] = triceratopsDigBone;

    triceratopsDigBone = QPixmap(":/images/triceratopsDigTail.png").scaled(300,300);
    triceratopsDigBones[DinosaurBone::tail] = triceratopsDigBone;

    QPixmap ui = QPixmap(":/images/tRexSilhouette.png").scaled(80,80);
    dinosaurUI[DinosaurName::tRex] = ui;

    ui = QPixmap(":/images/brontosaurusSilhouette.png").scaled(80,80);
    dinosaurUI[DinosaurName::brontosaurus] = ui;

    ui = QPixmap(":/images/triceratopsSilhouette.png").scaled(80,80);
    dinosaurUI[DinosaurName::triceratops] = ui;

    // Adds the dinosaurs to a QMap of dinos
    dinosaurs[DinosaurName::tRex] = Dinosaur(tRexBones, tRexDigBones);
    dinosaurs[DinosaurName::brontosaurus] = Dinosaur(brontosaurusBones, brontosaurusDigBones);
    dinosaurs[DinosaurName::triceratops] = Dinosaur(triceratopsBones, triceratopsDigBones);
}

QPixmap PlayerState::getUI(DinosaurName dinosaur){
    return dinosaurUI[dinosaur];
}

QPixmap PlayerState::getDigBone(){
    return dinosaurs[currentDinosaur].getDigBone(currentBone);
}

void PlayerState::nextBone(){
    boneFound = false;

    if (dinosaurs[currentDinosaur].unfoundBones.size() == 0){
        nextDinosaur();
        return;
    }

    currentBone = dinosaurs[currentDinosaur].getNextBone(currentBone);
}

QPixmap PlayerState::getCurrentBone(){
    return dinosaurs[currentDinosaur].getBoneImage(currentBone);
}

bool PlayerState::isComplete(DinosaurName dinosaur){
    if (gameOver){
        return true;
    }

    return dinosaurs[dinosaur].complete;
}

void PlayerState::nextDinosaur(){
    if (unfoundDinosaurs.size() == 0){
        gameOver = true;
        dinosaurs[currentDinosaur].complete = true;
        return;
    }

    dinosaurs[currentDinosaur].complete = true;
    completeDinosaurs.append(currentDinosaur);
    currentDinosaur = unfoundDinosaurs[0];
    unfoundDinosaurs.removeAt(0);

    currentBone = dinosaurs[currentDinosaur].getNextBone(DinosaurBone::none);
}

// Ability to get all bones that have been found for a specific dinosaur
QMap<DinosaurBone, QPixmap> PlayerState::getAllFoundBoneImages(DinosaurName dinosaur){
    return dinosaurs[dinosaur].getBoneImages(false);
}

QMap<DinosaurBone, QPixmap> PlayerState::getAllFoundDigBones(DinosaurName dinosaur){
    return dinosaurs[dinosaur].getBoneImages(true);
}


// Gets an image of a specific bone for a dinosaur
QPixmap PlayerState::getSpecificBone(DinosaurName dinosaur, DinosaurBone bone){
    return dinosaurs[dinosaur].bones[bone];
}


void PlayerState::shuffleDinosaurs(){
    unfoundDinosaurs.clear();
    completeDinosaurs.clear();

    unfoundDinosaurs.append(DinosaurName::tRex);
    unfoundDinosaurs.append(DinosaurName::brontosaurus);
    unfoundDinosaurs.append(DinosaurName::triceratops);

    // Shuffles the Dinosaur Bones so they can be found in any order
    for(int i = 0; i < unfoundDinosaurs.size(); i++){
        int newIndex = rand() % unfoundDinosaurs.size();
        DinosaurName tempDino = unfoundDinosaurs[newIndex];
        unfoundDinosaurs[newIndex] = unfoundDinosaurs[i];
        unfoundDinosaurs[i] = tempDino;
    }

    currentDinosaur = unfoundDinosaurs[0];
    unfoundDinosaurs.removeAt(0);

    currentBone = dinosaurs[currentDinosaur].getNextBone(DinosaurBone::none);
}

void PlayerState::setInput(KeyStroke key){
    inputLock.lock();
    currentInput = key;
    inputLock.unlock();
}

KeyStroke PlayerState::getInput(){
    std::lock_guard<std::mutex> lock(inputLock);
    return currentInput;
}
