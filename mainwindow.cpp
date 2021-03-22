#include <Python.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databaseaccess.h"
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QToolBar>
#include <QToolButton>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QSaveFile>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDir>
#include <QProgressDialog>
#include "mapview.h"
#include "helper.h"
#include "settingsdialog.h"
#include "model/filterpointsmodel.h"
#include "model/pointsmodel.h"
#include "delegate/pointitemdelegate.h"
#include "delegate/checkboxitemdelegate.h"
#include "qgroupheaderview.h"
#include "filedownloader.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_airwaysModel = new QStandardItemModel(this);
    QSortFilterProxyModel *searchAirfieldsModel = new QSortFilterProxyModel(this);
    searchAirfieldsModel->setSourceModel(m_airwaysModel);
    searchAirfieldsModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->airwayListView->setModel(searchAirfieldsModel);

    m_pointsModel = new PointsModel(this);
    FilterPointsModel *filterPointsModel = new FilterPointsModel(this);
    filterPointsModel->setSourceModel(m_pointsModel);
    filterPointsModel->setFilterKeyColumn(1);
    QGroupHeaderView *groupHeaderView = new QGroupHeaderView(Qt::Horizontal, ui->pointsTableView);
    groupHeaderView->setCheckable(true);
    ui->pointsTableView->setHorizontalHeader(groupHeaderView);
    ui->pointsTableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->pointsTableView->setModel(filterPointsModel);
    ui->pointsTableView->setItemDelegate(new PointItemDelegate(this));
    ui->pointsTableView->setItemDelegateForColumn(0, new CheckboxItemDelegate(this));
    ui->pointsTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->pointsTableView->horizontalHeader()->setStretchLastSection(true);

    mapView = nullptr;
    toolBar = addToolBar(QString());

    exportButton = new QToolButton(this);
    exportButton->setEnabled(false);
    exportButton->setText(tr("Export"));
    exportButton->setIconSize(QSize(32, 32));
    exportButton->setIcon(QIcon(":/images/res/img/icons8-save-48.png"));
    exportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    displayOnMapButton = new QToolButton(this);
    displayOnMapButton->setEnabled(false);
    displayOnMapButton->setText(tr("Unhide"));
    displayOnMapButton->setIconSize(QSize(32, 32));
    displayOnMapButton->setIcon(QIcon(":/images/res/img/icons8-map-marker-48.png"));
    displayOnMapButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    updateButton = new QToolButton(this);
    updateButton->setText(tr("Update"));
    updateButton->setIconSize(QSize(32, 32));
    updateButton->setIcon(QIcon(":/images/res/img/icons8-downloading-updates-48.png"));
    updateButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    settingsButton = new QToolButton(this);
    settingsButton->setText(tr("Settings"));
    settingsButton->setIconSize(QSize(32, 32));
    settingsButton->setIcon(QIcon(":/images/res/img/icons8-settings-48.png"));
    settingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    toolBar->addWidget(exportButton);
    toolBar->addWidget(displayOnMapButton);
    toolBar->addWidget(updateButton);
    toolBar->addWidget(settingsButton);

    readSettings();
    updateModels();

    connect(ui->airwayListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(filterPoints(const QModelIndex&)));
    connect(ui->searchLineEdit, SIGNAL(textChanged(const QString&)), searchAirfieldsModel, SLOT(setFilterRegExp(QString)));
    connect(m_pointsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(enabledToolButton()));
    connect(exportButton, SIGNAL(clicked(bool)), this, SLOT(exportToFile()));
    connect(displayOnMapButton, SIGNAL(clicked(bool)), this, SLOT(showAirways()));
    connect(settingsButton, SIGNAL(clicked(bool)), this, SLOT(showSettings()));
    connect(updateButton, SIGNAL(clicked(bool)), SLOT(downloadSourceData()));
    connect(groupHeaderView, SIGNAL(clickedCheckBox(bool)), this, SLOT(setCheckedAllRowTable(bool)));
}

MainWindow::~MainWindow()
{
    writeSettings();
    if (mapView != nullptr) {
        mapView->close();
        delete mapView;
    }
    delete ui;
}

