#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QObject>
#include <QFileSystemModel>
#include <QThread>
#include <QFileIconProvider>
#include <QDir>
#include <QMutex>
#include <QDebug>
class Model : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit Model(QFileSystemModel *parent = nullptr);

};


struct Block
{
    QString extension;
    uint numfiles;
    qint64 size;
    qint64 avgsize;
    Block()
    {
      clear();
    }

    void clear() {
        extension = "";
        numfiles = 0;
        size  = 0;
        avgsize = 0;
    }
};

class ThreadWorker : public QThread
{
    Q_OBJECT
public:
    explicit ThreadWorker(QObject *parent = nullptr);
    Model* get_model() const;
    Q_SLOT   void change_directory(const QModelIndex &index);
    Q_SIGNAL void send_NumSubDirs(const QString &size);
    Q_SIGNAL void send_block(const Block& block);
    Q_SIGNAL void send_common_block(const Block& block);
    Q_SIGNAL void directory_changed(const QString &_direcory);
protected:
    void run();

private:
    void send_error();
    void prepare_block(const Block &block);
        void prepare_common_block(Block &common_block, const Block &block);
    void send_blocks();
    bool _isRunning = false;
    Model *model;
    QDir directory;
    QVector<Block> blocks;
};

#endif // THREADWORKER_H
