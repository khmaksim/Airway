#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit SettingsDialog(QWidget *parent = nullptr);
        ~SettingsDialog();

    private:
        Ui::SettingsDialog *ui;
        QWidget *m_parent;
        void readSettings();

    private slots:
        void writeSettings();
        void showHidePassword(int);
        void showStatusConnect(const QString &error = QString());
        void onTestConnected();
};

#endif // SETTINGSDIALOG_H
