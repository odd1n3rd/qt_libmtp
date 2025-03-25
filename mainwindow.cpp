#include "mainwindow.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), viewModel(new MtpViewModel(this)) {
    deviceInfoLabel = new QLabel("No device connected", this);
    fileListWidget = new QListWidget(this);
    refreshButton = new QPushButton("Refresh Device", this);
    createDirButton = new QPushButton("Create Directory", this);
    deleteDirButton = new QPushButton("Delete Directory", this);
    writeFileButton = new QPushButton("Write File", this);
    readFileButton = new QPushButton("Read File", this);
    deleteFileButton = new QPushButton("Delete File", this);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(deviceInfoLabel);
    mainLayout->addWidget(fileListWidget);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(createDirButton);
    buttonLayout->addWidget(deleteDirButton);
    buttonLayout->addWidget(writeFileButton);
    buttonLayout->addWidget(readFileButton);
    buttonLayout->addWidget(deleteFileButton);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);

    connect(viewModel, &MtpViewModel::deviceUpdated, this, &MainWindow::updateDeviceInfo);
    connect(viewModel, &MtpViewModel::fileListUpdated, this, &MainWindow::updateFileList);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshDevice);
    connect(createDirButton, &QPushButton::clicked, this, &MainWindow::onCreateDirectory);
    connect(deleteDirButton, &QPushButton::clicked, this, &MainWindow::onDeleteDirectory);
    connect(writeFileButton, &QPushButton::clicked, this, &MainWindow::onWriteFile);
    connect(readFileButton, &QPushButton::clicked, this, &MainWindow::onReadFile);
    connect(deleteFileButton, &QPushButton::clicked, this, &MainWindow::onDeleteFile);


    setWindowTitle("MTP Device Manager");
    resize(600, 400);
    updateDeviceInfo();
}

MainWindow::~MainWindow() {}

void MainWindow::updateDeviceInfo() {
    QString info = viewModel->deviceInfo() + " - Free: " + viewModel->freeSpace();
    deviceInfoLabel->setText(info);
    updateFileList(viewModel->fileList());
}

void MainWindow::updateFileList(const QStringList &files) {
    fileListWidget->clear();
    fileListWidget->addItems(files);
}

void MainWindow::refreshDevice() {
    viewModel->refreshDevice();
}

void MainWindow::onCreateDirectory() {
    bool ok;
    QString dirName = QInputDialog::getText(this, "Create Directory", "Enter directory name:", QLineEdit::Normal, "", &ok);
    if (ok && !dirName.isEmpty()) {
        viewModel->createDirectory(dirName);
        updateFileList(viewModel->fileList());
    }
}

void MainWindow::onDeleteDirectory() {
    QListWidgetItem *item = fileListWidget->currentItem();
    if (item) {
        QString path = item->text();
        viewModel->deleteDirectory(path);
        updateFileList(viewModel->fileList());
    } else {
        QMessageBox::warning(this, "Error", "Select a directory to delete.");
    }
}

void MainWindow::onWriteFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select File to Write", "", "All Files (*)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QString destPath = QFileInfo(filePath).fileName();
            viewModel->writeFile(destPath, data);
            updateFileList(viewModel->fileList());
            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Failed to open file.");
        }
    }
}

void MainWindow::onReadFile() {
    QListWidgetItem *item = fileListWidget->currentItem();
    if (item) {
        QString path = item->text();
        viewModel->readFile(path);

    } else {
        QMessageBox::warning(this, "Error", "Select a file to read.");
    }
}

void MainWindow::onDeleteFile() {
    QListWidgetItem *item = fileListWidget->currentItem();
    if (item) {
        QString path = item->text();
        viewModel->deleteFile(path);
        updateFileList(viewModel->fileList());
    } else {
        QMessageBox::warning(this, "Error", "Select a file to delete.");
    }
}
