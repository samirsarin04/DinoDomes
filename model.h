#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QObject>
#include <QTimer>
#include "digscene.h"
#include "keystroke.h"
#include "museumscene.h"
#include "searchscene.h"
#include "playerstate.h"
#include <mutex>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

signals:
    void sendFrameToView(QImage frame);

public slots:
    void handleKeyPress(KeyStroke key);
    void newFrameTick();

private:
    enum Scene { dig, museum, search };
    Scene currentScene;
    QImage currentFrame;
    QTimer timer;
    std::mutex lock;

    PlayerState player;
    DigScene digScene;
    MuseumScene museumScene;
    SearchScene searchScene;

    void detectSceneChange(Scene before);
};

#endif // MODEL_H
