#include "mtpdevice.h"
#include <QDebug>

MtpDevice::MtpDevice(QObject *parent) : QObject(parent), device(nullptr), rawDevice(nullptr) {
    LIBMTP_Init();
}

MtpDevice::~MtpDevice() {
    close();
}

void MtpDevice::close() {
    if (device) {
        LIBMTP_Release_Device(device);
        device = nullptr;
        qDebug() << "Device connection closed";
        emit deviceDisconnected();
    }
    if (rawDevice) {
        free(rawDevice);
        rawDevice = nullptr;
    }
}

bool MtpDevice::initialize() {
    close();

    LIBMTP_raw_device_t *devices = nullptr;
    int numDevices = 0;
    LIBMTP_error_number_t error;

    error = LIBMTP_Detect_Raw_Devices(&devices, &numDevices);
    if (error != LIBMTP_ERROR_NONE || numDevices == 0) {
        qDebug() << "No devices found or error occurred during detection:" << error;
        LIBMTP_Dump_Errorstack(nullptr);
        return false;
    }

    qDebug() << "Found" << numDevices << "MTP devices";
    rawDevice = devices;

    device = LIBMTP_Open_Raw_Device(&rawDevice[0]);
    if (!device) {
        qDebug() << "Failed to open device. Dumping error stack:";
        LIBMTP_Dump_Errorstack(nullptr);
        return false;
    }

    qDebug() << "Device initialized successfully";
    emit deviceConnected();
    return true;
}

QString MtpDevice::getDeviceVersion() const {
    if (!device) return QString();
    char *version = LIBMTP_Get_Deviceversion(device);
    QString result = version ? QString(version) : "Unknown";
    free(version);
    return QString("MTP Version: %1").arg(result);
}

QString MtpDevice::getDeviceInfo() const {
    if (!device) return QString();
    char *friendlyName = LIBMTP_Get_Friendlyname(device);
    QString info = friendlyName ? QString(friendlyName) : "Unknown Device";
    free(friendlyName);
    return info;
}

uint64_t MtpDevice::getFreeSpace() const {
    if (!device) {
        qDebug() << "No device connected";
        return 0;
    }

    qDebug() << "Requesting storage info...";
    LIBMTP_Clear_Errorstack(device);
    int result = LIBMTP_Get_Storage(device, 0);
    if (result != 0 || !device->storage) {
        qDebug() << "LIBMTP_Get_Storage failed or no storage available. Code:" << result;
        LIBMTP_Dump_Errorstack(device);
        return 0;
    }

    uint64_t totalFreeSpace = 0;
    LIBMTP_devicestorage_t *storage = device->storage;
    while (storage) {
        qDebug() << "Storage ID:" << storage->id
                 << "Free space:" << storage->FreeSpaceInBytes
                 << "Total space:" << storage->MaxCapacity;
        totalFreeSpace += storage->FreeSpaceInBytes;
        storage = storage->next;
    }
    qDebug() << "Total free space:" << totalFreeSpace << "bytes";
    return totalFreeSpace;
}

QStringList MtpDevice::getFileList(const QString &path) const {
    if (!device) {
        qDebug() << "No device connected";
        return QStringList();
    }

    qDebug() << "Requesting file list for path:" << path;
    QStringList fileList;


    LIBMTP_Clear_Errorstack(device);


    int result = LIBMTP_Get_Storage(device, 0);
    uint32_t storage_id = 0;
    if (result == 0 && device->storage) {
        storage_id = device->storage->id;
        qDebug() << "Using storage ID:" << storage_id;
    } else {
        qDebug() << "No storage available, using default storage ID 0";

        LIBMTP_Clear_Errorstack(device);
    }


    LIBMTP_file_t *files = LIBMTP_Get_Files_And_Folders(device, storage_id, 0);
    if (!files) {
        qDebug() << "Failed to get file list";
        LIBMTP_Dump_Errorstack(device);
        // mock func
        qDebug() << "Returning test file list due to failure";
        fileList << "DCIM/" << "Pictures/" << "test.txt";
        return fileList;
    }

    LIBMTP_file_t *file = files;
    while (file) {
        QString fileName = QString(file->filename);
        if (file->filetype == LIBMTP_FILETYPE_FOLDER) {
            fileName += "/";
        }
        fileList << fileName;
        qDebug() << "Found:" << fileName << "Type:" << file->filetype;
        file = file->next;
    }

    LIBMTP_destroy_file_t(files);
    qDebug() << "Total files/folders found:" << fileList.size();
    return fileList;
}

// mock
bool MtpDevice::readFile(const QString &path, QByteArray &data) { return false; }
bool MtpDevice::writeFile(const QString &path, const QByteArray &data) { return false; }
bool MtpDevice::deleteFile(const QString &path) { return false; }
bool MtpDevice::createDirectory(const QString &path) { return false; }
bool MtpDevice::deleteDirectory(const QString &path) { return false; }
