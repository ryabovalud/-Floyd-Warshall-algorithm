#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // WIDGET_H
