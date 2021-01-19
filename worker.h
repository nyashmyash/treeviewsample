#ifndef WORKER_H
#define WORKER_H
#include "mainwindow.h"

class Worker:public QObject
{
   Q_OBJECT
public:
    Worker(QString path, QSize size);
public slots:
    void doWork();
signals:
    void send(QPixmap);
private:
    QString path;
    QSize size;
};

#endif // WORKER_H
