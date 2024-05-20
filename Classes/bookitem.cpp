#include "bookitem.h"
#include <QDateTime>

BookItem::BookItem(QObject *parent) :
    QObject(parent)
{
    m_date = QDateTime(QDate(2024,04,30), QTime(23,00, 00));
    m_date.setTimeSpec(Qt::TimeSpec::UTC);
}

QString BookItem::color() const
{
    return m_color;
}

void BookItem::setColor(QString color)
{
    if (m_color == color) {
        return;
    }

    m_color = color;

    emit colorChanged(m_color);
}

QString BookItem::text() const
{
    return m_text;
}

void BookItem::setText(QString text)
{
    if (m_text == text) {
        return;
    }

    m_text = text;

    emit textChanged(m_text);
}


QDateTime BookItem::date() const
{
    return m_date;
}

void BookItem::setDate(QDateTime date)
{
    if (m_date == date) {
        return;
    }
    m_date = date;
    
    emit dateChanged(m_date);
}
