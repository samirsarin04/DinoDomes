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

///@brief the header file for the search scene of DinoDomes
/// reviewed by: Ethan Heathcote
class PlayerState : public QObject
{
    Q_OBJECT
public:
    ///@brief constructs a PlayerState object
    ///@param parent the parent QObject
    explicit PlayerState(QObject *parent = nullptr);

    ///@brief This will cycle to the next bone and set boneFound to false again, if the dino is complete, it cycles to the next dinosaur
    void nextBone();

    ///@brief Cycles to the next dinosaur
    void nextDinosaur();

    ///@brief Indicates if the queried dinosaur is complete
    ///@param dinosaur the name of the dinosaur
    ///@return bool if the dino is complete or not
    bool isComplete(DinosaurName dinosaur);

    ///@brief Ability to get all bones that have been found for a specific dinosaur
    ///@param dinosaur the name of the dinosaur
    /// Does NOT include the currentBone, as it gets added when nextBone is called
    /// Complete dinosaurs will automatically return all bone images
    ///@return QMap of the found bone images for museum
    QMap<DinosaurBone, QPixmap> getAllFoundBoneImages(DinosaurName dinosaur);

    ///@brief Gets an image of the current bone for museum
    ///@return QPixmap of the current bone
    QPixmap getCurrentBone();

    ///@brief Gets an image of the current bone found for digging
    ///@return QPixmap of the current digbone
    QPixmap getDigBone();

    ///@brief Gets images of all the dig bones unconvered so far
    ///@param dinosaur the name of the dinosaur
    ///@return QMap of the uncovered dig bones
    QMap<DinosaurBone, QPixmap> getAllFoundDigBones(DinosaurName dinosaur);

    ///@brief returns the silhouette for a given dinosaur
    ///@param dinosaur the name of the dinosaur
    ///@return QPixmap of silhouette
    QPixmap getSilhouette(DinosaurName dinosaur);

    ///@brief stores the players most recent input
    ///@param key the keystroke made by the player
    void setInput(KeyStroke key);

    ///@brief returns the most recent keystroke of the player
    ///@return most recent keystroke
    KeyStroke getInput();

    ///@brief Resets the game logic back to a new state
    void resetGame();

    ///@brief used for locking frame generation
    std::mutex lock;
    ///@brief used for locking the reading/changing of inputs
    std::mutex inputLock;

    ///@brief This is the map that holds the dinosaur objects
    QMap<DinosaurName, Dinosaur> dinosaurs;
    ///@brief These are dinosaurs that the user has not encountered yet
    QVector<DinosaurName> unfoundDinosaurs;
    ///@brief These are ONLY the complete dinosaurs in the musuem
    /// currentDinosaur is NOT in either vector
    QVector<DinosaurName> completeDinosaurs;
    ///@brief Used to hold silhouettes of the dinosaurs
    QMap<DinosaurName, QPixmap> dinosaurUI;
    ///@brief used to hold sound effects that will be played during this frame
    QQueue<SoundEffect> soundEffects;
    ///@brief tracks the user's most recent input
    KeyStroke currentInput;
    ///@brief The current dinosaur that the user is finding / learning more about
    DinosaurName currentDinosaur;
    ///@brief The current bone that the user is finding / learning more about
    DinosaurBone currentBone;
    ///@brief Tracks if the game is over, set to true in a method when all dinos have been found
    bool gameOver;
    ///@brief This indicates if the current bone has been found or if the player is still searching for it
    /// for example if they teleport to the museum and the currentBone is not found, nothing would happen
    bool boneFound;

private:
    ///@brief Used to randomize the dinosaurs when a new game is selected
    void shuffleDinosaurs();
    ///@brief Creates the dinosaur objects and puts them into the map
    void initializeDinosaurs();

};

#endif // PLAYERSTATE_H
