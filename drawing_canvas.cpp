#include "drawing_canvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>
#include <cmath>

DrawingCanvas::DrawingCanvas(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color: white;");
}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::blue);
    for (const QPoint &point : points)
    {
        painter.drawEllipse(point, 4, 4);
    }

    if (convex_hull_points.size() > 1)
    {
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(Qt::NoBrush);

        QPolygon hull_polygon(convex_hull_points);
        painter.drawPolygon(hull_polygon);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        points.append(event->pos());
        update();
    }
}

void DrawingCanvas::clearCanvas()
{
    points.clear();
    convex_hull_points.clear();
    update();
}

// Graham Scan Convex Hull
int DrawingCanvas::orientation(const QPoint &p, const QPoint &q, const QPoint &r)
{
    int val = (q.y() - p.y()) * (r.x() - q.x()) -
              (q.x() - p.x()) * (r.y() - q.y());

    if (val == 0)
        return 0;             // collinear
    return (val > 0) ? 1 : 2; // clockwise / counterclockwise
}

void DrawingCanvas::convexHull()
{
    if (points.size() < 3)
    {
        return;
    }
    convex_hull_points.clear();
    QVector<QPoint> points2 = points;

    int ymin = points2[0].y(), min_idx = 0;
    for (int i = 1; i < points2.size(); i++)
    {
        int y = points2[i].y();
        if ((y < ymin) || (ymin == y && points2[i].x() < points2[min_idx].x()))
        {
            ymin = y;
            min_idx = i;
        }
    }
    std::swap(points2[0], points2[min_idx]);
    QPoint p0 = points2[0];

    std::sort(points2.begin() + 1, points2.end(), [&](const QPoint &p1, const QPoint &p2)
              {
                  int o = orientation(p0, p1, p2);
                  if (o == 0)
                  { // ambil yg deket sama p0 kalo collinear
                      return (p0.x() - p1.x()) * (p0.x() - p1.x()) + (p0.y() - p1.y()) * (p0.y() - p1.y()) <
                             (p0.x() - p2.x()) * (p0.x() - p2.x()) + (p0.y() - p2.y()) * (p0.y() - p2.y());
                  }
                  return (o == 2); // counterclockwise
              });

    convex_hull_points.push_back(points2[0]);
    convex_hull_points.push_back(points2[1]);
    convex_hull_points.push_back(points2[2]);

    for (int i = 3; i < points2.size(); i++)
    {
        while (convex_hull_points.size() > 1 && orientation(convex_hull_points[convex_hull_points.size() - 2], convex_hull_points.back(), points2[i]) != 2)
        {
            convex_hull_points.pop_back();
        }
        convex_hull_points.push_back(points2[i]);
    }

    update();
}