void MainWindow::updateModels()
{
    auto databaseAccess = DatabaseAccess::getInstance();
    QVector<Record> airways = databaseAccess->getAirways();

    // remove all rows
    m_airwaysModel->clear();

    for (QVector<Record>::const_iterator constIt = airways.constBegin(); constIt != airways.constEnd(); ++constIt) {
        QStandardItem *item = new QStandardItem((*constIt).first().toString());
        item->setCheckable(true);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        m_airwaysModel->appendRow(item);
    }
    ui->airwayListView->repaint();

    QVector<Record> points = databaseAccess->getPoints();
    // uncheked header
//    qobject_cast<QGroupHeaderView*>(ui->pointsTableView->horizontalHeader())->setChecked(false);
    QList<QVariant> data = QList<QVariant>();

    for (QVector<Record>::const_iterator constIt = points.constBegin(); constIt != points.constEnd(); ++constIt) {
        QList<QVariant> entry = QList<QVariant>();
        Record record = (*constIt);

        for (int i = 0; i < record.size(); i++)
            entry.append(record.value(i).toString());
        data.append(QVariant(entry));
    }
    m_pointsModel->setDataModel(data);
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.setValue("centralSplitter", ui->splitter->saveState());
    settings.setValue("maximized", this->isMaximized());
    settings.setValue(ui->pointsTableView->objectName(), ui->pointsTableView->horizontalHeader()->saveState());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    if (settings.contains("centralSplitter"))
        ui->splitter->restoreState(settings.value("centralSplitter").toByteArray());
    else {
        int widthMainwindow = this->size().width();
        ui->splitter->setSizes(QList<int>() << widthMainwindow / 4 << (widthMainwindow / 4) * 3);
    }
    if (settings.value("maximized").toBool())
        this->showMaximized();
    ui->pointsTableView->horizontalHeader()->restoreState(settings.value(ui->pointsTableView->objectName()).toByteArray());
    settings.endGroup();
    settings.beginGroup("connectDatabase");
    settings.endGroup();
}

void MainWindow::enabledToolButton()
{
    bool isEnable = false;
    for (int row = 0; row < m_pointsModel->rowCount(); row++) {
        if (m_pointsModel->index(row, 0).data(Qt::CheckStateRole).toBool()) {
            isEnable = true;
            break;
        }
    }
    exportButton->setEnabled(isEnable);
    displayOnMapButton->setEnabled(isEnable);
}

void MainWindow::showAirways()
{
    if (mapView == nullptr) {
        mapView = new MapView;
        connect(mapView, SIGNAL(checked(bool, QString, QString)), this, SLOT(setChecked(bool, QString, QString)));
    }

    mapView->clearMap();
    mapView->setTitle(tr("Airways"));

    bool setCenterMap = false;
    auto *filterPointsModel = qobject_cast<FilterPointsModel*>(ui->pointsTableView->model());
    auto *pointsModel = qobject_cast<PointsModel*>(filterPointsModel->sourceModel());

    QList<QVariant> points = {};
    QMap<QString, QVariant> point = {};
    QMap<QString, QString> args = {};

    for (int row = 0; row < filterPointsModel->rowCount(); row++) {
        if (!args.isEmpty() && args.value("nameAirway") != filterPointsModel->index(row, 1).data().toString()) {
            mapView->drawAirway(points, args);
            points.clear();
            args.clear();
        }

        if (args.isEmpty())
            args.insert("nameAirway", filterPointsModel->index(row, 1).data().toString());

        if (filterPointsModel->index(row, 0).data(Qt::CheckStateRole).toBool()) {
            QString codePoint = filterPointsModel->index(row, 2).data().toString();
            QString distance = QString::number(pointsModel->getDistance(filterPointsModel->mapToSource(filterPointsModel->index(row, 7))));
            double lat = Helper::convertCoordinateInDec(filterPointsModel->index(row, 3).data().toString());
            double lon = Helper::convertCoordinateInDec(filterPointsModel->index(row, 4).data().toString());
            QStringList mpu = {};
            mpu << pointsModel->getMagneticTrackAngle(filterPointsModel->mapToSource(filterPointsModel->index(row, 0)));

            if (!setCenterMap) {
                mapView->setCenter(QPointF(lat, lon));
                setCenterMap = true;
            }
            point["coordinate"] = QVariant(QPointF(lat, lon));
            point["info"] = QVariant(QString("%1\n%2").arg(filterPointsModel->index(row, 3).data().toString().right(1) + filterPointsModel->index(row, 3).data().toString().remove(-1, 1),
                                                           filterPointsModel->index(row, 4).data().toString().right(1) + filterPointsModel->index(row, 4).data().toString().remove(-1, 1)));        // Set coordinate for label on map
            point["code"] = QVariant(codePoint);
            point["distance"] = QVariant(distance);
            point["mpu"] = QVariant(mpu);
            points << point;
        }
        else
            points << QMap<QString, QVariant>();

    }
    // Draw last airway
    if (!args.isEmpty() && !points.isEmpty())
        mapView->drawAirway(points, args);

    setCheckedAllRowTable();
    mapView->show();
}

