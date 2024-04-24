#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QResizeEvent>
#include "ui_view.h"
#include <QMainWindow>
#include "keystroke.h"
#include "model.h"
#include "soundeffect.h"
#include <QSoundEffect>
#include <QtMultimedia>


QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE
///@brief view class declaration that is used for the dinodomes game
///Reviewed by Peyton Jensen
class View : public QMainWindow
{
    Q_OBJECT

protected:
    ///@brief handles a keypress that has occurred
    ///@param event the QKeyEvent that occurred
    void keyPressEvent(QKeyEvent *event) override;

    ///@brief handles resizes when needed
    ///@param event the QResizeEvent that has occurred
    void resizeEvent(QResizeEvent *event) override;

public:
    ///@brief constructs a view object
    ///@param model the model for the game
    View(Model &model, QWidget *parent = nullptr);

    ///@brief deconstructor
    ~View();

signals:
    ///@brief signals that a key has been pressed
    ///@param key the key pressed
    void keyPressed(KeyStroke key);

public slots:
    ///@brief updates the view on each frame
    ///@param frame the frame to be set to
    void updateFrame(QPixmap frame);

    ///@brief loops the audio played
    void loopAudio();

    ///@brief plays the sound effect passed
    ///@param sound the sound to be played
    void playSoundEffect(SoundEffect sound);

private:
    ///@brief pointer to the Ui
    Ui::View *ui;

    ///@brief the size of the view
    QSize gameSize;

    ///@brief the media player to be sent to output
    QMediaPlayer *music;

    ///@brief the player that actually outputs media
    QAudioOutput *output;

    ///sound effects used in game, explained by name
    QSoundEffect* dig;
    QSoundEffect* walk;
    QSoundEffect* findBone;
    QSoundEffect* door;
    QSoundEffect* shovel;
    QSoundEffect* brush;
    QSoundEffect* pageTurn;
    QSoundEffect* correct;
    QSoundEffect* wrong;
    QSoundEffect* victory;

    ///@brief map holding the sound effects
    QMap<SoundEffect, QSoundEffect*> soundEffects;
};
#endif // VIEW_H
