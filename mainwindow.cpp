#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QList>
#include <QPushButton>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    setupInputBindings();
    refreshDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & (Qt::ControlModifier | Qt::AltModifier)) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    QString input;
    switch (event->key()) {
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        input = QString::number(event->key() - Qt::Key_0);
        break;
    case Qt::Key_Period:
    case Qt::Key_Comma:
        input = QStringLiteral(".");
        break;
    case Qt::Key_Plus:
        input = QStringLiteral("+");
        break;
    case Qt::Key_Minus:
        input = QStringLiteral("-");
        break;
    case Qt::Key_Asterisk:
        input = QStringLiteral("*");
        break;
    case Qt::Key_Slash:
        input = QStringLiteral("/");
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Equal:
        input = QStringLiteral("=");
        break;
    case Qt::Key_Backspace:
        input = QStringLiteral("Backspace");
        break;
    case Qt::Key_Escape:
    case Qt::Key_Delete:
        input = QStringLiteral("C");
        break;
    default:
        if (event->text().compare(QStringLiteral("x"), Qt::CaseInsensitive) == 0) {
            input = QStringLiteral("*");
        } else if (event->text().compare(QStringLiteral("c"), Qt::CaseInsensitive) == 0) {
            input = QStringLiteral("C");
        }
        break;
    }

    if (input.isEmpty()) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    event->accept();
    processInput(input);
}

void MainWindow::setupInputBindings()
{
    const QList<QPushButton *> buttons =
            ui->centralwidget->findChildren<QPushButton *>();

    for (QPushButton *button : buttons) {
        const QString input = button->property("inputToken").toString();
        if (input.isEmpty()) {
            continue;
        }

        connect(button, &QPushButton::clicked, this, [this, input]() {
            processInput(input);
        });
    }
}

void MainWindow::processInput(const QString &input)
{
    m_calculator.process(input);
    refreshDisplay();

    // Keep keyboard input on the window even after a mouse click.
    setFocus();
}

void MainWindow::refreshDisplay()
{
    ui->displayLineEdit->setText(m_calculator.displayText());
    ui->expressionLabel->setText(m_calculator.expressionText());

    ui->displayLineEdit->setProperty("error", m_calculator.hasError());
    ui->displayLineEdit->style()->unpolish(ui->displayLineEdit);
    ui->displayLineEdit->style()->polish(ui->displayLineEdit);
    ui->displayLineEdit->update();
}
