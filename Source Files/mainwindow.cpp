#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <filesystem>

int assembleFile(Ui::MainWindow *ui,std::string ip,std::string op);
std::string filepath;
std::string opfilepath;

QString GetNearestExistingAncestorOfPath(const QString & path)
{
    if(QFileInfo::exists(path)) return path;

    QDir dir(path);
    if(!dir.makeAbsolute()) return {};
    do
    {
        dir.setPath(QDir::cleanPath(dir.filePath(QStringLiteral(".."))));
    }
    while(!dir.exists() && !dir.isRoot());

    return dir.exists() ? dir.path() : QString{};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("SIC-XE Assembler");
    QString sPath = "/Users/";
    dirModel = new  QFileSystemModel(this);
    dirModel -> setRootPath(sPath);
    ui->treeView->setModel(dirModel);
    ui->treeView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionOpen_triggered()
{

}


void MainWindow::on_asml_btn_clicked()
{
       std::string ip = filepath;
       std::string op = opfilepath + "/output.txt";
       assembleFile(ui,ip,op);

}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString myPath = dirModel->filePath(index);
    filepath = myPath.toStdString();
    int pos = myPath.lastIndexOf(QChar('/'));
    opfilepath =myPath.left(pos).toStdString();
}


void MainWindow::on_close_btn_clicked()
{
    this->close();
}


void MainWindow::on_save_btn_clicked()
{
    std::string status = "Saved!" ;
    status+="\nOutput file: " + opfilepath + "/output.txt";
    ui->status_tb->setText(status.c_str());
}


void MainWindow::on_open_btn_clicked()
{
     if(filepath.length()<=0){
         ui->status_tb->setText("Please select file");
         return;
     }
     std::string status = "Selected file" + filepath + " Click Assemble to start";
     ui->status_tb->setText(status.c_str());
}

