#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include "keystroke.h"
#include "model.h"
#include "QPainter"
#include <QSoundEffect>
//#include <QMediaPlayer>
#include <QtMultimedia>


QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event) override;

public:
    View(Model &model, QWidget *parent = nullptr);
    ~View();

signals:
    void keyPressed(KeyStroke key);

public slots:
    void updateFrame(QPixmap frame);
    void loopAudio();

private:
    Ui::View *ui;
    QPainter painter();

    QMediaPlayer *music;
    QAudioOutput *output;
};
#endif // VIEW_H
