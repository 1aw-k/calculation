#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "calculator_engine.h"

class QKeyEvent;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupInputBindings();
    void processInput(const QString &input);
    void refreshDisplay();

    Ui::MainWindow *ui;
    CalculatorEngine m_calculator;
};
#endif // MAINWINDOW_H
