#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QFileSystemModel>
#include <QWheelEvent>
#include <QVector>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setDataInListView(QString path, QSize size);
    void setDataInListViewThread(QString folderPath, QSize size);
    void wheelEvent(QWheelEvent *event);

private:
    Ui::MainWindow *ui;
    QVector<QPixmap> listPixmaps;
    QFileSystemModel *treeFilesModel;
    QStandardItemModel *modelListView;
    QSortFilterProxyModel *proxyModel;
public slots:
    void update (QPixmap i);

};




#endif // MAINWINDOW_H
