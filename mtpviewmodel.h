#ifndef MTPVIEWMODEL_H
#define MTPVIEWMODEL_H

#include <QObject>
#include "mtpdevice.h"

class MtpViewModel : public QObject {
    Q_OBJECT

public:
    explicit MtpViewModel(QObject *parent = nullptr);

    QString deviceInfo() const;
    QString freeSpace() const;
    QStringList fileList();

public slots:
    void refreshDevice();
    void readFile(const QString &path);
    void writeFile(const QString &path, const QByteArray &data);
    void deleteFile(const QString &path);
    void createDirectory(const QString &path);
    void deleteDirectory(const QString &path);

signals:
    void deviceUpdated();
    void fileListUpdated(const QStringList &files);
    void fileRead(const QByteArray &data);
    void operationFailed(const QString &error);

private:
    MtpDevice *mtpDevice;
};

#endif // MTPVIEWMODEL_H
