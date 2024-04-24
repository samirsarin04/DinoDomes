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
#include "soundeffect.h"
#include <mutex>

/**
 * @brief The Model class
 * Reprsents the model object for the class
 * Reviewed by Samir Sarin
 */
class Model : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Model constructor
     * @param parent
     */
    explicit Model(QObject *parent = nullptr);
    ~Model();

signals:

    /**
     * @brief sendFrameToView
     * @param frame
     */
    void sendFrameToView(QPixmap frame);

    /**
     * @brief sendSoundEffect
     * @param sound
     */
    void sendSoundEffect(SoundEffect sound);

public slots:
    /// @brief handles when a key is pressed by changing scene if demanded, then lets the scene
    ///        determine what else to do, ei move left or right.
    void handleKeyPress(KeyStroke key);
    /// @brief Runs game logic for each frame, called each frame and sends Qimage to be displayed
    ///        by view.
    void newFrameTick();

private:
    /**
     * @brief currentScene
     *  represents the current scene
     */
    Scene* currentScene;

    /**
     * @brief currentFrame
     * represents the current frame
     */
    QImage currentFrame;

    /**
     * @brief timer
     * timer object for frame
     */
    QTimer timer;

    /**
     * @brief player
     * The player state that holds the information about current dinosaur bone etc.
     */
    PlayerState player;

    /**
     * @brief digScene
     * Object for the dig scene
     */
    DigScene digScene;

    /**
     * @brief museumScene
     * Object for the museum scene
     */
    MuseumScene museumScene;

    /**
     * @brief searchScene
     * object for the search scene
     */
    SearchScene searchScene;
};
#endif // MODEL_H
