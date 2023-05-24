/**
 * @file
 * @brief MainWindow class and TouchesfonctionSqlModel class declaration
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQuickView> /** Qt5 QQuickView replaces Qt4 QtDeclarative, QWServer obsolete*/
#include <QtSql>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "customnetworkmanagerfactory.h"

/**
* @brief TouchesfonctionSqlModel class
*******************************************************************/
class TouchesfonctionSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit TouchesfonctionSqlModel(QObject *parent,QSqlDatabase config_db);
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());
    void setQuery(const QSqlQuery &query);
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const {	return m_roleNames;	}
public slots:
signals:
private:
    void generateRoleNames();
    QHash<int, QByteArray> m_roleNames;
};

/**
* @brief MainWindow class
*******************************************************************/
/*  SLOT:
*    void window_set_touchesfonction(void);
*    void window_set_proxy_web(QString);
*    void window_init_orientation_ecran(void);
*    void window_orientation_ecran(void);
*    void audio_play(QString);
*    void restart_app(void);
*   SIGNAL:
*    void signal_ihm_orientation_ecran(QVariant);
*    void signal_reset_touche();
*******************************************************************/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent);
   ~MainWindow();
   CustomNetworkManagerFactory* myNetworkAccessManagerFactory;
   void Debug(QString a);

   QQuickView *view = new QQuickView();
   QWidget *container = QWidget::createWindowContainer(view);
   QQmlApplicationEngine engine;
   static int const EXIT_CODE_RESTART= -123456789;

public slots:
   void window_set_touchesfonction(void);
   void window_set_proxy_web(QString);
   void window_init_orientation_ecran(void);
   void window_orientation_ecran(void);
   void audio_play(QString);
   void restart_app(void);  //NOT USED

signals:
    void signal_ihm_orientation_ecran(QVariant);
    void signal_reset_touche();

protected:
   virtual void  keyPressEvent(QKeyEvent *event); //NOT USED

private:
private slots:
protected slots:


};


#endif // MAINWINDOW_H



