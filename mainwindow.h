#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class Database;
class QStandardItemModel;
class QToolButton;
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
        QToolBar *toolBar;
        QToolButton *exportButton;
        QToolButton *displayOnMapButton;
        QToolButton *settingsButton;
        MapView *mapView;

        QStandardItemModel *m_airwaysModel;
        PointsModel *m_pointsModel;
        Database *db;

        void updateModels();
        void readSettings();
        void writeSettings();

    private slots:
        void enabledToolButton();
        void showAirways();
        void showSettings();
        void exportToFile();
        void filterPoints(const QModelIndex&);
        void setCheckedAllRowTable(bool checked = false);
};

#endif // MAINWINDOW_H
