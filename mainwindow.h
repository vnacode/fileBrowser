#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <threadworker.h>

#define TO_STR(x) #x







namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Q_SLOT void printBlock(const Block &block);
    Q_SLOT void printMainBlock(const Block &block);
    Q_SLOT void printNumSubDirs(const QString &number);
    Q_SLOT void change_current_directory(const QString &path);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ThreadWorker *threadworker;
    Q_SLOT void clearStatistic(const QModelIndex &index);
};

#endif // MAINWINDOW_H
