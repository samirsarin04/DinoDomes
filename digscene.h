#ifndef DIGSCENE_H
#define DIGSCENE_H

#include "scene.h"
#include <QFontMetrics>

/// @brief Header file for the dig scene
/// Used to track the state of found bones, draw images associated with each of the 3 dinosaurs' bones, and to handle the animations associated with the scene
/// Reviewed by: Arjun Sarkar

class DigScene : public Scene
{
public:
    /// @brief Constructor: sets up states and fields for the scene
    explicit DigScene(PlayerState& player, Scene** currentScene, QObject *parent = nullptr);

    /// @brief initializes pointers to the other scenes (museum, search scenes)
    void initializePointers(SearchScene &searchScene, MuseumScene &museumScene);

    /// @brief builds the scene by drawing the background and updates it every frame
    virtual QPixmap buildScene();
private:
    /// @brief Stores a pointer to the search scene
    SearchScene* searchPtr;

    /// @brief Stores a pointer to the museum scene
    MuseumScene* museumPtr;

    /// @brief Stores each T-rex bone as a key and links it to a corresponding fact about the T-rex bone
    QMap<DinosaurBone, QString> tRexFacts;

    /// @brief Stores each brontosaurus bone as a key and links it to a corresponding fact about the brontosaurus bone
    QMap<DinosaurBone, QString> brontosaurusFacts;

    /// @brief Stores each triceratops bone as a key and links it to a corresponding fact about the triceratops bone
    QMap<DinosaurBone, QString> triceratopsFacts;

    /// @brief Stores the image of the dig prompt as a pixmap
    QPixmap digPrompt;

    /// @brief Stores the image of the go to museum prompt as a pixmap
    QPixmap goToMuseum;

    /// @brief keeps track of how long the brush animation has lasted and which frame it is on. Set to -1 if animation is not currently playing
    int animationFrame;

    /// @brief int that keeps track of the brush position
    int brushPosition;

    /// @brief boolean that keeps track of wether the frame is locked or not
    bool animationLock = false;

    /// @brief boolean that keeps track of if the bone is showing or not
    bool showBone = false;

    /// @brief keeps track of font metrics info
    QFontMetrics fontMetrics;

    /// @brief loadFacts: Loads a fact about the found dinosaur bone based on the bone and dinosaur it is from
    /// @param fileName: takes in the name of the json file containing the facts
    /// @param result
    void loadFacts(QString fileName, QMap<DinosaurBone, QString> &result);

    /// @brief displayBone: Draws the current bone as a pixmap with the alpha value set at the value specified in the parameter
    /// @param percentTransparency: takes in the alpha value for the bone to be displayed
    void displayBone(float percentTransparency);
protected:
    /// @brief initializes the dig scene
    virtual void activate();

    /// @brief changes the "activated" boolean to false, makes it so that the dig scene is no longer the current scene
    virtual void deactivate();
};

#endif // DIGSCENE_H
