#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include "model.h"
#include "keystroke.h"

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
    View(Model& model, QWidget *parent = nullptr);
    ~View();

signals:
    void keyPressed(KeyStroke key);

public slots:
    void updateFrame(QImage frame);

private:
    Ui::View *ui;
};
#endif // VIEW_H
