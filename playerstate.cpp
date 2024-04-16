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
    QMap<DinosaurBone, QPixmap> brontosaurusBones;
    QMap<DinosaurBone, QPixmap> triceratopsBones;

    // load Trex
    QPixmap temp = QPixmap(":/TEMP_tRex.png");
    temp = temp.scaled(300, 300, Qt::KeepAspectRatio);
    tRexBones[DinosaurBone::head] = temp;
    tRexBones[DinosaurBone::body] = temp;
    tRexBones[DinosaurBone::legs] = temp;
    tRexBones[DinosaurBone::arms] = temp;
    tRexBones[DinosaurBone::digBone] = temp;


    temp = QPixmap(":/TEMP_brontosaurus.png");
    temp = temp.scaled(300, 300, Qt::KeepAspectRatio);
    brontosaurusBones[DinosaurBone::head] = temp;
    brontosaurusBones[DinosaurBone::body] = temp;
    brontosaurusBones[DinosaurBone::legs] = temp;
    brontosaurusBones[DinosaurBone::arms] = temp;
    brontosaurusBones[DinosaurBone::digBone] = temp;

    temp = QPixmap(":/TEMP_triceratops.png");
    temp = temp.scaled(300, 300, Qt::KeepAspectRatio);

    QPixmap head = QPixmap(":/triceratopsHead.png");
    temp = temp.scaled(300, 300, Qt::KeepAspectRatio);

    triceratopsBones[DinosaurBone::head] = head;
    triceratopsBones[DinosaurBone::body] = temp;
    triceratopsBones[DinosaurBone::legs] = head;
    triceratopsBones[DinosaurBone::arms] = head;
    triceratopsBones[DinosaurBone::digBone] = head;

    // Adds the dinosaurs to a QMap of dinos
    dinosaurs[DinosaurName::tRex] = Dinosaur(tRexBones);
    //TODO: ADD BONE IMAGES FOR DINO 1 SAME AS TREXBONES
    dinosaurs[DinosaurName::brontosaurus] = Dinosaur(brontosaurusBones);
     //TODO: ADD BONE IMAGES FOR DINO 2 SAME AS TREXBONES
    dinosaurs[DinosaurName::triceratops] = Dinosaur(triceratopsBones);
}

QPixmap PlayerState::getDigBone(DinosaurName dinosaur){
    return dinosaurs[dinosaur].bones[DinosaurBone::digBone];
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

void PlayerState::nextDinosaur(){
    if (unfoundDinosaurs.size() == 0){
        gameOver = true;
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
