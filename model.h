#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include "keystroke.h"
#include "digscene.h"
#include "museumscene.h"
#include "searchscene.h"

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
    enum Scene {dig, musuem, search};
    Scene currentScene;
    QImage currentFrame;
    QTimer timer;

    DigScene digScene;
    MuseumScene museumScene;
    SearchScene searchScene;

    void detectSceneChange(Scene before);
};

#endif // MODEL_H
