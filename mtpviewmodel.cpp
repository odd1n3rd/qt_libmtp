#include "mtpviewmodel.h"

MtpViewModel::MtpViewModel(QObject *parent) : QObject(parent), mtpDevice(new MtpDevice(this)) {
    connect(mtpDevice, &MtpDevice::deviceConnected, this, &MtpViewModel::deviceUpdated);
    mtpDevice->initialize();
}

QString MtpViewModel::deviceInfo() const {
    return mtpDevice->getDeviceInfo() + " (" + mtpDevice->getDeviceVersion() + ")";
}

QString MtpViewModel::freeSpace() const {
    uint64_t bytes = mtpDevice->getFreeSpace();
    if (bytes == 0) {
        return "Unknown";
    }
    return QString::number(bytes / (1024 * 1024)) + " MB";
}

QStringList MtpViewModel::fileList() {
    QStringList files = mtpDevice->getFileList("");
    emit fileListUpdated(files);
    return files;
}

void MtpViewModel::refreshDevice() {
    if (mtpDevice->initialize()) {
        emit deviceUpdated();
        emit fileListUpdated(fileList());
    }
}

// mock
void MtpViewModel::readFile(const QString &path) {}
void MtpViewModel::writeFile(const QString &path, const QByteArray &data) {}
void MtpViewModel::deleteFile(const QString &path) {}
void MtpViewModel::createDirectory(const QString &path) {}
void MtpViewModel::deleteDirectory(const QString &path) {}
