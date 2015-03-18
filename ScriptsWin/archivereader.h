
#ifndef ARCHIVEREADER_H
#define ARCHIVEREADER_H

#include <QObject>

class ArchiveReader
{
public:
    explicit ArchiveReader();

    /* 根据给的 filename ,从包中取出该文件,并读取返回该文件的内容 */
    QString readArchiveFile(const QString &title, const QString& archiveFile, const QString& password);
    /*  反复的从包中读取文件, 对速度有影响,是否可以考虑先把整个包数据读取到内存,调用文件时便可直接从内存中取该文件内容? */
//    QVector<QByteArray> filesData;

    // 列出包内所有文件
    QStringList listFiles(const QString& archiveFile, const QString& password);
    
    // 解压包
    QStringList extractArchive(const QString &archive, const QString& password);

    // 然后再重新打上包
    bool reCompressFiles(const QStringList &files, const QString &archive, const QString &password);
    bool reCompressFile(const QString &file, const QString &archive, const QString &password);

};

#endif // ARCHIVEREADER_H
