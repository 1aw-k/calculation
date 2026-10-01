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
    QString m_pendingOperator;
    QString m_lastOperator;

    double m_accumulator;
    double m_lastOperand;
    bool m_startNewOperand;
    bool m_justEvaluated;
    bool m_hasError;
};

#endif // CALCULATORENGINE_H
