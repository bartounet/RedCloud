#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QObject>
#include <QLabel>
#include <QListWidget>
#include "uploadwidget.h"
#include "uploadwatcher.h"
#include "optionswidget.h"

class MainWindow : public QDialog
{
    Q_OBJECT
private :
	UploadWidget mUpload;
	UploadWatcher mWatcher;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots :


};

#endif // MAINWINDOW_H
