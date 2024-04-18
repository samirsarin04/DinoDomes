#ifndef MUSEUMSCENE_H
#define MUSEUMSCENE_H

#include "scene.h"

class MuseumScene : public Scene
{
public:
    explicit MuseumScene(PlayerState& player, Scene** currentScene, QObject *parent = nullptr);
    virtual QPixmap buildScene();
    void initializePointers(SearchScene &searchScene);
private:
    // This private class represents a singular question
    class Question
    {
    public:
        // The questions to be asked
        QString question;
        // The response when the question is answered
        QString response;
        // a shuffled array of strings of possible answers
        QVector<QString> options;
        // index of correct answer in option
        int correctIndex;
    };

    /// @brief Loads the quiz questions from the given file path.
    /// @param The resource path to the json file holding the questions
    /// @param The list of questions from the file in the given path. Returns
    /// empty list if anything is wrong with the filepath or json structure.
    QVector<MuseumScene::Question> loadQuestions(QString resourcePath);

    //QMap<DinosaurBone, QPixmap> tRexFacts;

    // Maps Dinosaur names to Questions about that dinosaur
    QMap<DinosaurName, QVector<MuseumScene::Question>> questionsMap;

    DinosaurName currentDinosaur;

    SearchScene* searchPtr;
    QPixmap background;

    QPixmap triceratopsSilhouette;
    QPixmap tRexSilhouette;
    QPixmap brontosaurusSilhouette;

    int tRexBaseX;
    int tRexBaseY;
    int brontosaurusBaseX;
    int brontosaurusBaseY;
    int triceratopsBaseX;
    int triceratopsBaseY;

    //Used to track coordinates of dino bones as they are animated
    int animationX;
    int animationY;
    int animationFrameCount;
    int animationDimension;

    //Used to track the final coordinates for each dino bone
    QMap<DinosaurName, QMap<DinosaurBone, QPoint>> dinosaurCoordinates;

    // tracks if the animation is currently occurring
    bool animationActive;

    bool showDinoFact;

    //tracks if the player has answered the quiz
    int playerAnswered = -1;

    //quiz question that it the user is on
    int quizNumber = -1;

    // animates the bone to bounce and then snap to its coordinates
    void animateBone();
    // draws the scene
    void drawWorld();

    void quizGuess(int guess);

    void closeDinoFact();

protected:
    virtual void activate();
    virtual void deactivate();
    virtual void processPlayerInput();
    void switchToSearchScene();
};

#endif // MUSEUMSCENE_H
