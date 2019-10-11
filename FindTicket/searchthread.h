#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H
#include <QtCore>
#include <QtWidgets>

class SearchThread : public QThread
{
    Q_OBJECT

public:
    explicit SearchThread();
    void run();

signals:
    void valueChanged(int);
    void threadFinished();
};

#endif // SEARCHTHREAD_H
