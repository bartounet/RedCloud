#include "logwidget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QTextCursor>

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
		mError.setText(error);
		mOutput.setText(QString(p->readAllStandardOutput()));
		if (p->state() != QProcess::NotRunning)
		{
			connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(refreshStandardLogs()));
			connect(p, SIGNAL(readyReadStandardError()), this, SLOT(refreshErrorLogs()));
			connect(p, SIGNAL(finished(int)), this, SLOT(stopRefreshing()));
		}
	}

	mCurrentProcess = p;
	//mRefresh.start(500);
}

void LogWidget::refreshStandardLogs()
{
	if (mCurrentProcess)
	{
		mCurrentProcess->setReadChannel(QProcess::StandardOutput);
		QString output;
		while (mCurrentProcess->canReadLine())
			output += mCurrentProcess->readLine();
		mOutput.setText(mOutput.toPlainText() + output);
		QTextCursor curs(mOutput.textCursor());
		curs.movePosition(QTextCursor::End);
		mOutput.setTextCursor(curs);
		mOutput.repaint();

	}
}

void LogWidget::refreshErrorLogs()
{
	if (mCurrentProcess)
	{
		mCurrentProcess->setReadChannel(QProcess::StandardError);
		QString error;
		while (mCurrentProcess->canReadLine())
			error += mCurrentProcess->readLine();
		mError.setText(mError.toPlainText() + error);
		QTextCursor curs(mOutput.textCursor());
		curs.movePosition(QTextCursor::End);
		mError.setTextCursor(curs);
		mError.repaint();

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
}
