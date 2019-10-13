#include <QDebug>
#include <chrono>
#include <thread>
#include "searchthread.h"
#include <QtWidgets>

SearchThread::SearchThread()
{
}

void SearchThread::run()
{
    for (int i = 0; i <= 100; i++)
    {
        emit valueChanged(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

}
