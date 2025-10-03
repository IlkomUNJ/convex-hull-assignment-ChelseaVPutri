#ifndef DRAWING_CANVAS_H
#define DRAWING_CANVAS_H

#include <QWidget>
#include <QVector>
#include <QPoint>

class DrawingCanvas : public QWidget {
    Q_OBJECT

public:
    explicit DrawingCanvas(QWidget *parent = nullptr);


public slots:
    void convexHull();
    void clearCanvas();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    static int orientation(const QPoint &p, const QPoint &q, const QPoint &r);
    QVector<QPoint> points;
    QVector<QPoint> convex_hull_points;
};

#endif // DRAWING_CANVAS_H
