#ifndef BOOKITEM_H
#define BOOKITEM_H

#include "qdatetime.h"
#include <QObject>

class BookItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QDateTime date READ date WRITE setDate NOTIFY dateChanged)
    
public:
    explicit BookItem(QObject *parent = 0);

    QString color() const;
    void setColor(QString color);
    QString text() const;
    void setText(QString text);
    
    QDateTime date() const;
    void setDate(QDateTime date);

signals:
    void colorChanged(QString color);
    void textChanged(QString text);
    void dateChanged(QDateTime text);

private:
    QString m_color;
    QString m_text;
    QDateTime m_date;
};


#endif // BOOKITEM_H
