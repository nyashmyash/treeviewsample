#include "worker.h"
#include <QImageReader>

Worker::Worker(QString s, QSize size) : path(s), size(size), terminate(false)
{

}

void Worker::run()
{
    QStringList thumbnails;
    if(!path.isEmpty())
    {
        QDir dir(path);
        QStringList filter;
        QList<QByteArray> formats = QImageReader::supportedImageFormats();
        for(int i = 0; i< formats.length(); i++)
        {
            filter << "*." + QString(formats[i]);
        }
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
        if (terminate)
            break;
        if ( !pixmap.isNull() )
        {
           emit send(pixmap);
        }
    }
}

void Worker::reciveBoolStop()
{
    terminate = true;
}
