#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>
#include <QSettings>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(ui->selectFileButton, SIGNAL(clicked()), this, SLOT(selectFileDatabase()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(writeSettings()));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addNewSourceDataToList()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeSourceDataFromList()));
    connect(ui->sourceDataUrlListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(enableRemoveButton()));

    readSettings();
}

SettingsDialog::~SettingsDialog()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.beginGroup("settingsDialog");
    settings.setValue("geometry", this->geometry());
    settings.endGroup();
    settings.endGroup();

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

    settings.beginGroup("database");
    settings.setValue("file", ui->fileDatabaseLineEdit->text().simplified());
    settings.endGroup();
    settings.beginWriteArray("sourcesData");
    for (int row = 0; row < ui->sourceDataUrlListWidget->count(); row++) {
        settings.setArrayIndex(row);
        settings.setValue("url", ui->sourceDataUrlListWidget->item(row)->text());
    }
    settings.endArray();

    this->accept();
}

void SettingsDialog::readSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.beginGroup("settingsDialog");
    if (settings.contains("geometry"))
        this->setGeometry(settings.value("geometry").toRect());
    settings.endGroup();
    settings.endGroup();
    settings.beginGroup("database");
    ui->fileDatabaseLineEdit->setText(settings.value("file").toString());
    settings.endGroup();
    int rows = settings.beginReadArray("sourcesData");
    for (int row = 0; row < rows; ++row) {
        settings.setArrayIndex(row);
        QListWidgetItem *item = new QListWidgetItem(settings.value("url").toString(), ui->sourceDataUrlListWidget);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
        ui->sourceDataUrlListWidget->addItem(item);
    }
     settings.endArray();
}

void SettingsDialog::addNewSourceDataToList()
{
    QListWidgetItem *item = new QListWidgetItem(tr("Enter URL source data..."), ui->sourceDataUrlListWidget);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    ui->sourceDataUrlListWidget->addItem(item);
}

void SettingsDialog::removeSourceDataFromList()
{
    QList<QListWidgetItem*> items = ui->sourceDataUrlListWidget->selectedItems();
    foreach(QListWidgetItem *item, items)
        delete ui->sourceDataUrlListWidget->takeItem(ui->sourceDataUrlListWidget->row(item));
}

void SettingsDialog::enableRemoveButton()
{
    ui->removeButton->setEnabled(ui->sourceDataUrlListWidget->selectedItems().size() > 0);
}
