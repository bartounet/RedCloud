#include "logwidget.h"
#include <QVBoxLayout>
#include <QDebug>

LogWidget::LogWidget(QWidget *parent) :
	QDialog(parent)
{	
	mCurrentProcess = 0;
	mError.setReadOnly(true);
	mOutput.setReadOnly(true);
	mTabs.addTab(&mOutput, "Output");
	mTabs.addTab(&mError, "Error");

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(&mTabs);
	layout->setMargin(0);
	layout->setSpacing(0);
	setLayout(layout);
	resize(400, 400);
}

void LogWidget::setCurrentProcess(QProcess *p)
{
	if (p)
	{
		QString error(p->readAllStandardError());
		qDebug() << error;
		mError.setText(error);
		mOutput.setText(QString(p->readAllStandardOutput()));
		if (p->state() != QProcess::NotRunning)
		{
			connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(refreshLogs()));
			connect(p, SIGNAL(finished(int)), this, SLOT(stopRefreshing()));
//			connect(&mRefresh, SIGNAL(timeout()), this, SLOT(refreshLogs()));
		}
	}

	mCurrentProcess = p;
	//mRefresh.start(500);
}

void LogWidget::refreshLogs()
{
	if (mCurrentProcess)
	{
		mCurrentProcess->setReadChannel(QProcess::StandardOutput);
		QString output;
		while (mCurrentProcess->canReadLine())
			output += mCurrentProcess->readLine();
		qDebug() << "Refresh";
		qDebug() << output;
		mOutput.setText(mOutput.toPlainText() + output);
		mOutput.repaint();
	}
}

void LogWidget::stopRefreshing()
{
	if (mCurrentProcess)
	{
		QString error(mCurrentProcess->readAllStandardError());
		QString output(mCurrentProcess->readAllStandardOutput());
		mError.setText(mError.toPlainText() + error);
		mOutput.setText(mOutput.toPlainText() + output);
	}
	mRefresh.stop();
}
