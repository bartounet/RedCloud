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

#ifndef UPLOADWIDGET_H
#define UPLOADWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QObject>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QButtonGroup>
#include <QProcess>
#include <QCheckBox>
#include "s_options.h"
#include "optionswidget.h"

class UploadWidget : public QWidget
{
    Q_OBJECT

	enum e_quality
	{
		LOW,
		NORMAL,
		HIGH,
		CUSTOM
	};


private :
	s_options getOptions();

	void createOptionsFile(s_options& opt, bool noGPS);

	QPushButton	mChooseDirectory;
	QLabel mChosenDirectory;
	QListWidget mFiles;
	QLabel mSetName;
	QLineEdit mSetNameEdit;
	QLabel mDimension;
	QButtonGroup mQualities;
	QPushButton mCustomParameters;
	QPushButton mUpload;
	OptionsWidget mCustomOptions;
	QCheckBox mWithGPS;

public:
	explicit UploadWidget(QWidget *parent = 0);

public slots :
	void onChooseDirectoryClick();
	void onGo();
	void onProcessEnded() {mUpload.setEnabled(true);}
signals :
	void processToAdd(QString, QStringList);
};

#endif // UPLOADWIDGET_H
