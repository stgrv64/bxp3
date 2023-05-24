#ifndef X7CUSTOMEVENT_H
#define X7CUSTOMEVENT_H

#include <QEvent>
#include <QVariant>

// Evenements personnalisés inter class

struct EventTest //definir les types par catégorie
{
    enum event {FIRST=1,
                TEST,
                LAST};
};


class X7CustomEvent: public QEvent {
public:
    X7CustomEvent(int eventType, QVariant data=QVariant()) :
    QEvent(X7CustomEvent::custom_type()), _eventType(eventType),_data(data) {
        setAccepted(false);
    }

    static QEvent::Type custom_type() {
        static int type = QEvent::registerEventType();
        return (QEvent::Type) type;
    }

    int getEventType() {
        return _eventType;
    }

    QVariant getData(){
        return _data;
    }

    X7CustomEvent* makeNew()
    {
        return new X7CustomEvent(_eventType,_data);
    }
private:
    int _eventType; //type a definir dans les structures
    QVariant _data; //data personnalisées

};

#endif // X7CUSTOMEVENT_H
