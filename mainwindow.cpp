#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Convex Hull Visualization");
    connect(ui->convex_hull_button, &QPushButton::clicked, ui->drawing_canvas, &DrawingCanvas::convexHull);
    connect(ui->clear_all_button, &QPushButton::clicked, ui->drawing_canvas, &DrawingCanvas::clearCanvas);
}

MainWindow::~MainWindow()
{
    delete ui;
}
