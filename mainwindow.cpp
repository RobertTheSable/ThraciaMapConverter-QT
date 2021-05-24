/*
 *  Thracia 776 Map Converter
 *
 *  Copyright (C) 2015 Robert the Sable
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3
 *  as published by the Free Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  <Description> For those of you not familiar with the GNU GPL system,
 *  the license is in the file named "COPYING".
 */ 

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "TMXReader/tmxreader.h"
#include "ThraciaMap/thraciamap.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDir>

//#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), "Tiled Files (*.tmx);;All Files (*.*)");
    if(filename != "")
    {
        QFileInfo fileInfo(filename);
        ui->lineEdit_3->setText(filename);
        ui->lineEdit->setText(fileInfo.absoluteDir().absoluteFilePath("") + '/' + fileInfo.completeBaseName() + ".map");
        ui->lineEdit_2->setText(fileInfo.absoluteDir().absoluteFilePath("") + '/' + fileInfo.completeBaseName() + ".changes");
        ui->pushButton->setEnabled(true);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString test = ui->lineEdit_3->text();
    std::string infile = test.toStdString();
    TMXReader reader(infile.c_str());
    int x = reader.getX();
    int y = reader.getY();

    TMX_err_type error = reader.parseTMX();
    QString errormsg("Error(s) while parsing TMX:");
    if(error == NO_ERROR)
    {
        unsigned int *mapData = reader.getData();

        ThraciaMap snesmap(x, y, mapData);
        if(reader.hasChanges())
        {
            for(int i = 0; i < reader.getNumberOfChanges(); i++)
            {
                MapChange m = reader.getChange(i);
                int *changeData;
                m.getData(&changeData);
                snesmap.addChange(i, m.getXPos(), m.getYPos(), m.getXSize(), m.getYSize(), m.bitSet(), changeData);
            }
            snesmap.writeChangesToFile(ui->lineEdit_2->text().toLatin1().data());
        }
        snesmap.writeToFile(ui->lineEdit->text().toLatin1().data());

        //delete mapData;
        QMessageBox::information(this, "Conversion Finished", "Conversion completed succesfully");
    }
    else
    {
        if(error & ERR_NO_MAIN)
        {
            errormsg += "\n No Main layer detected.";
        }
        else
        {
            if(error & ERR_CHANGE_X)
            {
                errormsg += "\nNo X property detected in one or more changes";
            }
            if(error & ERR_CHANGE_Y)
            {
                errormsg += "\nNo Y property detected in one or more changes";
            }
            if(error & ERR_CHANGE_WIDTH)
            {
                errormsg += "\nNo Width property detected in one or more changes";
            }
            if(error & ERR_CHANGE_HEIGHT)
            {
                errormsg += "\nNo Height property detected in one or more changes";
            }
            if(error & ERR_CHANGE_ID)
            {
                errormsg += "\nNo ID property detected in one or more changes";
            }
        }
        QMessageBox::warning(this, "Error while parsing TMX!", errormsg);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Thracia 776 TMX Converter", "Thracia 776 TMX Converter\nBy Robert the Sable.\nVersion 1.0");
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::exit();
}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    ui->pushButton->setEnabled(!arg1.isEmpty() && !ui->lineEdit->text().isEmpty() && !ui->lineEdit_2->text().isEmpty());
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    ui->pushButton->setEnabled(!arg1.isEmpty() && !ui->lineEdit_3->text().isEmpty() && !ui->lineEdit_2->text().isEmpty());

}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    ui->pushButton->setEnabled(!arg1.isEmpty() && !ui->lineEdit->text().isEmpty() && !ui->lineEdit_3->text().isEmpty());

}
