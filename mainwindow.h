#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QProcess>
#include <QList>
#include <QProgressBar>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStringList getProcess();
    void fillTable();
    QString runComand(QString cmd);
    void showProcess();

public slots:
    void signal_kill();
    void signal_cont();
    void signal_stop();
    void alocar_cpu();
    void priority();
    void atualizar();

private:
    Ui::MainWindow *ui;
    QString processList;
    QStringList listaProcessos;
    int numberPid;
    int numberCpu;
    int numberPriority;
    QProcess process;
    enum Colunas{
        CPU,NOME
    };
};
#endif // MAINWINDOW_H
