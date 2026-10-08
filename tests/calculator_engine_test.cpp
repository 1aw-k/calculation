#include "calculator_engine_test.h"

#include <QtTest>

void CalculatorEngineTest::init()
{
    m_engine.clear();
}

QString CalculatorEngineTest::runInputs(const QStringList &inputs)
{
    for (const QString &input : inputs) {
        m_engine.process(input);
    }

    return m_engine.displayText();
}

void CalculatorEngineTest::basicArithmetic()
{
    QStringList inputs;
    inputs << "1" << "2" << "+" << "8" << "=";

    QCOMPARE(runInputs(inputs), QStringLiteral("20"));
    QCOMPARE(m_engine.expressionText(), QStringLiteral("12 + 8 ="));

    m_engine.clear();
    inputs.clear();
    inputs << "9" << "-" << "4" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("5"));

    m_engine.clear();
    inputs.clear();
    inputs << "6" << "*" << "7" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("42"));

    m_engine.clear();
    inputs.clear();
    inputs << "8" << "/" << "2" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("4"));
}

void CalculatorEngineTest::continuousOperators()
{
    QStringList inputs;
    inputs << "8" << "+" << "*" << "2" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("16"));
    QCOMPARE(m_engine.expressionText(), QStringLiteral("8 × 2 ="));

    m_engine.clear();
    inputs.clear();
    inputs << "2" << "+" << "3" << "+" << "4" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("9"));
    QCOMPARE(m_engine.expressionText(), QStringLiteral("5 + 4 ="));

    m_engine.clear();
    inputs.clear();
    inputs << "1" << "+" << "-" << "3" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("-2"));
}

void CalculatorEngineTest::decimalRules()
{
    QStringList inputs;
    inputs << "1" << "." << "." << "5" << "+" << "." << "2" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("1.7"));
    QCOMPARE(m_engine.expressionText(), QStringLiteral("1.5 + 0.2 ="));

    m_engine.clear();
    inputs.clear();
    inputs << "." << "." << "5";
    QCOMPARE(runInputs(inputs), QStringLiteral("0.5"));

    m_engine.clear();
    inputs.clear();
    inputs << "0" << "." << "1" << "+" << "0" << "." << "2" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("0.3"));
}

void CalculatorEngineTest::backspaceAndClear()
{
    QStringList inputs;
    inputs << "1" << "2" << "3" << "Backspace";
    QCOMPARE(runInputs(inputs), QStringLiteral("12"));

    m_engine.process(QStringLiteral("Backspace"));
    QCOMPARE(m_engine.displayText(), QStringLiteral("1"));

    m_engine.clear();
    inputs.clear();
    inputs << "9" << "8" << "C";
    QCOMPARE(runInputs(inputs), QStringLiteral("0"));
    QCOMPARE(m_engine.expressionText(), QString());

    inputs.clear();
    inputs << "2" << "+" << "3" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("5"));
}

void CalculatorEngineTest::divisionByZero()
{
    QStringList inputs;
    inputs << "5" << "/" << "0" << "=";

    QCOMPARE(runInputs(inputs), QStringLiteral("Error"));
    QVERIFY(m_engine.hasError());
    QCOMPARE(m_engine.expressionText(),
             QStringLiteral("5 ÷ 0 = 除数不能为 0"));

    m_engine.process(QStringLiteral("C"));
    QCOMPARE(m_engine.displayText(), QStringLiteral("0"));
    QVERIFY(!m_engine.hasError());
}

void CalculatorEngineTest::continueAfterResult()
{
    QStringList inputs;
    inputs << "2" << "+" << "3" << "=" << "4";
    QCOMPARE(runInputs(inputs), QStringLiteral("4"));

    m_engine.clear();
    inputs.clear();
    inputs << "2" << "+" << "3" << "=" << "*" << "2" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("10"));
    QCOMPARE(m_engine.expressionText(), QStringLiteral("5 × 2 ="));
}

void CalculatorEngineTest::repeatedEquals()
{
    QStringList inputs;
    inputs << "2" << "+" << "3" << "=" << "=";

    QCOMPARE(runInputs(inputs), QStringLiteral("8"));
    QCOMPARE(m_engine.expressionText(), QStringLiteral("5 + 3 ="));
}

void CalculatorEngineTest::leadingNegativeNumber()
{
    QStringList inputs;
    inputs << "-" << "2" << "*" << "3" << "=";

    QCOMPARE(runInputs(inputs), QStringLiteral("-6"));
    QCOMPARE(m_engine.expressionText(), QStringLiteral("-2 × 3 ="));
}

void CalculatorEngineTest::memoryOperations()
{
    m_engine.process(QStringLiteral("MC"));
    m_engine.clear();

    QStringList inputs;
    inputs << "7" << "M+" << "MR";
    QCOMPARE(runInputs(inputs), QStringLiteral("7"));

    m_engine.process(QStringLiteral("MC"));
    m_engine.clear();
    inputs.clear();
    inputs << "3" << "M-" << "MR";
    QCOMPARE(runInputs(inputs), QStringLiteral("-3"));

    m_engine.process(QStringLiteral("MC"));
    m_engine.process(QStringLiteral("MR"));
    QCOMPARE(m_engine.displayText(), QStringLiteral("0"));
}

void CalculatorEngineTest::clearPreservesMemory()
{
    m_engine.process(QStringLiteral("MC"));
    m_engine.clear();

    QStringList inputs;
    inputs << "8" << "M+" << "C" << "MR";
    QCOMPARE(runInputs(inputs), QStringLiteral("8"));
}

void CalculatorEngineTest::percentageOperations()
{
    QStringList inputs;
    inputs << "5" << "0" << "%";
    QCOMPARE(runInputs(inputs), QStringLiteral("0.5"));

    m_engine.clear();
    inputs.clear();
    inputs << "2" << "0" << "0" << "+" << "1" << "0" << "%" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("220"));

    m_engine.clear();
    inputs.clear();
    inputs << "1" << "0" << "0" << "/" << "5" << "0" << "%" << "=";
    QCOMPARE(runInputs(inputs), QStringLiteral("200"));
}

QTEST_APPLESS_MAIN(CalculatorEngineTest)
