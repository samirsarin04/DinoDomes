#ifndef MUSEUMSCENE_H
#define MUSEUMSCENE_H

#include "scene.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

///@brief header file for the museum scene
/// This is the class that builds the frame for the museum scene
/// Inherits from base class scene
/// reviewed by: Ethan Heathcote
class MuseumScene : public Scene
{
public:
    ///@brief constructor for MuseumScene
    ///@param player object shared between scenes to track state
    ///@param currentScene pointer to a pointer of the currentScene
    ///@param parent parent QObject
    explicit MuseumScene(PlayerState& player, Scene** currentScene, QObject *parent = nullptr);

    ///@brief builds the frame that is sent to the view
    ///@return QPixmap of the current frame in the museum scene
    virtual QPixmap buildScene();

    ///@brief this allows for scenes to pass control flow to other scenes
    ///@param searchScene the searchScene object
    void initializePointers(SearchScene &searchScene);
private:
    ///@brief This private class represents a singular question during the quiz phase
    class Question
    {
    public:
        ///@brief The questions to be asked
        QString question;
        ///@brief The response when the question is answered
        QString response;
        ///@brief a shuffled array of strings of possible answers
        QVector<QString> options;
        ///@brief index of correct answer in option
        int correctIndex;
    };

    /// @brief Loads the quiz questions from the given file path.
    /// @param The resource path to the json file holding the questions
    /// @param The list of questions from the file in the given path. Returns
    /// empty list if anything is wrong with the filepath or json structure.
    QVector<MuseumScene::Question> loadQuestions(QString resourcePath, DinosaurName dinosaur);

    ///@brief performs the logic to animate the bone at the beginning of the scene when a bound is found
    void animateBoneLogic();

    ///@brief draws the bone animation
    void drawBoneAnimation();

    ///@brief draws the museum background, silhouettes and user prompts
    void drawBackgroundAndFoundDinos();

    ///@brief draws the quiz sequence
    void drawQuiz();

    ///@brief draws the dino fact sheet after 4 bones have been found for a dinosaur
    void drawFinalDinoFact();

    ///@brief draws the credit sequence after the game is finished
    void drawCredits();

    ///@brief updates the world, entry point for all other draw methods
    void drawWorld();

    ///@brief processes a users guess during the quiz sequence
    ///@param guess, the int associated with their 1-4 guess for the quiz
    void quizGuess(int guess);

    ///@brief closes the dino fact sheet
    void closeDinoFact();

    ///@brief switches control flow from the museum scene to the search scene
    void switchToSearchScene();

    ///@brief Maps Dinosaur names to Questions about that dinosaur
    QMap<DinosaurName, QVector<MuseumScene::Question>> questionsMap;

    ///@brief holds the fact sheets for each dinosaur
    QMap<DinosaurName, QString> facts;

    ///@brief holds the header for each dinosaur fact sheet
    QMap<DinosaurName, QString> factsHeader;

    ///@brief tracks which dinosaur the user started the museum scene with
    DinosaurName currentDinosaur;

    ///@brief pointer to search scene, used to pass control flow
    SearchScene* searchPtr;

    ///@brief the museum background
    QPixmap background;

    ///@brief silhouettes of the dinosaurs
    QPixmap triceratopsSilhouette;
    QPixmap tRexSilhouette;
    QPixmap brontosaurusSilhouette;

    ///@brief image of the pop up for quiz and fact sheet sequence
    QPixmap quizBackground;

    ///@brief images of the prompts for the user
    QPixmap pressAnyKey;
    QPixmap pressF;
    QPixmap pressNumbers;
    QPixmap youWin;

    ///@brief fonts used during the sequence
    QFont body;
    QFont title;
    QFont credits;
    QFont factFont;

    ///@brief these are the anchor coordinates for each of the dinosaurs
    QMap<DinosaurName, QPoint> dinosaurBaseCoordinates;

    ///@brief the question that will be asked to the user during the quiz sequence
    Question currentQuestion;

    ///@brief Used to track current coordinates of dino bones as they are animated
    int animationX;
    int animationY;
    int animationFrameCount;
    int animationDimension;

    ///@brief used to track the number of frames generated during the credit sequence
    int gameOverFrameCount;

    ///@brief used to track the location of the credits as they scroll
    int creditY;

    ///@brief these are the coordinates used during the animation for where the bones move to
    QMap<DinosaurName, QPoint> dinosaurCoordinates;

    ///@brief tracks if the animation is currently occurring
    bool animationActive;

    ///@brief tracks if the dino fact should be shown
    bool showDinoFact;

    ///@brief tracks if the quiz should be closed
    bool closeQuiz;

    ///@brief tracks if the credits should start
    bool startCredits;

    ///@brief tracks if the game over sound should play
    bool winSoundPlayed;

    ///@brief tracks if the player has submitted an answer for the quiz
    int playerAnswered;

    ///@brief quiz question that it the user is on
    int quizNumber;

protected:
    ///@brief resets the museum state when control flow is handed to the museum
    virtual void activate();

    ///@brief deactivates the museum when control flow is passed to another scene
    virtual void deactivate();

    ///@brief performs actions based on what the user's most recent input is
    virtual void processPlayerInput();
};

#endif // MUSEUMSCENE_H
