#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mtpviewmodel.h"

class QListWidget;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateDeviceInfo();
    void updateFileList(const QStringList &files);
    void refreshDevice();
    void onCreateDirectory();
    void onDeleteDirectory();
    void onWriteFile();
    void onReadFile();
    void onDeleteFile();

private:
    MtpViewModel *viewModel;
    QListWidget *fileListWidget;
    QLabel *deviceInfoLabel;
    QPushButton *refreshButton;
    QPushButton *createDirButton;
    QPushButton *deleteDirButton;
    QPushButton *writeFileButton;
    QPushButton *readFileButton;
    QPushButton *deleteFileButton;
};

#endif // MAINWINDOW_H