void MainWindow::showSettings()
{
    SettingsDialog settingsDialog(this);
    if (settingsDialog.exec() == QDialog::Accepted) {
        updateModels();
    }
}

void MainWindow::exportToFile()
{
    QString nameFile = QFileDialog::getSaveFileName(this, tr("Save file"), QString("D:/airway.txt"));
    if (nameFile.isEmpty()) {
        qDebug() << "Empty name save file";
        return;
    }

    QSaveFile file(nameFile);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }
    auto *filterPointsModel = qobject_cast<FilterPointsModel*>(ui->pointsTableView->model());
    auto *pointsModel = qobject_cast<PointsModel*>(filterPointsModel->sourceModel());

    QTextStream out(&file);
    QString nameAirway = QString();

    for (int row = 0; row < filterPointsModel->rowCount(); row++) {
        if (filterPointsModel->index(row, 0).data(Qt::CheckStateRole).toBool()) {
            if (nameAirway != filterPointsModel->index(row, 1).data().toString()) {
                if (!nameAirway.isEmpty())
                    out << '1' << endl;

                nameAirway = filterPointsModel->index(row, 1).data().toString();
                out << nameAirway << endl;      // Write name airway
            }
            out << filterPointsModel->index(row, 2).data().toString() << endl;                                                              // Name point
            out << filterPointsModel->index(row, 3).data().toString().remove(QRegExp("[\\s\\.]")) << endl;
            out << filterPointsModel->index(row, 4).data().toString().remove(QRegExp("[\\s\\.]")) << endl;
            out << pointsModel->getMagneticTrackAngle(filterPointsModel->mapToSource(filterPointsModel->index(row, 0))).at(0) << endl;      // Magnetic track angle forward
            out << pointsModel->getMagneticTrackAngle(filterPointsModel->mapToSource(filterPointsModel->index(row, 0))).at(1) << endl;      // Magnetic track angle back
            out << pointsModel->getDistance(filterPointsModel->mapToSource(filterPointsModel->index(row, 0))) << endl;                      // Distance
        }
    }
    if (file.commit())
        QMessageBox::information(this, tr("Export to file"), tr("Data export completed successfully."));
    setCheckedAllRowTable();
}

void MainWindow::filterPoints(const QModelIndex &index)
{
    if (index.data(Qt::CheckStateRole).toBool())
        m_codeAirwayFilter << index.data().toString();
    else
        m_codeAirwayFilter.removeOne(index.data().toString());

    FilterPointsModel *filterPointsModel = qobject_cast<FilterPointsModel*>(ui->pointsTableView->model());
    filterPointsModel->setFilterProperty("codeAirway", m_codeAirwayFilter);
    setCheckedAllRowTable();
}

