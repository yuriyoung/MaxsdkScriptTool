#include "archivereader.h"
#include "zip.h"
#include "unzip.h"

#include <QObject>
#include <QFile>
#include <QDir>
#include <QMessageBox>

#include <QDebug>

/*
    压缩包内容格式必须为 (/类型名文件夹/文件s)
*/
ArchiveReader::ArchiveReader()
{
}

// 这个太慢了
QString ArchiveReader::readArchiveFile(const QString &title, const QString &archiveFile, const QString &password)
{
    QString data;
    if( title.isEmpty() )
        return data;
    if ( !QFile::exists(archiveFile) )
    {
        qDebug() << "File does not exist.";
        QMessageBox::information(0, title, "File dose not exist:" + title, 0);
        return data;
    }

    UnZip::ErrorCode ec;
    UnZip uz;

    if ( !password.isEmpty() )
        uz.setPassword(password);

    ec = uz.openArchive( archiveFile );
    if ( ec != UnZip::Ok )
    {
        qDebug() << "Unable to open archive: " << uz.formatError(ec);
        QString err = "Unable to open archive: " + uz.formatError(ec);
        QMessageBox::information(0, title, err, 0);
        return data;
    }

    QList<UnZip::ZipEntry> list = uz.entryList();
    if ( !list.isEmpty() )
    {
        for (int i = 0; i < list.size(); ++i)
        {
            const UnZip::ZipEntry& entry = list.at(i);

            QString file = entry.filename;

            // 只取文件名,去掉文件夹名
            QString basename = file;
            int idx = file.lastIndexOf("/");    //idx==file.length()-1 是文件夹
            if (idx >= 0 && idx != file.length()-1)
            {
                basename = file.right(file.length() - idx - 1);
            }
            if( basename == title/*.contains( title, Qt::CaseSensitive) || title.contains( file, Qt::CaseSensitive)*/ )
            {
                QString tempDir = QDir::tempPath();
                if ( UnZip::Ok == uz.extractFile(file, tempDir) )
                {

                    QString tempFile = tempDir + "/" + file;
                    data = tempFile;
//                    QMessageBox::information(0, title, tempFile, 0);
                }
            }
        }
    }

    uz.closeArchive();
    return data;
}

QStringList ArchiveReader::listFiles(const QString &archiveFile, const QString &password)
{
    QStringList fileList;
    if ( !QFile::exists(archiveFile) )
    {
        qDebug() << "File does not exist.";
        QString err = QObject::tr("File does not exist: ") + archiveFile;
        QMessageBox::information(0, QObject::tr("error"), err, QMessageBox::Ok);
        return fileList;
    }

    UnZip::ErrorCode ec;
    UnZip uz;

    if ( !password.isEmpty() )
        uz.setPassword(password);

    ec = uz.openArchive( archiveFile );
    if ( ec != UnZip::Ok )
    {
        qDebug() << "Unable to open archive: " << uz.formatError(ec);
        QString err = QObject::tr("Unable to open archive: ") + uz.formatError(ec);
        QMessageBox::information(0, QObject::tr("error"), err, QMessageBox::Ok);
        return fileList;
    }

    QList<UnZip::ZipEntry> list = uz.entryList();
    if ( !list.isEmpty() )
    {
        for (int i = 0; i < list.size(); ++i)
        {
            const UnZip::ZipEntry& entry = list.at(i);
            QString file = entry.filename;
//            // 只取文件名,去掉文件夹名
//            int idx = file.lastIndexOf("/");    //idx==file.length()-1 是文件夹
//            if (idx >= 0 && idx != file.length()-1)
//            {
//                file = file.right(file.length() - idx - 1);
//            }
//            if( idx == file.length() )
//                continue;
            fileList.append( file );
        }
    }

    uz.closeArchive();
    return fileList;
}

QStringList ArchiveReader::extractArchive(const QString &archive, const QString& password)
{
    QStringList fileList;
    UnZip::ErrorCode ue;
    UnZip unzip;

    if (!password.isEmpty())
        unzip.setPassword(password);

    ue = unzip.openArchive(archive);
    if( ue != UnZip::Ok )
    {
        qDebug() << "Unable to open archive: " << unzip.formatError(ue);
        return fileList;
    }

    QList<UnZip::ZipEntry> files = unzip.entryList();
    if ( files.isEmpty() )
        return fileList;

    QStringList list;
    for (int i = 0; i < files.size(); ++i)
    {
        const UnZip::ZipEntry& entry = files.at(i);
        QString file = entry.filename;
        list.append( file );
    }
    QString tempDir = QDir::tempPath();
    ue = unzip.extractFiles(list, tempDir);
    if( ue != UnZip::Ok )
    {
        qDebug() << "Extract archive failed: " << unzip.formatError(ue);
        return fileList;
    }

    foreach( QString f, list )
    {
        fileList << tempDir + "/" + f;
    }

    return fileList;
}

bool ArchiveReader::reCompressFiles(const QStringList &files, const QString &archive, const QString &password)
{
    Zip::ErrorCode ec;
    Zip zip;
    ec = zip.createArchive(archive);
    if (ec != Zip::Ok)
    {
        qDebug() << "Unable to create archive: " << zip.formatError(ec).toAscii().data();
        return false;
    }
    zip.setPassword(password);

    zip.addFiles(files);
    if (ec != Zip::Ok)
    {
        qDebug() << "Unable to add file: " << zip.formatError(ec).toAscii().data();
    }

    foreach( QString file, files )
        QFile::remove(file);

    return true;
}

bool ArchiveReader::reCompressFile(const QString &file, const QString &archive, const QString &password)
{
    Zip::ErrorCode ec;
    Zip zip;
    ec = zip.createArchive(archive);
    if (ec != Zip::Ok)
    {
        qDebug() << "Unable to create archive: " << zip.formatError(ec).toAscii().data();
        return false;
    }
    zip.setPassword(password);

    zip.addFile(file);
    if (ec != Zip::Ok)
    {
        qDebug() << "Unable to add file: " << zip.formatError(ec).toAscii().data();
    }

    QFile::remove(file);

    return true;
}
