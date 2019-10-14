#ifndef SESSIONSMANAGER_H
#define SESSIONSMANAGER_H
#include <QtCore>

class SessionsManager : public QObject
{
    /* SessionsManager is responsible for saving and loading user sessions. */

    Q_OBJECT

public:
    SessionsManager();
    virtual ~SessionsManager();

    void start();
    void save_atomary();
    void save_session();
    void load_session();

//private slots:
//    void some();

//signals:
//    void dump_start();
//    void dump_end();
};

#endif // SESSIONSMANAGER_H
