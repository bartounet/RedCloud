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

#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "s_options.h"

class OptionsWidget : public QDialog
{
    Q_OBJECT
private:
	QLineEdit mMaxPhotoDimension;
	QLineEdit mBundlerInitPair;
	QLineEdit mCMVSNbClusters;
	QLineEdit mPMVSlevel;
	QLineEdit mPMVScsize;
	QLineEdit mPMVSthreshold;
	QLineEdit mPMVSwsize;
	QLineEdit mPMVSminImageNum;
	QLineEdit mPMVSCPU;
	QLineEdit mCutCoef;
	QLineEdit mPoissonDepth;
	QLineEdit mNumberOfFaces;
	QLineEdit mTextureSize;
	QPushButton mOK;

public:
    explicit OptionsWidget(QWidget *parent = 0);
	s_options getOptions();
signals:

public slots:

};

#endif // OPTIONSWIDGET_H
