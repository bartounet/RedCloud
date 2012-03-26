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

#ifndef UPLOADWATCHER_H
#define UPLOADWATCHER_H

#include <QWidget>
#include <QToolBar>
#include <QTreeWidget>
#include "logwidget.h"
#include <QFile>

class QProcess;

class UploadWatcher : public QWidget
{
    Q_OBJECT

	enum e_actions
	{
		MODEL = 0,
		PICTURES,
		DELETE
	};

private:
	bool removeDir(QString path);

	QToolBar mToolBar;
	QTreeWidget mTree;
	QProcess mProcess;
	QStringList mPictureFolders;
	QList<QAction*> mActions;
	LogWidget mLogs;
	QFile mSavedResults;
public:
    explicit UploadWatcher(QWidget *parent = 0);
	~UploadWatcher();
signals:
	void processEnded();
public slots:
	void onNewProcess(QString name, QStringList arguments);
	void onProcessEnded(int index);
	void getPossibleActions(QTreeWidgetItem* current, QTreeWidgetItem* old);
	void deleteResult();
	void showPictures();
	void showModel();
};

#endif // UPLOADWATCHER_H
