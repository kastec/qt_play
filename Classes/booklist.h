#ifndef BOOKLIST_H
#define BOOKLIST_H

#include <QtQml>
#include <QObject>
#include <QQmlListProperty>
#include <QAbstractListModel>

class BookItem;

class BookList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<BookItem> data READ data NOTIFY dataChanged)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    explicit BookList(QObject *parent = nullptr);

    Q_INVOKABLE BookItem* getBookItem( int index );

    QQmlListProperty<BookItem> data();
    Q_INVOKABLE void add(const QString& name="");
    Q_INVOKABLE void updateListData();


signals:
     void dataChanged();

private:
     BookItem * getNewBookItem();
    static void appendData(QQmlListProperty<BookItem> *list, BookItem *value);
    static qsizetype countData(QQmlListProperty<BookItem> *list);
    static BookItem *atData(QQmlListProperty<BookItem> *list, qsizetype index);
    static void clearData(QQmlListProperty<BookItem> *list);

    QList<BookItem*> m_data;

};

#endif // BOOKLIST_H
