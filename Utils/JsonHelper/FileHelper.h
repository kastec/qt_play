
#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QObject>
#include <QBuffer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QFile>
#include <QDir>

class FileHelper 
{
    
public:
  
 // TODO:   In Qt 5, you should use QSaveFile instead of QFile when saving the file.

  static void save(QString path, QByteArray content) {
      QFile file(path);
      file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);

      file.write(content);
      file.close();
  }

    static QByteArray readAll(QString path)
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QByteArray rawData = file.readAll();
        file.close();
        return rawData;
    }
};

#endif // FILEHELPER_H
