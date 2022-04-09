#include "md5HashFile.h"
#include "md5.h"
#include "fstream"
#include <QDebug>

MD5HashFile::MD5HashFile(QObject *parent)
    : QObject{parent}
{

}

void MD5HashFile::start_MD5_hashing(QStack<QString> files)
{
    while(!files.isEmpty()){
        QString path = files.pop();
        emit appendFileHash(this->md5_file(path), path);
    }
}

QString MD5HashFile::md5_file(QString filename)
{
    std::ifstream fin(filename.toUtf8().data());
    std::array<char, 1024> buffer{};
    MD5 hash;
    while (std::streamsize n = fin.readsome(buffer.data(), 1024)) {
        hash.update(buffer.data(), n);
    }
    // TODO: Return std::array<char, 16> instead of std::string
    return hash.finalize().hexdigest();
}


