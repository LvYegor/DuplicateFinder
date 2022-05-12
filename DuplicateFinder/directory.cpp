#include "directory.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <QMessageBox>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "widget.h"

#include <QDebug>

Directory::Directory(QThread *parent)
    : QThread{parent}
{

}

Directory::~Directory()
{

}

void Directory::setDirectrory(char *_directory)
{
    strncpy(this->introducedDir, _directory, PATH_MAX);
}


void Directory::directoryScan(const char *introducedDir, char *option, int fl)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char pathName[PATH_MAX + 1];
    dir = opendir(introducedDir);
    if(dir == NULL) {
        emit errorExistDirect();
        return;
    }
    entry = readdir(dir);
    while(entry != NULL) {
        struct stat entryInfo;
        if((strncmp(entry->d_name, ".", PATH_MAX) == 0) || (strncmp(entry->d_name, "..", PATH_MAX) == 0)) {
            entry = readdir(dir);
            continue;
        }
        (void)strncpy(pathName, introducedDir, PATH_MAX);
        (void)strncat(pathName, "/", PATH_MAX);
        (void)strncat(pathName, entry->d_name, PATH_MAX);
        if(lstat(pathName, &entryInfo) == 0) {
            if(S_ISDIR(entryInfo.st_mode)) {
                directoryScan(pathName, option, fl);
            }
            else if(S_ISREG(entryInfo.st_mode)) {
                if(strstr(option, "f") != NULL || fl == 1) {                   
                    emit appendPath(pathName);
                }
            }
        }
        entry = readdir(dir);
    }
    (void)closedir(dir);
    return;
}

void Directory::run()
{
    directoryScan(introducedDir, option, 1);
}