void MainWindow::setCheckedAllRowTable(bool checked)
{
//    if (sender() != qobject_cast<QGroupHeaderView*>(ui->pointsTableView->horizontalHeader())) {
    qobject_cast<QGroupHeaderView*>(ui->pointsTableView->horizontalHeader())->setChecked(checked);
//    }

//    sortSearchFilterObstracleModel->setDynamicSortFilter(false);
    FilterPointsModel *filterPointsModel = qobject_cast<FilterPointsModel*>(ui->pointsTableView->model());

    if (filterPointsModel->rowCount() == 0)
        return;

    for (int row = 0; row < filterPointsModel->rowCount(); row++)
        m_pointsModel->setData(filterPointsModel->mapToSource(filterPointsModel->index(row, 0)), checked, Qt::CheckStateRole);
}

void MainWindow::setChecked(bool checked, QString codeAirway, QString codePoint)
{
    FilterPointsModel *filterPointsModel = qobject_cast<FilterPointsModel*>(ui->pointsTableView->model());

    for (int row = 0; row < filterPointsModel->rowCount(); row++)
        if (filterPointsModel->index(row, 1).data().toString().contains(codeAirway)
                && filterPointsModel->index(row, 2).data().toString().contains(codePoint)) {
            m_pointsModel->setData(filterPointsModel->mapToSource(filterPointsModel->index(row, 0)), checked, Qt::CheckStateRole);
        }
}

void MainWindow::downloadSourceData()
{
    updateButton->setDisabled(true);

    QSettings settings;
    m_progressDialog = new QProgressDialog(this);
    m_progressDialog->setWindowModality(Qt::WindowModal);
    m_progressDialog->setCancelButtonText(tr("Cancel"));
    m_progressDialog->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    m_progressDialog->setLabelText(tr("Uploading files..."));

    int rows = settings.beginReadArray("sourcesData");
    for (int row = 0; row < rows; ++row) {
        settings.setArrayIndex(row);
        QUrl urlSourceData(settings.value("url").toString());
        if (urlSourceData.isValid()) {
            FileDownloader *downloader = new FileDownloader(urlSourceData, this);
            connect(downloader, SIGNAL(downloaded()), SLOT(saveSourceDataFile()));
            connect(downloader, &FileDownloader::updateDownloadProgress, this, &MainWindow::showProgressDownloadFile);
            connect(m_progressDialog, SIGNAL(canceled()), downloader, SLOT(cancelDownload()));
            connect(m_progressDialog, SIGNAL(canceled()), this, SLOT(enabledUpdateButton()));
//            m_progressDialog->setLabelText(tr("Uploading file:%1").arg(urlSourceData.fileName()));
        }
        break;
    }
    settings.endArray();
}

void MainWindow::saveSourceDataFile()
{
    auto downloader = qobject_cast<FileDownloader*>(sender());

    if (!QDir::current().mkpath("uploaded"))
        return;
    QFile fileSourceData(QDir::currentPath().append("\\").append("uploaded").append("\\").append(downloader->downloadedFileName()));

    if (fileSourceData.open(QIODevice::WriteOnly)) {
        fileSourceData.write(downloader->downloadedData());
        fileSourceData.close();
    }
}

void MainWindow::startParser(const QString &fileName)
{
    QString pyCallFuncStr = QString("parser_airway.parser('%1')").arg(fileName);

    Py_InitializeEx(0);
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./scripts')");
    PyRun_SimpleString("import parser_airway");
    PyRun_SimpleString(pyCallFuncStr.toLocal8Bit().data());
    Py_Finalize();
}

void MainWindow::showProgressDownloadFile(qint64 bytesReceived, qint64 bytesTotal)
{
    if (m_progressDialog) {
        bytesTotalFiles[sender()] = bytesTotal;
        bytesReceivedFiles[sender()] = bytesReceived;
        qint64 bytesTotalAll = 0;
        qint64 bytesReceivedAll = 0;

        for (auto total : bytesTotalFiles.values())
            bytesTotalAll += total;

        for (auto received : bytesReceivedFiles.values())
            bytesReceivedAll += received;

        m_progressDialog->setValue((bytesReceivedAll * 100) / bytesTotalAll);
    }
}

void MainWindow::enabledUpdateButton()
{

}
