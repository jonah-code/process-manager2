#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <QDebug>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sched.h>
#include <QTableWidget>
#include <QTimerEvent>
#include <QTimer>
#include <unistd.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);
    processList = "ps -auf | tail -n +2 | awk '{print $2}'";

    connect(ui->kill,SIGNAL(clicked(bool)),
            this,SLOT(signal_kill()));

    connect(ui->continuei,SIGNAL(clicked(bool)),
            this,SLOT(signal_cont()));

    connect(ui->stop,SIGNAL(clicked(bool)),
            this,SLOT(signal_stop()));

    connect(ui->butomCpu,SIGNAL(clicked(bool)),
            this,SLOT(alocar_cpu()));

    connect(ui->butomPriority,SIGNAL(clicked(bool)),
            this,SLOT(priority()));

    connect(ui->atualizar,SIGNAL(clicked(bool)),
            this,SLOT(atualizar()));




    QStringList colunm;
    colunm<< "Cpu %" << "Pid" << "ppid" << "Prioridade" << " Status" << "name";
    ui->table->setColumnCount(6);
    ui->table->setHorizontalHeaderLabels(colunm);

    listaProcessos = getProcess();
    //qDebug() << listaProcessos;





}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList MainWindow::getProcess()
{
    process.start("sh", QStringList() << "-c" << processList);
    process.waitForFinished(-1);

    QString str = process.readAllStandardOutput();
    QStringList process_list = str.split("\n");


    process_list.removeLast();


    return process_list;
}

QString MainWindow::runComand(QString cmd)
{
    QProcess myProc;
    myProc.start("sh", QStringList() << "-c" << cmd);
    myProc.waitForFinished(-1); // will wait forever until finished
    return myProc.readAllStandardOutput();
}

void MainWindow::showProcess()
{
    foreach(auto process, listaProcessos) {
        QString pid	= runComand("cat /proc/"+process+"/status | grep ^Pid | awk '{print $2}'");
        QString name = runComand("cat /proc/"+process+"/status | grep -i name | awk '{print $2}'");
        QString cpu_prct = runComand("ps -auf | grep "+process+" | awk '{print $3}'");
        QString state = runComand("cat /proc/"+process+"/stat | awk '{print $3}'");
        QString ppid = runComand("cat /proc/"+process+"/status | grep ^PPid | awk '{print $2}'");
        QString prioridade = runComand("cat /proc/"+process+"/stat | awk '{print $18}'");

        name.replace("\n", "");
        cpu_prct.replace("\n", "");
        state.replace("\n", "");
        pid.replace("\n", "");
        ppid.replace("\n", "");
        prioridade.replace("\n", "");

        qDebug() << name;
        qDebug() << cpu_prct;
        qDebug() << pid;
        qDebug() << state;
        qDebug() << ppid;
        qDebug() << prioridade;

        ui->table->insertRow(ui->table->rowCount());
        ui->table->setItem(ui->table->rowCount()-1,0,
                           new QTableWidgetItem(cpu_prct));
        ui->table->setItem(ui->table->rowCount()-1,1,
                           new QTableWidgetItem(pid));
        ui->table->setItem(ui->table->rowCount()-1,2,
                           new QTableWidgetItem(ppid));
        ui->table->setItem(ui->table->rowCount()-1,3,
                           new QTableWidgetItem(prioridade));
        ui->table->setItem(ui->table->rowCount()-1,4,
                           new QTableWidgetItem(state));
        ui->table->setItem(ui->table->rowCount()-1,5,
                           new QTableWidgetItem(name));


    }
}



void MainWindow::signal_kill()
{
    numberPid = ui->lineEdit->text().toInt();
    qDebug() << numberPid;
    kill(numberPid,SIGKILL);
}

void MainWindow::signal_cont()
{
    numberPid = ui->lineEdit->text().toInt();
    qDebug() << numberPid;
    kill(numberPid,SIGCONT);
}

void MainWindow::signal_stop()
{
    numberPid = ui->lineEdit->text().toInt();
    qDebug() << numberPid;
    kill(numberPid,SIGSTOP);
}

void MainWindow::alocar_cpu()
{

    cpu_set_t  mask;  // máscara para habilitação de CPUs
    CPU_ZERO(&mask);
    numberPid = ui->lineEdit->text().toInt();
    numberCpu = ui->getCpu->text().toInt();
    qDebug() << numberCpu;
    qDebug() << numberPid;
    CPU_SET(numberCpu, &mask);    // alocar na CPU 0
    sched_setaffinity(numberPid, sizeof(mask), &mask);

}

void MainWindow::priority()
{
    numberPid = ui->lineEdit->text().toInt();
    numberPriority =  ui->getPriority->text().toInt();
    qDebug() << numberPriority;
    qDebug() << numberPid;
    setpriority(PRIO_PROCESS, numberPid, numberPriority);
}

void MainWindow::atualizar()
{
    processList = "ps -auf | tail -n +2 | awk '{print $2}'";
    getProcess();
    showProcess();
}





// ps -auf | tail -n +2 | awk '{print $2}'

