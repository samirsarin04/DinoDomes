#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include <QObject>
#include <QQueue>
#include "keystroke.h"
#include <mutex>
#include <QPixmap>
#include <QVector>
#include "dinosaurbone.h"
#include "dinosaurname.h"
#include "dinosaur.h"
#include "soundeffect.h"
#include <QMap>

class PlayerState : public QObject
{
    Q_OBJECT
public:
    explicit PlayerState(QObject *parent = nullptr);

    //Temporary, may use a mechanism like this for sound effects
    QQueue<SoundEffect> soundEffects;
    void setInput(KeyStroke key);
    KeyStroke getInput();
    std::mutex lock;



    // These are dinosaurs that the user has not encountered yet
    QVector<DinosaurName> unfoundDinosaurs;

    // These are ONLY the complete dinosaurs in the musuem
    // currentDinosaur is NOT in either vector
    QVector<DinosaurName> completeDinosaurs;

    // This is the map that holds the dinosaur objects
    QMap<DinosaurName, Dinosaur> dinosaurs;


    /*

    SCENES SHOULD BE ABLE TO SYNC THEIR STATES WITH THESE ATTRIBUTES/METHODS

    ***USE THE METHODS/ATTRIBUTES BELOW***

    DINOSAUR CLASS METHODS DO NOT NEED TO BE DIRECTLY ACCESSED

    YOU CAN PRESS M WHEN THE GAME LAUNCHES TO SIMULATE FINDING BONES

    DINOSAURS / BONES APPEAR IN RANDOM ORDER

    ADD TO THESE METHODS / ATTRIBUTES AS NEEDED
    */

    void resetGame();

    // set to true in a method when all dinos have been found
    bool gameOver;

    // The current dinosaur is stored as this enum
    DinosaurName currentDinosaur;

    // The current bone enum that the user is finding / learning more about
    DinosaurBone currentBone;

    // This indicates if the current bone has been found or if the player is still searching for it
    // for example if they teleport to the museum and the currentBone is not found, nothing would happen
    bool boneFound;

    // This will cycle to the next bone and set boneFound to false again, if the dino is complete, it cycles to the next dinosaur
    // Will probably be invoked by the museum when all bones are found for the current dino
    void nextBone();

    // This will cycle to the next dinosaur if the user guesses the dino early
    void nextDinosaur();

    //Indicates if the queried dinosaur is complete
    bool isComplete(DinosaurName dinosaur);

    // Ability to get all bones that have been found for a specific dinosaur
    // Does NOT include the currentBone, as it gets added when nextBone is called
    // Complete dinosaurs will automatically return all bone images
    // The idea is that the musuem may want to draw attention to the currentBone if boneFound is true
    QMap<DinosaurBone, QPixmap> getAllFoundBoneImages(DinosaurName dinosaur);

    // Gets an image of the current bone
    QPixmap getCurrentBone();


    QPixmap getDigBone();


    QMap<DinosaurBone, QPixmap> getAllFoundDigBones(DinosaurName dinosaur);

    // Gets an image of a specific bone for a dinosaur
    // can be called with getSpecificBone(player->currentDinosaur, player->currentBone)
    // modular for flexibility
    QPixmap getSpecificBone(DinosaurName dinosaur, DinosaurBone bone);

protected:
    std::mutex inputLock;
    KeyStroke currentInput;

private:
    // Used to randomize the dinosaurs when a new game is selected
    void shuffleDinosaurs();
    // Creates the dinosaur objects and puts them into the map
    void initializeDinosaurs();

signals:
};

#endif // PLAYERSTATE_H
