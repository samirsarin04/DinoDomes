#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QPixmap>
#include <QObject>
#include <QTimer>
#include "digscene.h"
#include "keystroke.h"
#include "museumscene.h"
#include "searchscene.h"
#include "playerstate.h"
#include "scene.h"
#include <mutex>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    ~Model();

signals:
    void sendFrameToView(QPixmap frame);

public slots:
    /// @brief handles when a key is pressed by changing scene if demanded, then lets the scene
    ///        determine what else to do, ei move left or right.
    void handleKeyPress(KeyStroke key);
    /// @brief Runs game logic for each frame, called each frame and sends Qimage to be displayed
    ///        by view.
    void newFrameTick();

private:
    //enum SceneEnum { dig, museum, search };
    Scene* currentScene;
    QImage currentFrame;
    QTimer timer;
    //std::mutex lock;

    PlayerState player;
    DigScene digScene;
    MuseumScene museumScene;
    SearchScene searchScene;

    // NOT NECESSARY NOW?
    //void detectSceneChange(Scene before);
};

#endif // MODEL_H
