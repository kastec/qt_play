#ifndef PERSON_H
#define PERSON_H

#include <QObject>

class Person : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER name )
    Q_PROPERTY(int age MEMBER age )
    Q_PROPERTY(QString seat MEMBER seat )
    Q_PROPERTY(bool enabled READ getEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit Person(QObject *parent = nullptr);
public:
    QString name;
    int age;
    QString seat;
    bool enabled;

  public:
    bool getEnabled() const;
    void setEnabled(bool isEnabled);

signals:
    void enabledChanged(bool isEnabled);
};


Q_DECLARE_METATYPE(Person)

#endif // PERSON_H
