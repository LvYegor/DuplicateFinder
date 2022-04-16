#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QMessageBox>
#include <unistd.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("/home/lavash/SPOVM/CourseProject/logo.png"));
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
    connect(md5Thread, &QThread::finished, this, &Widget::createTreeDisplay);





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

    direct->setDirectrory(ui->lineEdit_8->text().toUtf8().data());

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
    md5Thread->quit();
}

void Widget::appendFileHash(const QString hash, const QString path)
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

void Widget::createTreeDisplay()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->treeWidget->setColumnCount(1);

    for(auto pathVector : hashedFiles)
    {
        if(pathVector.size() > 1)
        {
            QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget);
            root->setBackground(0, Qt::gray);
            root->setText(0, pathVector[0]);
//            root->setCheckState(0, Qt::Unchecked);
            root->setExpanded(true);
            ui->treeWidget->addTopLevelItem(root);

            for(auto path : pathVector)
            {
                QTreeWidgetItem *child = new QTreeWidgetItem();
                child->setText(0, path);
                child->setCheckState(0, Qt::Checked);
                root->addChild(child);
            }
        }
    }
    if(ui->treeWidget->topLevelItemCount() == 0)
    {
        QMessageBox::information(this, "", "There are no duplicates.");
    }
}


void Widget::on_pushButton_clicked()
{
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        for (int j = 0; j < ui->treeWidget->topLevelItem(i)->childCount(); j++)
        {
            QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i)->child(j);
            if (item->checkState(0) == Qt::Checked)
                QFile::remove(item->text(0));
        }
    }
    QMessageBox::information(this, "Complete", "Duplicates has been deleted.");
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

