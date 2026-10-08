#ifndef CALCULATORENGINETEST_H
#define CALCULATORENGINETEST_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "../calculator_engine.h"

class CalculatorEngineTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void basicArithmetic();
    void continuousOperators();
    void decimalRules();
    void backspaceAndClear();
    void divisionByZero();
    void continueAfterResult();
    void repeatedEquals();
    void leadingNegativeNumber();
    void memoryOperations();
    void percentageOperations();

private:
    QString runInputs(const QStringList &inputs);

    CalculatorEngine m_engine;
};

#endif // CALCULATORENGINETEST_H
