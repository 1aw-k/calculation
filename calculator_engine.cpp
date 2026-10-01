#include "calculator_engine.h"

#include <QtGlobal>

CalculatorEngine::CalculatorEngine()
{
    clear();
}

QString CalculatorEngine::process(const QString &input)
{
    if (input.isEmpty()) {
        return m_displayText;
    }

    if (input == QStringLiteral("C")) {
        clear();
        return m_displayText;
    }

    if (m_hasError) {
        if (input == QStringLiteral("Backspace")) {
            clear();
            return m_displayText;
        }

        if (input.size() == 1 && input.at(0).isDigit()) {
            clear();
        } else if (input == QStringLiteral(".")) {
            clear();
        } else {
            return m_displayText;
        }
    }

    if (input.size() == 1 && input.at(0).isDigit()) {
        inputDigit(input);
    } else if (input == QStringLiteral(".")) {
        inputDecimalPoint();
    } else if (isOperator(input)) {
        inputOperator(input);
    } else if (input == QStringLiteral("=")) {
        inputEquals();
    } else if (input == QStringLiteral("Backspace")) {
        inputBackspace();
    }

    return m_displayText;
}

QString CalculatorEngine::displayText() const
{
    return m_displayText;
}

QString CalculatorEngine::expressionText() const
{
    return m_expressionText;
}

bool CalculatorEngine::hasError() const
{
    return m_hasError;
}

void CalculatorEngine::clear()
{
    m_displayText = QStringLiteral("0");
    m_expressionText.clear();
    m_pendingOperator.clear();
    m_lastOperator.clear();

    m_accumulator = 0.0;
    m_lastOperand = 0.0;
    m_startNewOperand = true;
    m_justEvaluated = false;
    m_hasError = false;
}

bool CalculatorEngine::isOperator(const QString &input)
{
    return input == QStringLiteral("+")
            || input == QStringLiteral("-")
            || input == QStringLiteral("*")
            || input == QStringLiteral("/");
}

void CalculatorEngine::inputDigit(const QString &digit)
{
    if (m_justEvaluated || m_startNewOperand) {
        if (m_justEvaluated) {
            m_lastOperator.clear();
            m_lastOperand = 0.0;
        }

        m_displayText = digit;
        m_startNewOperand = false;
        m_justEvaluated = false;
        updateTypingExpression();
        return;
    }

    if (m_displayText == QStringLiteral("0")) {
        m_displayText = digit;
    } else if (m_displayText == QStringLiteral("-0")) {
        m_displayText = QStringLiteral("-") + digit;
    } else if (currentDigitCount() < MaxInputDigits) {
        m_displayText.append(digit);
    }

    updateTypingExpression();
}

void CalculatorEngine::inputDecimalPoint()
{
    if (m_justEvaluated || m_startNewOperand) {
        if (m_justEvaluated) {
            m_lastOperator.clear();
            m_lastOperand = 0.0;
        }

        m_displayText = QStringLiteral("0.");
        m_startNewOperand = false;
        m_justEvaluated = false;
        updateTypingExpression();
        return;
    }

    if (!m_displayText.contains(QLatin1Char('.'))) {
        if (m_displayText == QStringLiteral("-")) {
            m_displayText = QStringLiteral("-0.");
        } else {
            m_displayText.append(QLatin1Char('.'));
        }
    }

    updateTypingExpression();
}

void CalculatorEngine::inputOperator(const QString &op)
{
    // A leading minus is treated as the sign of the first operand.
    if (op == QStringLiteral("-")
            && m_pendingOperator.isEmpty()
            && m_startNewOperand
            && m_displayText == QStringLiteral("0")) {
        m_displayText = QStringLiteral("-");
        m_startNewOperand = false;
        m_justEvaluated = false;
        m_expressionText = m_displayText;
        return;
    }

    // Consecutive operators replace the previous pending operator.
    if (m_startNewOperand && !m_pendingOperator.isEmpty()) {
        m_pendingOperator = op;
        m_expressionText = formatNumber(m_accumulator)
                + QLatin1Char(' ')
                + operatorSymbol(m_pendingOperator);
        return;
    }

    if (!m_pendingOperator.isEmpty()) {
        if (!applyPendingOperation(currentValue())) {
            return;
        }
    } else {
        m_accumulator = currentValue();
    }

    if (m_justEvaluated) {
        m_lastOperator.clear();
        m_lastOperand = 0.0;
        m_justEvaluated = false;
    }

    m_pendingOperator = op;
    m_startNewOperand = true;
    m_expressionText = formatNumber(m_accumulator)
            + QLatin1Char(' ')
            + operatorSymbol(m_pendingOperator);
}

