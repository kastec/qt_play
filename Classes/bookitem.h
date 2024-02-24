#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QObject>

class BookItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:
    explicit BookItem(QObject *parent = 0);

    QString color() const;
    void setColor(QString color);
    QString text() const;
    void setText(QString text);

signals:
    void colorChanged(QString color);
    void textChanged(QString text);

private:
    QString m_color;
    QString m_text;
};


#endif // BOOKITEM_H
