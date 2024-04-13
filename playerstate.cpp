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

void PlayerState::initializeDinosaurs(){
    // Initializes the art assets for each dinosaur
    // REPEAT THREE TIMES FOR EACH DINOSAUR
    QMap<DinosaurBone, QPixmap> tRexBones;

    tRexBones[DinosaurBone::head] = QPixmap(":/background.png");
    tRexBones[DinosaurBone::body] = QPixmap(":/background.png");
    tRexBones[DinosaurBone::legs] = QPixmap(":/background.png");
    tRexBones[DinosaurBone::arms] = QPixmap(":/background.png");

    // Adds the dinosaurs to a QMap of dinos
    dinosaurs[DinosaurName::tRex] = Dinosaur(tRexBones);

    //TODO: ADD BONE IMAGES FOR DINO 1 SAME AS TREXBONES
    dinosaurs[DinosaurName::dino1] = Dinosaur(tRexBones);
     //TODO: ADD BONE IMAGES FOR DINO 2 SAME AS TREXBONES
    dinosaurs[DinosaurName::dino2] = Dinosaur(tRexBones);
}


void PlayerState::nextBone(){
    boneFound = false;

    if (dinosaurs[currentDinosaur].unfoundBones.size() == 0){
        nextDinosaur();
        return;
    }

    currentBone = dinosaurs[currentDinosaur].getNextBone(currentBone);
}

void PlayerState::nextDinosaur(){
    if (unfoundDinosaurs.size() == 0){
        gameOver = true;
        return;
    }

    dinosaurs[currentDinosaur].complete = true;
    completeDinosaurs.append(currentDinosaur);
    currentDinosaur = unfoundDinosaurs[0];
    unfoundDinosaurs.removeAt(0);

    currentBone = dinosaurs[currentDinosaur].getNextBone(currentBone);
}

// Ability to get all bones that have been found for a specific dinosaur
QMap<DinosaurBone, QPixmap> PlayerState::getAllFoundBoneImages(DinosaurName dinosaur){
    return dinosaurs[dinosaur].getBoneImages();
}

// Gets an image of a specific bone for a dinosaur
QPixmap PlayerState::getSpecificBone(DinosaurName dinosaur, DinosaurBone bone){
    return dinosaurs[dinosaur].bones[bone];
}


void PlayerState::shuffleDinosaurs(){
    unfoundDinosaurs.clear();
    completeDinosaurs.clear();

    unfoundDinosaurs.append(DinosaurName::tRex);
    unfoundDinosaurs.append(DinosaurName::dino1);
    unfoundDinosaurs.append(DinosaurName::dino2);

    srand((unsigned) time(NULL));

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