void CalculatorEngine::inputEquals()
{
    if (m_pendingOperator.isEmpty()) {
        if (!m_justEvaluated || m_lastOperator.isEmpty()) {
            return;
        }

        const double leftOperand = currentValue();
        double result = 0.0;
        const QString expression = formatNumber(leftOperand)
                + QLatin1Char(' ')
                + operatorSymbol(m_lastOperator)
                + QLatin1Char(' ')
                + formatNumber(m_lastOperand)
                + QStringLiteral(" =");

        if (!calculate(m_lastOperator, leftOperand, m_lastOperand, &result)) {
            if (!m_hasError) {
                m_expressionText = expression;
            }
            return;
        }

        m_expressionText = expression;
        m_accumulator = result;
        m_displayText = formatNumber(result);
        m_startNewOperand = true;
        m_justEvaluated = true;
        return;
    }

    const QString op = m_pendingOperator;
    const double leftOperand = m_accumulator;
    const double rightOperand = currentValue();
    const QString expression = formatNumber(leftOperand)
            + QLatin1Char(' ')
            + operatorSymbol(op)
            + QLatin1Char(' ')
            + formatNumber(rightOperand)
            + QStringLiteral(" =");

    double result = 0.0;
    if (!calculate(op, leftOperand, rightOperand, &result)) {
        if (!m_hasError) {
            m_expressionText = expression;
        }
        return;
    }

    m_expressionText = expression;
    m_displayText = formatNumber(result);
    m_accumulator = result;
    m_pendingOperator.clear();
    m_lastOperator = op;
    m_lastOperand = rightOperand;
    m_startNewOperand = true;
    m_justEvaluated = true;
}

void CalculatorEngine::inputBackspace()
{
    if (m_hasError || m_justEvaluated) {
        clear();
        return;
    }

    if (m_startNewOperand) {
        return;
    }

    m_displayText.chop(1);
    if (m_displayText.isEmpty() || m_displayText == QStringLiteral("-")) {
        m_displayText = QStringLiteral("0");
    }

    updateTypingExpression();
}

bool CalculatorEngine::applyPendingOperation(double rightOperand)
{
    double result = 0.0;
    if (!calculate(m_pendingOperator, m_accumulator, rightOperand, &result)) {
        return false;
    }

    m_accumulator = result;
    m_displayText = formatNumber(result);
    return true;
}

bool CalculatorEngine::calculate(const QString &op, double leftOperand,
                                 double rightOperand, double *result)
{
    if (op == QStringLiteral("/") && qFuzzyIsNull(rightOperand)) {
        setDivisionByZeroError(leftOperand, rightOperand);
        return false;
    }

    if (op == QStringLiteral("+")) {
        *result = leftOperand + rightOperand;
    } else if (op == QStringLiteral("-")) {
        *result = leftOperand - rightOperand;
    } else if (op == QStringLiteral("*")) {
        *result = leftOperand * rightOperand;
    } else if (op == QStringLiteral("/")) {
        *result = leftOperand / rightOperand;
    } else {
        return false;
    }

    if (!qIsFinite(*result)) {
        m_displayText = QStringLiteral("Error");
        m_expressionText = QStringLiteral("结果超出范围");
        m_pendingOperator.clear();
        m_lastOperator.clear();
        m_startNewOperand = true;
        m_justEvaluated = false;
        m_hasError = true;
        return false;
    }

    return true;
}

void CalculatorEngine::setDivisionByZeroError(double leftOperand,
                                              double rightOperand)
{
    m_displayText = QStringLiteral("Error");
    m_expressionText = formatNumber(leftOperand)
            + QLatin1Char(' ')
            + operatorSymbol(QStringLiteral("/"))
            + QLatin1Char(' ')
            + formatNumber(rightOperand)
            + QStringLiteral(" = 除数不能为 0");

    m_pendingOperator.clear();
    m_lastOperator.clear();
    m_startNewOperand = true;
    m_justEvaluated = false;
    m_hasError = true;
}

void CalculatorEngine::updateTypingExpression()
{
    if (m_pendingOperator.isEmpty()) {
        m_expressionText = m_displayText;
        return;
    }

    m_expressionText = formatNumber(m_accumulator)
            + QLatin1Char(' ')
            + operatorSymbol(m_pendingOperator)
            + QLatin1Char(' ')
            + m_displayText;
}

double CalculatorEngine::currentValue() const
{
    bool ok = false;
    const double value = m_displayText.toDouble(&ok);
    return ok ? value : 0.0;
}

int CalculatorEngine::currentDigitCount() const
{
    int digitCount = 0;
    for (int index = 0; index < m_displayText.size(); ++index) {
        if (m_displayText.at(index).isDigit()) {
            ++digitCount;
        }
    }

    return digitCount;
}

QString CalculatorEngine::operatorSymbol(const QString &op)
{
    if (op == QStringLiteral("*")) {
        return QStringLiteral("×");
    }

    if (op == QStringLiteral("/")) {
        return QStringLiteral("÷");
    }

    return op;
}

QString CalculatorEngine::formatNumber(double value)
{
    if (!qIsFinite(value)) {
        return QStringLiteral("Error");
    }

    if (qFuzzyIsNull(value)) {
        value = 0.0;
    }

    return QString::number(value, 'g', 15);
}
