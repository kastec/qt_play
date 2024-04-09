
#ifndef QQMLHELPER_H
#define QQMLHELPER_H

#include <QObject>

#define QML_WRITABLE_PROPERTY(type, name) \
protected: \
    Q_PROPERTY (type name READ get_##name WRITE set_##name NOTIFY name##Changed) \
    public: \
    type name; \
    type get_##name () const { \
        return name ; \
} \
    public: \
    bool set_##name (type value) { \
        bool ret = false; \
        if ((ret = name != value)) { \
            name = value; \
            emit name##Changed (name); \
    } \
        return ret; \
} \
    Q_SIGNALS: \
    void name##Changed (type name); \
    private:

#define QML_READ_PROPERTY(type, name) \
              protected: \
    Q_PROPERTY (type name MEMBER name NOTIFY name##Changed) \
    public: \
    type name; \
    Q_SIGNALS: \
    void name##Changed (type name); \
    private:

#define QML_CONSTANT_PROPERTY(type, name) \
              protected: \
    Q_PROPERTY (type name MEMBER name CONSTANT) \
    public: \
    type name; \
    private:

/*
#define QML_READ_PROPERTY(type, name) \
protected: \
    Q_PROPERTY (type name READ get_##name CONSTANT) \
    private: \
    type m_##name; \
    public: \
    type get_##name () const { \
        return m_##name ; \
} \
    private:
*/

/*
#define QML_LIST_PROPERTY(type, name) \
    Q_PROPERTY (QList<QObject*> name READ get_##name NOTIFY name##Changed) \
    public: \
    type name; \
    QList<QObject *> get_##name () const { \
        return *reinterpret_cast<const QList<QObject *>*>(& name); \
        } \
    bool set_##name (type value) { \
        bool ret = false; \
        if ((ret = value != name)) { \
            name = value; \
            emit name##Changed (name); \
         } \
        return ret; \
    } \
    Q_SIGNALS: \
    void name##Changed (type name); \
    private:
*/


#define QML_LIST_PROPERTY(type, name) \
              Q_PROPERTY (type name READ get_##name WRITE set_##name) \
              Q_PROPERTY (QList<QObject*> name##Display READ get_##name##Display NOTIFY name##DisplayChanged) \
              public: \
    type name; \
    type get_##name () const { \
        return name ; \
} \
    bool set_##name (type value) { \
        bool ret = false; \
        if ((ret = value != name)) { \
            name = value; \
            emit name##Changed (name); \
            emit name##DisplayChanged (name); \
    } \
        return ret; \
} \
    QList<QObject *> get_##name##Display () const { \
        QT_WARNING_PUSH \
        QT_WARNING_DISABLE_GCC("-Wstrict-aliasing")  \
        return *reinterpret_cast<const QList<QObject *>*>(&name); \
        QT_WARNING_POP  \
} \
    Q_SIGNALS: \
    void name##Changed (type name); \
    void name##DisplayChanged (type name); \
    private:


#define QML_WRITABLE_PROPERTY_CB(type, name, onChaging) \
protected: \
    Q_PROPERTY (type name READ get_##name WRITE set_##name NOTIFY name##Changed) \
    public: \
    type name; \
    type get_##name () const { return name;}  \
\
    public: \
    bool set_##name (type value) { \
        if (name == value) return false;\
        bool res = onChaging(value); \
        if (res == false) return false;  \
        name = value; \
        emit name##Changed (name); \
        return true; \
} \
    Q_SIGNALS: \
    void name##Changed (type name); \
    private:



/*
#define QML_READONLY_PROPERTY(type, name) \
              protected: \
    Q_PROPERTY (type name READ get_##name NOTIFY name##Changed) \
    private: \
    type m_##name; \
    public: \
    type get_##name () const { \
        return m_##name ; \
} \
    bool update_##name (type name) { \
        bool ret = false; \
        if ((ret = m_##name != name)) { \
            m_##name = name; \
            emit name##Changed (m_##name); \
    } \
        return ret; \
} \
    Q_SIGNALS: \
    void name##Changed (type name); \
    private:

#define QML_CONSTANT_PROPERTY(type, name) \
           protected: \
 Q_PROPERTY (type name READ get_##name CONSTANT) \
 private: \
 type m_##name; \
 public: \
 type get_##name () const { \
     return m_##name ; \
} \
 private:

#define QML_LIST_PROPERTY(CLASS, NAME, TYPE) \
           public: \
 static int NAME##_count (QQmlListProperty<TYPE> * prop) { \
     CLASS * instance = qobject_cast<CLASS *> (prop->object); \
     return (instance != NULL ? instance->m_##NAME.count () : 0); \
} \
 static void NAME##_clear (QQmlListProperty<TYPE> * prop) { \
     CLASS * instance = qobject_cast<CLASS *> (prop->object); \
     if (instance != NULL) { \
         instance->m_##NAME.clear (); \
 } \
} \
 static void NAME##_append (QQmlListProperty<TYPE> * prop, TYPE * obj) { \
     CLASS * instance = qobject_cast<CLASS *> (prop->object); \
     if (instance != NULL && obj != NULL) { \
         instance->m_##NAME.append (obj); \
 } \
} \
 static TYPE * NAME##_at (QQmlListProperty<TYPE> * prop, int idx) { \
     CLASS * instance = qobject_cast<CLASS *> (prop->object); \
     return (instance != NULL ? instance->m_##NAME.at (idx) : NULL); \
} \
 QList<TYPE *> get_##NAME##s (void) const { \
     return m_##NAME; \
} \
 private: \
 QList<TYPE *> m_##NAME;

#define QML_ENUM_CLASS(name, ...) \
class name : public QObject { \
     Q_GADGET \
     public: \
     enum Type { __VA_ARGS__ }; \
     Q_ENUMS (Type) \
};

class QmlProperty : public QObject { Q_OBJECT }; // NOTE : to avoid "no suitable class found" MOC note
*/


/*!  SINGLETON DESIGN PATTERN (Thread Safe) */
#define SINGLETON_PATTERN_DECLARE(classname)\
public: \
    static classname* i() {  if(m_instance==NULL) m_instance=new classname(); return m_instance; } \
    private: \
    static classname* m_instance; 

#define SINGLETON_PATTERN_IMPLIMENT(classname)\
classname* classname::m_instance = NULL; 


#endif // QQMLHELPER_H
