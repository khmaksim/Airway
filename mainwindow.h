#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
    class MainWindow;
}

class Database;
class QStandardItemModel;
class QToolButton;
class QProgressDialog;
class MapView;
class PointsModel;
class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        QProgressDialog *m_progressDialog;
        QToolBar *toolBar;
        QToolButton *exportButton;
        QToolButton *displayOnMapButton;
        QToolButton *updateButton;
        QToolButton *settingsButton;
        MapView *mapView;

        QStandardItemModel *m_airwaysModel;
        PointsModel *m_pointsModel;
        Database *db;

        void updateModels();
        void readSettings();
        void writeSettings();
        void startParser(const QString &fileName);

    private slots:
        void enabledToolButton();
        void enabledUpdateButton();
        void showAirways();
        void showSettings();
        void exportToFile();
        void filterPoints(const QModelIndex&);
        void setCheckedAllRowTable(bool checked = false);
        void setChecked(bool checked, QString codeAirway, QString codePoint);
        void downloadSourceData();
        void saveSourceDataFile();
        void showProgressDownloadFile(qint64 bytesReceived, qint64 bytesTotal);

    private:
        QMap<QObject*, qint64> bytesTotalFiles;
        QMap<QObject*, qint64> bytesReceivedFiles;
        QStringList m_codeAirwayFilter;
};

#endif // MAINWINDOW_H
