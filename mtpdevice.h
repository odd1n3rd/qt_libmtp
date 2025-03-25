#ifndef MTPDEVICE_H
#define MTPDEVICE_H

#include <libmtp.h>
#include <QString>
#include <QObject>

class MtpDevice : public QObject {
    Q_OBJECT

public:
    explicit MtpDevice(QObject *parent = nullptr);
    ~MtpDevice();

    bool initialize();
    void close();
    QString getDeviceVersion() const;
    QString getDeviceInfo() const;
    uint64_t getFreeSpace() const;
    QStringList getFileList(const QString &path) const;
    bool readFile(const QString &path, QByteArray &data);
    bool writeFile(const QString &path, const QByteArray &data);
    bool deleteFile(const QString &path);
    bool createDirectory(const QString &path);
    bool deleteDirectory(const QString &path);

signals:
    void deviceConnected();
    void deviceDisconnected();

private:
    LIBMTP_mtpdevice_t *device;
    LIBMTP_raw_device_t *rawDevice;
};

#endif // MTPDEVICE_H
