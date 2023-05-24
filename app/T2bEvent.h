#ifndef T2BEVENT_H
#define T2BEVENT_H

#include <QEvent>
#include <QVariant>

struct EventType {

    enum event {
        BADGE_UPDATE_NOM //update de la table de conversion badge -> nom.
    };

};
// Evénement personnalisé pour time2bii
// le type
class T2bEvent: public QEvent {
public:

    T2bEvent(int eventType, const QVariant& data=QVariant(), const QHash<QString,QString> &dataListe=QHash<QString,QString>()) : QEvent(T2bEvent::custom_type()), _eventType(eventType), _data(data), _dataListe(dataListe)
    {
        setAccepted(false);
    }

    static QEvent::Type custom_type()
    {
        static int type = QEvent::registerEventType();
        return (QEvent::Type) type;
    }

    int eventType() {
        return _eventType;
    }

    QVariant data(){
        return _data;
    }

    QHash<QString,QString> dataListe()
    {
        return _dataListe;
    }

    bool value(const QString &key, QString *value)
    {
        if(_dataListe.contains(key))
        {
            *value = _dataListe.value(key);
            return true;
        }else
            return false;
    }



    T2bEvent* makeNew()
    {
        return new T2bEvent(_eventType,_data,_dataListe);
    }

private:
    int _eventType;
    QVariant _data;
    QHash<QString,QString> _dataListe;
};

#endif // T2BEVENT_H
