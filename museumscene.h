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

    /// @brief Loads the quiz questions from the
    /// @param The resource path to the json file holding the questions
    /// @param The list of questions from the file in the given path
    QVector<MuseumScene::Question> loadQuestions(QString resourcePath);

    //QMap<DinosaurBone, QPixmap> tRexFacts;
    // Maps Dinosaur names to Questions about that dinosaur
    QMap<DinosaurName, QVector<MuseumScene::Question>> questionsMap;
    SearchScene* searchPtr;
    QPixmap background;
    int tRexBaseX;
    int tRexBaseY;
    int brontosaurusBaseX;
    int brontosaurusBaseY;
    int triceratopsBaseX;
    int triceratopsBaseY;
};

#endif // MUSEUMSCENE_H
