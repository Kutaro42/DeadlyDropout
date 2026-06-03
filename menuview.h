#ifndef MENUVIEW_H
#define MENUVIEW_H
#include <QWidget>

class MenuView : public QWidget
{
    Q_OBJECT
public:
    MenuView(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);

signals:
    void sigGameRq();

};

#endif // MENUVIEW_H
