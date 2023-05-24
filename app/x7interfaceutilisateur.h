#ifndef X7INTERFACEUTILISATEUR_H
#define X7INTERFACEUTILISATEUR_H

#include <QObject>

class X7InterfaceUtilisateur : public QObject
{
    Q_OBJECT
public:
    explicit X7InterfaceUtilisateur(QObject *parent = nullptr);

signals:

public slots:
};

#endif // X7INTERFACEUTILISATEUR_H