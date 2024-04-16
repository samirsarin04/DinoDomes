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
        QString* options;
        // index of correct answer in option
        int correctIndex;
    };

    void loadQuestions();
    void openGuess(QString question, QString answer, QString* incOpts);

    QMap<DinosaurBone, QPixmap> tRexFacts;
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
