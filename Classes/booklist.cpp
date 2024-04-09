#include "booklist.h"
#include "bookitem.h"
#include "Utils/datasamples.h"
#include <QAbstractListModel>
#include <QDebug>
#include "DataSources/appMessageBus.h"

BookList::BookList(QObject *parent) : QObject(parent) {
  auto book = getNewBookItem();
  m_data << book;

  auto book2 = getNewBookItem();
  m_data << book2;
  
  connect(AppMessageBus::i(), &AppMessageBus::createBook, this,
          [=](const QString& author) {
              qDebug() << "got SIGNAL CreateBook "<< author;
              this->add(author);
          });
}

QQmlListProperty<BookItem> BookList::data() {
  return QQmlListProperty<BookItem>( // this, this,
      static_cast<QObject *>(this), static_cast<void *>(&m_data),
      &BookList::appendData, &BookList::countData, &BookList::atData,
      &BookList::clearData);
}


BookItem* BookList::getBookItem(int index)
{
  return m_data[index];
}

BookItem * BookList::getNewBookItem()
{
  DataSamples::LoadJsonData();

  auto color = DataSamples::GetColor();
  qDebug("add item: " + color.toLatin1());

  BookItem *book = new BookItem(this);
  book->setColor(color);
  book->setText(color);
  //book->setProperty("color", color);
  //book->setProperty("text", color);
  
  
  
  
  return book;
}

void BookList::add(const QString& name) {

  auto book = getNewBookItem();
  if(!name.isEmpty())
      book->setText(name);
  
  m_data.insert(m_data.count()/2,book);
//  m_data.append(book);
  
  emit AppMessageBus::i()->logEvent(" log bus event: book added "+book->text());
  
  emit dataChanged();

}

void BookList::updateListData() {

  QList<BookItem *> newList = QList<BookItem *>();

  auto book = getNewBookItem();
  newList << book;

  auto book2 = getNewBookItem();
  newList << book2;

  m_data = newList;
  emit dataChanged();
}

void BookList::appendData(QQmlListProperty<BookItem> *list, BookItem *value) {
  QList<BookItem *> *data = static_cast<QList<BookItem *> *>(list->data);
  data->append(value);
}

qsizetype BookList::countData(QQmlListProperty<BookItem> *list) {
  QList<BookItem *> *data = static_cast<QList<BookItem *> *>(list->data);
  return data->size();
}

BookItem *BookList::atData(QQmlListProperty<BookItem> *list, qsizetype index) {
  QList<BookItem *> *data = static_cast<QList<BookItem *> *>(list->data);
  return data->at(index);
}

void BookList::clearData(QQmlListProperty<BookItem> *list) {
  QList<BookItem *> *data = static_cast<QList<BookItem *> *>(list->data);
  qDeleteAll(data->begin(), data->end());
  data->clear();
}
