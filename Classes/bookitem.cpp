#include "bookitem.h"


BookItem::BookItem(QObject *parent) :
    QObject(parent)
{
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
