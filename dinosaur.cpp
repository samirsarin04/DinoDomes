#include "dinosaur.h"

#include <QDebug>

Dinosaur::Dinosaur() {
   // shuffleUnfoundBones();
}

Dinosaur::Dinosaur(QMap<DinosaurBone, QPixmap> bones, QMap<DinosaurBone, QPixmap> digBones)
    : bones(bones)
    , digBones(digBones)
    , complete(false)
    , foundBones()
{
    shuffleUnfoundBones();
}

QPixmap Dinosaur::getDigBone(DinosaurBone bone){
    return digBones[bone];
}

void Dinosaur::shuffleUnfoundBones(){

    // Adds the bones to the unfoundBones vector
    unfoundBones.append(DinosaurBone::head);
    unfoundBones.append(DinosaurBone::body);
    unfoundBones.append(DinosaurBone::tail);
    unfoundBones.append(DinosaurBone::legs);

    // Shuffles the Dinosaur Bones so they can be found in any order
    for(int i = 0; i < unfoundBones.size(); i++){
        int newIndex = rand() % unfoundBones.size();
        DinosaurBone tempBone = unfoundBones[newIndex];
        unfoundBones[newIndex] = unfoundBones[i];
        unfoundBones[i] = tempBone;
    }
}

DinosaurBone Dinosaur::getNextBone(DinosaurBone foundBone){
    if (foundBone != DinosaurBone::none){
        foundBones[foundBone] = bones[foundBone];
    }

    if (unfoundBones.size() == 0){
        return DinosaurBone::none;
    }

    DinosaurBone nextBone = unfoundBones[0];
    unfoundBones.removeAt(0);

    return nextBone;
}

QMap<DinosaurBone, QPixmap> Dinosaur::getBoneImages(){
    if (complete){
        return bones;
    }

    return foundBones;
}

// This returns the image associated with a bone for the dinosaur
QPixmap Dinosaur::getBoneImage(DinosaurBone bone){
    if (bone == DinosaurBone::none){
        return QPixmap();
    }

    return bones[bone];
}
