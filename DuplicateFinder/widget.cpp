#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <unistd.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    direct = new Directory();
    md5Hash = new MD5HashFile();
    md5Thread = new QThread();
    amountOfFiles = 0;

    files.clear();
    this->hashedFiles.clear();

    direct->moveToThread(direct);
    md5Hash->moveToThread(md5Thread);

    md5Thread->start();

    connect(this, &Widget::directoryScan, direct, &Directory::directoryScan);
    connect(direct, &Directory::errorExistDirect, this, &Widget::errorExistDirect);
    connect(direct, &Directory::appendPath, this, &Widget::appendPath);
    connect(direct, &Directory::finished, this, &Widget::fileHashing);
    connect(this, &Widget::start_MD5_hashing, md5Hash, &MD5HashFile::start_MD5_hashing);
    connect(md5Hash, &MD5HashFile::appendFileHash, this, &Widget::appendFileHash);
    connect(this, &Widget::destroyed, md5Thread, &QThread::quit);


}

Widget::~Widget()
{
    delete direct;
    delete md5Hash;
    delete md5Thread;
    delete ui;
}


void Widget::on_toolButton_2_clicked()
{
    ui->lineEdit_8->setText(QFileDialog::getExistingDirectory());
}


void Widget::on_pushButton_2_clicked()
{
    if(ui->lineEdit_8->text().isEmpty())
    {
        QMessageBox::warning(this, "Error", "Empty text line");
        return;
    }

    direct->setDirectroty(ui->lineEdit_8->text().toUtf8().data());

    ui->stackedWidget->setCurrentIndex(1);
    direct->start();
}

void Widget::errorExistDirect()
{
    QMessageBox::warning(this, "Error", "Direct does not exist");
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::appendPath(const QString pathName)
{
    this->files.push(pathName);
    this->amountOfFiles += 1;

    ui->label_4->setText(pathName);
}

void Widget::fileHashing()
{
    emit start_MD5_hashing(files);
}

void Widget::appendFileHash(QString hash, QString path)
{
    if(!this->hashedFiles.contains(hash))
    {
        QVector<QString> newVector;
        newVector.clear();
        newVector.append(path);
        this->hashedFiles.insert(hash, newVector);
    }
    else
    {
        this->hashedFiles[hash].append(path);
    }

    ui->label_4->setText(hash);
}




