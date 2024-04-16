#ifndef DINOSAUR_H
#define DINOSAUR_H

#include <QPixmap>
#include <QVector>
#include <QMap>
#include "dinosaurbone.h"

class Dinosaur
{
public:
    Dinosaur();
    Dinosaur(QMap<DinosaurBone, QPixmap> bones, QMap<DinosaurBone, QPixmap> digBones);

    // This stores all of the images for each dino bone
    QMap<DinosaurBone, QPixmap> bones;

    QMap<DinosaurBone, QPixmap> digBones;

    // These are all the bones that are not found
    // The bone that the user is currently looking for is NOT in this vector
    QVector<DinosaurBone> unfoundBones;

    // This is a map (dictionary) of all the bones found thus far
    // Useful for drawing the bones that have been found by the user
    QMap<DinosaurBone, QPixmap> foundBones;

    // This indicates if the dino has been solved or all bones have been found
    bool complete;

    // This returns the next dinosaur bone that will be set as the currentBone
    // It also adds the foundBone to to the foundBones Map
    // If there is no next bone, none is returned
    DinosaurBone getNextBone(DinosaurBone foundBone);

    //This returns all images in the foundBones map or all bones if the dinosaur is complete
    QMap<DinosaurBone, QPixmap> getBoneImages();

    QPixmap getDigBone(DinosaurBone bone);

    // This returns the image associated with a bone for the dinosaur
    QPixmap getBoneImage(DinosaurBone bone);


private:
    void shuffleUnfoundBones();
};

#endif // DINOSAUR_H
