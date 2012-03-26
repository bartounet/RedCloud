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

#include "optionswidget.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QDoubleValidator>

OptionsWidget::OptionsWidget(QWidget *parent) :
	QDialog(parent)
{
	mOK.setText("OK");
	connect(&mOK, SIGNAL(clicked()), this, SLOT(close()));

	s_options opt = s_options::getOptionsMedium();
	mMaxPhotoDimension.setText(QString::number(opt.MaxPhotoDimension));
	mBundlerInitPair.setText(QString::number(opt.BundlerInitPair));
	mCMVSNbClusters.setText(QString::number(opt.CMVSNbClusters));
	mPMVSlevel.setText(QString::number(opt.PMVSlevel));
	mPMVScsize.setText(QString::number(opt.PMVScsize));
	mPMVSthreshold.setText(QString::number(opt.PMVSthreshold));
	mPMVSwsize.setText(QString::number(opt.PMVSwsize));
	mPMVSminImageNum.setText(QString::number(opt.PMVSminImageNum));
	mPMVSCPU.setText(QString::number(opt.PMVSCPU));
	mCutCoef.setText(QString::number(opt.CutCoef));
	mPoissonDepth.setText(QString::number(opt.PoissonDepth));
	mNumberOfFaces.setText(QString::number(opt.NumberOfFaces));
	mTextureSize.setText(QString::number(opt.TextureSize));

	mMaxPhotoDimension.setValidator(new QIntValidator());
	mBundlerInitPair.setValidator(new QIntValidator());
	mCMVSNbClusters.setValidator(new QIntValidator());
	mPMVSlevel.setValidator(new QIntValidator());
	mPMVScsize.setValidator(new QIntValidator());
	mPMVSthreshold.setValidator(new QDoubleValidator());
	mPMVSwsize.setValidator(new QIntValidator());
	mPMVSminImageNum.setValidator(new QIntValidator());
	mPMVSCPU.setValidator(new QIntValidator());
	mCutCoef.setValidator(new QDoubleValidator());
	mPoissonDepth.setValidator(new QIntValidator());
	mNumberOfFaces.setValidator(new QIntValidator());
	mTextureSize.setValidator(new QIntValidator());


	QFormLayout *flayout = new QFormLayout();
	flayout->addRow("MaxPhotoDimension", &mMaxPhotoDimension);
	flayout->addRow("BundlerInitPair", &mBundlerInitPair);
	flayout->addRow("CMVSNbClusters", &mCMVSNbClusters);
	flayout->addRow("PMVSlevel", &mPMVSlevel);
	flayout->addRow("PMVScsize", &mPMVScsize);
	flayout->addRow("PMVSthreshold", &mPMVSthreshold);
	flayout->addRow("PMVSwsize", &mPMVSwsize);
	flayout->addRow("PMVSminImageNum", &mPMVSminImageNum);
	flayout->addRow("PMVSCPU", &mPMVSCPU);
	flayout->addRow("CutCoef", &mCutCoef);
	flayout->addRow("PoissonDepth", &mPoissonDepth);
	flayout->addRow("NumberOfFaces", &mNumberOfFaces);
	flayout->addRow("TextureSize", &mTextureSize);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addLayout(flayout);
	layout->addWidget(&mOK, 1, Qt::AlignHCenter);

	setLayout(layout);
}


s_options OptionsWidget::getOptions()
{
	s_options res;

	res.MaxPhotoDimension = mMaxPhotoDimension.text().toInt();
	res.BundlerInitPair = mBundlerInitPair.text().toInt();
	res.CMVSNbClusters = mCMVSNbClusters.text().toInt();
	res.PMVSlevel = mPMVSlevel.text().toInt();
	res.PMVScsize = mPMVScsize.text().toInt();
	res.PMVSthreshold= mPMVSthreshold.text().toFloat();
	res.PMVSwsize = mPMVSwsize.text().toInt();
	res.PMVSminImageNum = mPMVSminImageNum.text().toInt();
	res.PMVSCPU = mPMVSCPU.text().toInt();
	res.CutCoef = mCutCoef.text().toFloat();
	res.PoissonDepth = mPoissonDepth.text().toInt();
	res.NumberOfFaces = mNumberOfFaces.text().toInt();
	res.TextureSize = mTextureSize.text().toInt();

	return res;
}
