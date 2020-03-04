#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QModelIndex>






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    threadworker(new ThreadWorker)
{
    qRegisterMetaType<QModelIndex>(TO_STR(QModelIndex));
    qRegisterMetaType<Block>(TO_STR(Block));

    ui->setupUi(this);
    ui->treeView->setModel(threadworker->get_model());
    ui->treeView->setSortingEnabled(true);
    ui->treeView->setColumnWidth(0,ui->treeView->width() / 2);


    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), threadworker, SLOT(change_directory(QModelIndex)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(clearStatistic(QModelIndex)));
    connect(threadworker, SIGNAL(send_block(Block)), this, SLOT(printBlock(Block)));
    connect(threadworker, SIGNAL(send_common_block(Block)), this, SLOT(printMainBlock(Block)));
    connect(threadworker, SIGNAL(send_NumSubDirs(QString)), this, SLOT(printNumSubDirs(QString)));
    connect(threadworker, SIGNAL(directory_changed(QString)), this, SLOT(change_current_directory(QString)));

}

void MainWindow::printBlock(const Block &block)
{
 ui->textBrowser->append("\nФайлы: " + block.extension);
 ui->textBrowser->append("Количество файлов:" + QString::number(block.numfiles));
 ui->textBrowser->append("Размер файлов:" +  locale().formattedDataSize(block.size));
 ui->textBrowser->append("Средний размер файлов:" + locale().formattedDataSize(block.avgsize));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete threadworker;
}

 void MainWindow::clearStatistic(const QModelIndex &index)
{
    ui->textBrowser->clear();
}

void MainWindow::printMainBlock(const Block &block)
{
    ui->AllSize->setText(locale().formattedDataSize(block.size));
    ui->AvgSize->setText(locale().formattedDataSize(block.avgsize));
    ui->NumFiles->setText(QString::number(block.numfiles));
}

void MainWindow::printNumSubDirs(const QString &number)
{
    ui->NumSubDirs->setText(number);
}

void MainWindow::change_current_directory(const QString &path)
{
    ui->Statistic->setTitle("Статистика директории: " + path);
}
