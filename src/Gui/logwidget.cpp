//     This file is part of RedCloud tool.
// 
//     Copyright (C) 2012 Munzer Thibaut, Jonathan Aigrain, Adrien Chey, Nicolas Fabretti
//     Original idea from O.R.A. by Adrien Normier
//     Contacts : ora.ctc@gmail.com (+336 77 09 31 16)
//                redcloud.contact@gmail.com   
// 
//     Redcloud is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     Geoscale is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with Geoscale.  If not, see <http://www.gnu.org/licenses/>.

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
