#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}




void MainWindow::on_pushButton_clicked()
{
    client_.addToMessage(ui->key->toPlainText(), ui->value->toPlainText());
    QJsonDocument doc(client_.getJson());
    QString strJson(doc.toJson(QJsonDocument::Compact));
    ui->textEdit->setText(strJson);
}



void MainWindow::on_pushButton_2_clicked()
{
    client_.send();
}

void MainWindow::on_pushButton_3_clicked()
{
    QUrl url = QUrl(ui->textEdit_2->toPlainText());
    client_.init(url, true, ROLE_SEND);
}

void MainWindow::on_pushButton_4_clicked()
{
    client_.clear();
     ui->textEdit->setText("");
}
