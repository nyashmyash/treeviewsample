#include "mainwindow.h"
#include "worker.h"
#include <windows.h>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    treeFilesModel = new QFileSystemModel();
    treeFilesModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    treeFilesModel->setRootPath(QDir::rootPath());
    treeFilesModel->setResolveSymlinks(true);
    ui->treeView->setModel(treeFilesModel);
    ui->treeView->header()->hide();
    for (int i = 1; i < treeFilesModel->columnCount(); ++i)
    {
        ui->treeView->hideColumn(i);
    }
    modelListView   =     new QStandardItemModel(this);
    proxyModel	= new QSortFilterProxyModel(this);
    connect(ui->treeView, &QTreeView::clicked,this, [=](const QModelIndex& index)
    {
        QString path = "";
        QModelIndex prev = index;
        while(prev.isValid())
        {
            path = prev.data().toString() + "\\" + path;
            prev = prev.parent();
        }

        setDataInListViewThread(path, QSize(200, 200));
    });

    proxyModel->setSourceModel(modelListView);
    proxyModel->setFilterKeyColumn(0);
    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setGridSize(QSize(200, 200));
    ui->listView->setModel(modelListView);
    ui->listView->setEditTriggers(QAbstractItemView::AnyKeyPressed |	QAbstractItemView::DoubleClicked);

}
void MainWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numDegrees.isNull()) {
        QSize size = ui->listView->gridSize();
        if (numDegrees.y()>0 && size.height() >199 && size.width()>199)
        {
            event->accept();
            return;
        }

        if (numDegrees.y()<0 && size.height()<51 && size.width()<51)
        {
            event->accept();
            return;
        }
        int cnt = modelListView->rowCount();
        size = QSize(size.width()+numDegrees.y(), size.height()+numDegrees.y());
        modelListView->removeRows( 0, modelListView->rowCount() );
        modelListView->insertRows(0, cnt);
        for(int i = 0; i < cnt; i++)
        {
            QPixmap pic =listPixmaps[i].scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            modelListView->setData(modelListView->index(i,0), pic, Qt::DecorationRole);
        }
        ui->listView->setGridSize(size);
    }
    event->accept();
}

void MainWindow::setDataInListViewThread(QString folderPath, QSize size)
{
    modelListView->removeRows( 0, modelListView->rowCount() );

    modelListView->insertColumn(0);
    //modelListView->insertRows(0,numRows);
    listPixmaps.clear();

    QThread *thread= new QThread;
    Worker *my = new Worker(folderPath, size);

    my->moveToThread(thread);

    connect(my, SIGNAL(send(QPixmap)), this, SLOT(update(QPixmap)));
    connect(thread, SIGNAL(started()), my, SLOT(doWork()));

    thread->start();
}

void MainWindow::update (QPixmap p)
{
    listPixmaps.append(p);
    modelListView->insertRow(modelListView->rowCount());
    modelListView->setData( modelListView->index(modelListView->rowCount()-1,0), p, Qt::DecorationRole);
}

MainWindow::~MainWindow()
{
    delete ui;
}

