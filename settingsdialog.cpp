#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(ui->selectFileButton, SIGNAL(clicked()), this, SLOT(selectFileDatabase()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(writeSettings()));

    readSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::selectFileDatabase()
{
    QString fileDatabase = QFileDialog::getOpenFileName(this, tr("Select file database"), QDir::homePath(),
                                                        tr("All database SQLite (*.db *.sdb. *sqlite *.db3 *.s3db *sqlite3 *.sl3 *.db2 *.s2db *.sqlite2 *.sl2)"));

    if (!fileDatabase.isEmpty()) {
        ui->fileDatabaseLineEdit->setText(QDir::toNativeSeparators(fileDatabase));
    }
}

void SettingsDialog::writeSettings()
{
    QSettings settings;

    settings.beginGroup("connectDatabase");
    settings.setValue("file", ui->fileDatabaseLineEdit->text().simplified());
    settings.endGroup();

    this->accept();
}

void SettingsDialog::readSettings()
{
    QSettings settings;

    settings.beginGroup("connectDatabase");
    ui->fileDatabaseLineEdit->setText(settings.value("file").toString());
    settings.endGroup();
}
