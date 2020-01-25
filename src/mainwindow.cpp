#include "mainwindow.h"
#include "ui_mainwindow.h"

Q_LOGGING_CATEGORY(CodeCrypto, "CodeCrypto")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    process(new QProcess(this))
{
    ui->setupUi(this);
    setWindowTitle("Cifratura documenti");
    ui->password->setEchoMode(QLineEdit::Password);
    connect(ui->apri,&QPushButton::clicked,this,&MainWindow::openFile);
    connect(ui->actionEsci,&QAction::triggered,qApp,&QApplication::quit);
    connect(ui->actionAbout,&QAction::triggered,this,&MainWindow::about);
}

void MainWindow::openFile()
{
    if(ui->password->text().length() == 0 && ui->password->text().length() < 8)
    {
        QLabel *label = new QLabel("",ui->statusBar);
        QPixmap pixmap(QPixmap(":/images/attenzione.png").scaledToHeight(32));

        label->setPixmap(pixmap);
        label->setToolTip(tr("Attenzione"));

        ui->statusBar->addWidget(label);
        ui->label_word->setText("Digita una password almeno di otto caratteri\nalfanumerici compreso di maiuscole e minuscole.\n");
    }
    else{
        QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Apri documento"),
                                                        "*.*", QObject::tr("Tutti i file(*.*)"));

        if(fileName.isEmpty())
            return;
        else
            encriptFile(fileName,ui->password->text());
    }
}

void MainWindow::encriptFile(QString nomeFile, QString password)
{
    ui->label_word->clear();
    QFileInfo infoFile(nomeFile);
#ifdef Q_OS_LINUX
    process->start("/usr/bin/gpg -z "+ui->compressione->currentText()+" -q --batch --yes --passphrase \""+password+"\" --cipher-algo "+ui->comboBoxCompression->currentText()+" --s2k-digest-algo "+ui->comboBoxProtection->currentText()+" -c "+nomeFile);
#elif defined Q_OS_WIN
    process->start("win/gpg -z "+ui->compressione->currentText()+" --cipher-algo "+ui->comboBoxCompression->currentText()+" --s2k-digest-algo "+ui->comboBoxProtection->currentText()+" -q --batch --yes --passphrase \""+password+"\" -c "+nomeFile);
#endif
    ui->label_word->setText("<p>Documento <b>"+infoFile.fileName()+".gpg</b> salvato correttamente</p>");
}

void MainWindow::about()
{
    QMessageBox::information(this,"CodeCrypto","(C) Copyright 2018 Angelo Scarnà. Tutti i diritti riservati.\n"
                             "Il software usa lo strumento gpg per la cifratura.");
}

MainWindow::~MainWindow()
{
    delete ui;
}
