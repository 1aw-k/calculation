#ifndef CALCULATORENGINE_H
#define CALCULATORENGINE_H

#include <QString>

class CalculatorEngine
{
public:
    CalculatorEngine();

    QString process(const QString &input);
    QString displayText() const;
    QString expressionText() const;
    bool hasError() const;
    void clear();

    static bool isOperator(const QString &input);

private:
    static const int MaxInputDigits = 15;

    void inputDigit(const QString &digit);
    void inputDecimalPoint();
    void inputOperator(const QString &op);
    void inputEquals();
    void inputBackspace();
    void inputMemory(const QString &input);
    void inputPercent();

    bool applyPendingOperation(double rightOperand);
    bool calculate(const QString &op, double leftOperand,
                   double rightOperand, double *result);
    void setDivisionByZeroError(double leftOperand, double rightOperand);
    void updateTypingExpression();

    double currentValue() const;
    int currentDigitCount() const;

    static QString operatorSymbol(const QString &op);
    static QString formatNumber(double value);

    QString m_displayText;
    QString m_expressionText;
    QString m_pendingOperator; // Current binary operator, empty when none.
    QString m_lastOperator;    // Used by repeated equals.

    double m_accumulator;
    double m_lastOperand;
    double m_memoryValue;
    bool m_startNewOperand; // Next digit starts a fresh operand.
    bool m_justEvaluated;   // Enables repeated equals and result continuation.
    bool m_hasError;        // Blocks invalid operations until clear/new input.
    bool m_memorySet;
};

#endif // CALCULATORENGINE_H
