#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QAbstractSpinBox>
#include <QtWidgets/QLineEdit>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>

using boost::multiprecision::int1024_t;
using boost::conversion::try_lexical_convert;

//todo: make it multiline
class BigSpinPrivate;
class BigSpin : public QAbstractSpinBox
{
    Q_OBJECT
public:
    BigSpin(QWidget *parent = 0) : QAbstractSpinBox()
    {
        connect(this->lineEdit(), &QLineEdit::textEdited, this, &BigSpin::onEditFinished);
    };
    ~BigSpin() {};

    int1024_t value() const
    {
        return longValue;
    };

    int1024_t minimum() const
    {
        return minValue;
    };

    void setMinimum(int1024_t min)
    {
        minValue = min;
        if (longValue < minValue)
        {
            longValue = minValue;
            this->lineEdit()->setText(textFromValue(longValue));
        }
    }

    int1024_t maximum() const
    {
        return maxValue;
    };

    void setMaximum(int1024_t max)
    {
        maxValue = max;
        if (longValue > maxValue)
        {
            longValue = maxValue;
            this->lineEdit()->setText(textFromValue(longValue));
        }
    }

    void setRange(int1024_t min, int1024_t max)
    {
        setMinimum(min);
        setMaximum(max);
    }

    virtual void stepBy(int steps)
    {
        auto newValue = longValue;
        if (steps < 0 && ((newValue + steps) > newValue))
            newValue = std::numeric_limits<int1024_t>::min();
        else if (steps > 0 && ((newValue + steps) < newValue))
            newValue = std::numeric_limits<int1024_t>::max();
        else
            newValue += steps;
        this->lineEdit()->setText(textFromValue(newValue));

        setValue(newValue);
        this->lineEdit()->setText(textFromValue(longValue));
    }

protected:
    virtual QValidator::State validate(QString &input, int &pos) const
    {
        int1024_t value;
        if (!try_lexical_convert(input.toStdString(), value))
                return QValidator::Invalid;

        QString minText = textFromValue(minValue),
                maxText = textFromValue(maxValue);

        if (input.size() > minText.size() || input.size() > maxText.size())
            return QValidator::Invalid;

        if ((input.size() == minText.size() && input < minText) || (input.size() == maxText.size() && input > maxText))
            return QValidator::Invalid;

        return QValidator::Acceptable;
    }

    virtual int1024_t valueFromText(const QString &text) const
    {
        int1024_t value = 0;
        try_lexical_convert(text.toStdString(), value);
        return value;
    }

    virtual QString textFromValue(int1024_t val) const
    {
        std::string text;
        try_lexical_convert(val, text);
        return QString(text.c_str());
    }

    virtual QAbstractSpinBox::StepEnabled stepEnabled() const
    {
        return StepUpEnabled | StepDownEnabled;
    }

public Q_SLOTS:
    void setValue(int1024_t value)
    {
        longValue = value;
        if (longValue < minValue)
            longValue = minValue;
        else if (longValue > maxValue)
            longValue = maxValue;
        valueChanged(longValue);

        this->lineEdit()->setText(textFromValue(longValue));
    }

    void onEditFinished(QString input)
    {
        int pos = 0;
        if (QValidator::Acceptable == validate(input, pos))
            setValue(valueFromText(input));
    }
Q_SIGNALS:
    void valueChanged(int1024_t value);

private:
    int1024_t minValue = std::numeric_limits<int1024_t>::min();
    int1024_t maxValue = std::numeric_limits<int1024_t>::max();
    int1024_t longValue = 0;

    Q_PROPERTY(int1024_t minimum  READ minimum    WRITE setMinimum)
    Q_PROPERTY(int1024_t maximum  READ maximum    WRITE setMaximum)
    Q_PROPERTY(int1024_t value    READ value      WRITE setValue NOTIFY valueChanged USER true)
    Q_DECLARE_PRIVATE(BigSpin)
};
