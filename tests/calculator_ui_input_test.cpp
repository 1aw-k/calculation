#include "calculator_ui_input_test.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QtTest>

#include "../mainwindow.h"

namespace {

QString displayText(MainWindow *window)
{
    return window->findChild<QLineEdit *>(QStringLiteral("displayLineEdit"))
            ->text();
}

QString expressionText(MainWindow *window)
{
    return window->findChild<QLabel *>(QStringLiteral("expressionLabel"))
            ->text();
}

void clickButton(MainWindow *window, const QString &objectName)
{
    QPushButton *button =
            window->findChild<QPushButton *>(objectName);
    QVERIFY2(button, qPrintable(QStringLiteral("Missing button: ")
                               + objectName));
    QTest::mouseClick(button, Qt::LeftButton);
}

} // namespace

void CalculatorUiInputTest::keyboardAndMouseProduceSameResult()
{
    MainWindow keyboardWindow;
    keyboardWindow.show();
    QCoreApplication::processEvents();

    QTest::keyClicks(&keyboardWindow, QStringLiteral("12"));
    QTest::keyClick(&keyboardWindow, Qt::Key_Plus);
    QTest::keyClick(&keyboardWindow, Qt::Key_8);
    QTest::keyClick(&keyboardWindow, Qt::Key_Equal);

    MainWindow mouseWindow;
    mouseWindow.show();
    QCoreApplication::processEvents();

    clickButton(&mouseWindow, QStringLiteral("oneButton"));
    clickButton(&mouseWindow, QStringLiteral("twoButton"));
    clickButton(&mouseWindow, QStringLiteral("plusButton"));
    clickButton(&mouseWindow, QStringLiteral("eightButton"));
    clickButton(&mouseWindow, QStringLiteral("equalsButton"));

    QCOMPARE(displayText(&keyboardWindow), QStringLiteral("20"));
    QCOMPARE(displayText(&mouseWindow), QStringLiteral("20"));
    QCOMPARE(displayText(&mouseWindow), displayText(&keyboardWindow));
    QCOMPARE(expressionText(&mouseWindow), expressionText(&keyboardWindow));
}

void CalculatorUiInputTest::keyboardAndMouseHandleBackspaceAndClear()
{
    MainWindow keyboardWindow;
    keyboardWindow.show();
    QCoreApplication::processEvents();

    QTest::keyClicks(&keyboardWindow, QStringLiteral("123"));
    QTest::keyClick(&keyboardWindow, Qt::Key_Backspace);
    QTest::keyClick(&keyboardWindow, Qt::Key_Escape);

    MainWindow mouseWindow;
    mouseWindow.show();
    QCoreApplication::processEvents();

    clickButton(&mouseWindow, QStringLiteral("oneButton"));
    clickButton(&mouseWindow, QStringLiteral("twoButton"));
    clickButton(&mouseWindow, QStringLiteral("threeButton"));
    clickButton(&mouseWindow, QStringLiteral("backspaceButton"));
    clickButton(&mouseWindow, QStringLiteral("clearButton"));

    QCOMPARE(displayText(&keyboardWindow), QStringLiteral("0"));
    QCOMPARE(displayText(&mouseWindow), QStringLiteral("0"));
    QCOMPARE(displayText(&mouseWindow), displayText(&keyboardWindow));
}

void CalculatorUiInputTest::memoryShortcutsAndButtonsStayConsistent()
{
    MainWindow keyboardWindow;
    keyboardWindow.show();
    QCoreApplication::processEvents();

    QTest::keyClicks(&keyboardWindow, QStringLiteral("5"));
    QTest::keyClick(&keyboardWindow, Qt::Key_P, Qt::ControlModifier);
    QTest::keyClick(&keyboardWindow, Qt::Key_R, Qt::ControlModifier);

    MainWindow mouseWindow;
    mouseWindow.show();
    QCoreApplication::processEvents();

    clickButton(&mouseWindow, QStringLiteral("fiveButton"));
    clickButton(&mouseWindow, QStringLiteral("memoryAddButton"));
    clickButton(&mouseWindow, QStringLiteral("memoryRecallButton"));

    QCOMPARE(displayText(&keyboardWindow), QStringLiteral("5"));
    QCOMPARE(displayText(&mouseWindow), QStringLiteral("5"));
    QCOMPARE(displayText(&mouseWindow), displayText(&keyboardWindow));
}

QTEST_MAIN(CalculatorUiInputTest)
