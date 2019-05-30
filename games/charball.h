#ifndef CHARBALL_H
#define CHARBALL_H

#include <QColor>
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QTimerEvent>
#include <QWidget>

class CharBall : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    CharBall(int size, int position, int speed, QChar character);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget);
    void destroy();
    QChar character();
    double wind;
    double rad;

private:
    void destroying();

    int currentSpeed;
    int currentX;
    int currentY;
    int currentRadius;
    int destroyed;
    QColor color;
    QChar currentCharacter;
};

#endif // CHARBALL_H
