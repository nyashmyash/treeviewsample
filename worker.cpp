#include "worker.h"

Worker::Worker(QString s, QSize size) : path(s), size(size)
{

}

void Worker::doWork()
{
    QStringList thumbnails;
    if(!path.isEmpty())
    {
        QDir dir(path);
        QStringList filter;
        filter << QLatin1String("*.png");
        filter << QLatin1String("*.jpeg");
        filter << QLatin1String("*.jpg");
        dir.setNameFilters(filter);
        QFileInfoList filelistinfo = dir.entryInfoList();
        foreach (const QFileInfo &fileinfo, filelistinfo)
        {
            thumbnails << fileinfo.absoluteFilePath();
        }
    }
    else
    {
        return;
    }
    const int numRows = thumbnails.size();
    for(int i=0;i<numRows ;++i)
    {
        QPixmap  pixmap( thumbnails.at( i ) );
        pixmap = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        if ( !pixmap.isNull() )
        {
           emit send(pixmap);
        }
    }
}
