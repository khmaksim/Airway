#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>
#include <QSettings>
#include <QValidator>
#include <QScreen>
#include <QDebug>
#include "databaseaccess.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_parent(parent)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->portServerDatabaseLineEdit->setValidator(new QIntValidator());
    ui->statusConnectLabel->setWordWrap(true);

    connect(ui->saveButton, &QPushButton::clicked, this, &SettingsDialog::writeSettings);
    connect(ui->saveButton, &QPushButton::clicked, this, &SettingsDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &SettingsDialog::reject);
    connect(ui->showPasswordCheckBox, &QCheckBox::stateChanged, this, &SettingsDialog::showHidePassword);
    connect(ui->testConnectedButton, SIGNAL(clicked()), this, SLOT(onTestConnected()));

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

void SettingsDialog::showHidePassword(int state)
{
    ui->passwordLineEdit->setEchoMode(state == Qt::Unchecked ? QLineEdit::Password : QLineEdit::Normal);
}

void SettingsDialog::writeSettings()
{
    QSettings settings;

    settings.beginGroup("connectDatabase");
    settings.setValue("host", ui->hostServerDatabaseLineEdit->text().simplified());
    settings.setValue("port", ui->portServerDatabaseLineEdit->text().simplified());
    settings.setValue("nameDatabase", ui->nameDatabaseLineEdit->text().simplified());
    settings.setValue("user", ui->userLineEdit->text().simplified());
    settings.setValue("password", ui->passwordLineEdit->text().simplified());

    QString host = settings.value("host", "localhost").toString();
    int port = settings.value("port", "5432").toInt();
    QString nameDatabase = settings.value("nameDatabase").toString();
    QString user = settings.value("user", "postgres").toString();
    QString password = settings.value("password").toString();
    DatabaseAccess::getInstance()->initConnectDatabase(host, port, nameDatabase, user, password);
    settings.endGroup();
}

void SettingsDialog::readSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.beginGroup("settingsDialog");
    if (settings.contains("geometry"))
        this->setGeometry(settings.value("geometry").toRect());
    else {
        // default size
        QRect rect = frameGeometry();
        if (m_parent == nullptr)
            rect.moveCenter(QGuiApplication::screens().at(0)->geometry().center());
        else
            rect.moveCenter(m_parent->geometry().center());
        this->setGeometry(rect.topLeft().x(), rect.topLeft().y(), 500, 400);
    }
    settings.endGroup();
    settings.endGroup();
    settings.beginGroup("connectDatabase");
    ui->hostServerDatabaseLineEdit->setText(settings.value("host", "localhost").toString());
    ui->portServerDatabaseLineEdit->setText(settings.value("port", "5432").toString());
    ui->nameDatabaseLineEdit->setText(settings.value("nameDatabase").toString());
    ui->userLineEdit->setText(settings.value("user", "postgres").toString());
    ui->passwordLineEdit->setText(settings.value("password").toString());
    settings.endGroup();
}

void SettingsDialog::showStatusConnect(const QString &error)
{
    if (error.isEmpty())
        ui->statusConnectLabel->setText(tr("<html><head/><body><p><span style=\" color:#00aa00;\">Successfully connected</span></p></body></html>"));
    else
        ui->statusConnectLabel->setText(QString("<html><head/><body><p><span style=\" color:#f51616;\">%1</span></p></body></html>").arg(error));
}

void SettingsDialog::onTestConnected()
{
    QMap<QString, QVariant> configConnectDatabase;
    configConnectDatabase.insert("host", ui->hostServerDatabaseLineEdit->text().simplified());
    configConnectDatabase.insert("port", ui->portServerDatabaseLineEdit->text().simplified());
    configConnectDatabase.insert("nameDatabase", ui->nameDatabaseLineEdit->text().simplified());
    configConnectDatabase.insert("user", ui->userLineEdit->text().simplified());
    configConnectDatabase.insert("password", ui->passwordLineEdit->text().simplified());

    if (DatabaseAccess::getInstance()->connect(configConnectDatabase))
        showStatusConnect();
}
