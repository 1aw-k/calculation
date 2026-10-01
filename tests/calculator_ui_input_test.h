#ifndef CALCULATORUIINPUTTEST_H
#define CALCULATORUIINPUTTEST_H

#include <QObject>
#include <QString>

class CalculatorUiInputTest : public QObject
{
    Q_OBJECT

private slots:
    void keyboardAndMouseProduceSameResult();
    void keyboardAndMouseHandleBackspaceAndClear();
};

#endif // CALCULATORUIINPUTTEST_H
