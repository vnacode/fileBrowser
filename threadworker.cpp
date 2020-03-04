#include "threadworker.h"
#include <QDirIterator>


Model::Model(QFileSystemModel *parent)
    : QFileSystemModel(parent)
{
    this->setRootPath("");
    this->iconProvider()->setOptions(QFileIconProvider::DontUseCustomDirectoryIcons);
}




ThreadWorker::ThreadWorker(QObject *parent)
    : QThread(parent),
      model(new Model)
{

}



Model *ThreadWorker::get_model() const
{
    if (model == nullptr)
        return new Model();
    return model;
}

void ThreadWorker::change_directory(const QModelIndex &index)
{
    if (index.isValid())
    {
        directory = model->filePath(index);
        if (model->fileInfo(index).isFile())
            return;

        if (this->_isRunning)
        {
            this->_isRunning = false;
            this->wait();
            blocks.clear();
        }
        emit directory_changed(directory.path());

        if (!directory.isEmpty())
            this->start();
        else
            send_error();

    }
}

void ThreadWorker::run()
{
    _isRunning = true;
    uint numdirs = directory.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs).count();
    emit send_NumSubDirs(QString::number(numdirs));

    Block block;
    Block common_block;
    directory.setSorting(QDir::Type);
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    QDirIterator itr(directory,QDirIterator::Subdirectories);
    while (itr.hasNext() && _isRunning) {

        itr.next();
        if (itr.fileInfo().suffix() != block.extension)
        {
            prepare_block(block);
            prepare_common_block(common_block,block);
            send_common_block(common_block);
            block.clear();
        }
        block.extension = itr.fileInfo().suffix();
        block.numfiles++;
        block.size += itr.fileInfo().size();
    }

    if (block.numfiles > 0)
    {
        prepare_common_block(common_block,block);
        prepare_block(block);
        block.clear();
    }

    emit send_common_block(common_block);
    send_blocks();
}



void ThreadWorker::send_error()
{
    emit send_common_block(Block());
    emit send_NumSubDirs("0");
}


void ThreadWorker::prepare_block(const Block &block)
{
    bool founded = false;
    for (Block &_block : blocks)
    {
        if (block.extension == _block.extension)
        {
            founded = true;
            _block.size += block.size;
            _block.numfiles += block.numfiles;
            if (_block.numfiles > 0)
                _block.avgsize = _block.size / _block.numfiles;
        }
    }
    if (!founded && block.numfiles > 0)
    {
        blocks.append(block);
    }

}

void ThreadWorker::prepare_common_block(Block &common_block, const Block &block)
{
    common_block.size += block.size;
    common_block.numfiles += block.numfiles;

    if (common_block.numfiles > 0)
        common_block.avgsize = common_block.size / common_block.numfiles;
}

void ThreadWorker::send_blocks()
{
    for (Block &_block : blocks)
    {
        if (!_isRunning)
            return;
        if (_block.extension == "")
            _block.extension = "Без расширения";
        send_block(_block);
    }
}


