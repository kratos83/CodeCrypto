#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtCore>

Q_DECLARE_LOGGING_CATEGORY(CodeCrypto)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void encriptFile(QString nomeFile, QString password);
    void openFile();
    void about();
private:
    Ui::MainWindow *ui;
    QProcess *process;
};

#endif // MAINWINDOW_H
