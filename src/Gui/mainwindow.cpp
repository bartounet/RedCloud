#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "uploadwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent)
{
	resize(800, 600);
	connect(&mUpload, SIGNAL(processToAdd(QString,QStringList)),
			&mWatcher, SLOT(onNewProcess(QString, QStringList)));
	connect(&mWatcher, SIGNAL(processEnded()), &mUpload, SLOT(onProcessEnded()));
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(&mUpload);
	layout->addWidget(&mWatcher);

	QVBoxLayout *conflayout = new QVBoxLayout();
	conflayout->addLayout(layout);

	setLayout(conflayout);
}

MainWindow::~MainWindow()
{
}


