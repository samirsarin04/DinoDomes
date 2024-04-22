#ifndef DINOSAUR_H
#define DINOSAUR_H

#include <QPixmap>
#include <QVector>
#include <QMap>
#include "dinosaurbone.h"
///@brief header file for dinosaur objects
/// Used to track the state and store images associated with each of the 3 dinosaurs
/// reviewed by: Ethan Heathcote
class Dinosaur
{
public:
    ///@brief default constructor
    Dinosaur();

    ///@brief constructs the dinosaur object
    Dinosaur(QMap<DinosaurBone, QPixmap> bones, QMap<DinosaurBone, QPixmap> digBones);

    ///@brief this returns the next dinosaur bone that will be set as the currentBone
    ///It also adds the foundBone to to the foundBones Map
    ///If there is no next bone, none is returned
    ///@param the bone that was most recently found
    ///@return the new current bone
    DinosaurBone getNextBone(DinosaurBone foundBone);

    ///@brief This returns all images in the foundBones map or foundDigBones map
    ///@param bool if the foundBones or foundDigBones map is desired
    ///@return QMap of either foundBones or foundDigBones
    QMap<DinosaurBone, QPixmap> getBoneImages(bool isDigBone);

    ///@brief returns the current dig bone image
    ///@param the bone being queried
    ///@return QPixmap of dig bone queried
    QPixmap getDigBone(DinosaurBone bone);

    ///@brief  This returns the museum image associated with a bone for the dinosaur
    ///@param the bone being queried
    ///@return QPixmap of museum bone queried
    QPixmap getBoneImage(DinosaurBone bone);


    ///@brief This stores all of the museum bone images for each dino
    QMap<DinosaurBone, QPixmap> bones;

    ///@brief This stores all of the dig bone images for each dino
    QMap<DinosaurBone, QPixmap> digBones;

    ///@brief These are all the bones that are not found
    /// The bone that the user is currently looking for is NOT in this vector
    QVector<DinosaurBone> unfoundBones;

    ///@brief All the museum bones found thus far
    QMap<DinosaurBone, QPixmap> foundBones;

    ///@brief All the dig bones found thus far
    QMap<DinosaurBone, QPixmap> foundDigBones;

    ///@brief This indicates if all of the bones have been found for this dinosaur
    bool complete;

private:
    ///@brief shuffles the dinosaur bones so they appear in a random order
    void shuffleUnfoundBones();
};

#endif // DINOSAUR_H
