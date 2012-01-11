#include "uploadwidget.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QDebug>
#include <QListWidget>
#include <QRadioButton>
#include <cassert>
#include <QProcess>
#include <QMessageBox>
#include "optionswidget.h"

UploadWidget::UploadWidget(QWidget *parent) :
    QWidget(parent)
{
	setMaximumWidth(280);
	mChooseDirectory.setText("Choose Folder");
#ifdef Q_WS_WIN
	mChooseDirectory.setMaximumWidth(100);
#elif Q_WS_MAC
	mChooseDirectory.setMaximumWidth(150);
#endif
	mQualities.addButton(new QRadioButton("Low"), LOW);
	mQualities.addButton(new QRadioButton("Normal"), NORMAL);
	mQualities.addButton(new QRadioButton("High"), HIGH);
	mQualities.addButton(new QRadioButton("Custom"), CUSTOM);

	mCustomParameters.setText("Parameters");
	mCustomParameters.setMaximumWidth(100);
	mSetName.setText("Scene's name :");
	mSetNameEdit.setMaximumHeight(20);
	mDimension.setText("Quality : ");
	mUpload.setText("GO!");
	QFont f = mUpload.font();
	f.setPointSize(18);
	mUpload.setFont(f);

	connect(&mCustomParameters, SIGNAL(clicked()), &mCustomOptions, SLOT(show()));
	connect(&mChooseDirectory, SIGNAL(clicked()), this, SLOT(onChooseDirectoryClick()));
	connect(&mUpload, SIGNAL(clicked()), this, SLOT(onGo()));
	mQualities.button(NORMAL)->click();

	QHBoxLayout *dirlayout = new QHBoxLayout();
	dirlayout->addWidget(&mChooseDirectory);
	dirlayout->addWidget(&mChosenDirectory, 1, Qt::AlignLeft);

	QHBoxLayout *setlayout = new QHBoxLayout();
	setlayout->addWidget(&mSetName);
	setlayout->addWidget(&mSetNameEdit);

	QVBoxLayout *buttongroup = new QVBoxLayout();
	buttongroup->addWidget(&mDimension);
	buttongroup->addWidget(mQualities.button(LOW));
	buttongroup->addWidget(mQualities.button(NORMAL));
	buttongroup->addWidget(mQualities.button(HIGH));
	buttongroup->addWidget(mQualities.button(CUSTOM));

	QHBoxLayout *dimlayout = new QHBoxLayout();
	dimlayout->addLayout(buttongroup);
	dimlayout->addWidget(&mCustomParameters, 1, Qt::AlignBottom);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addLayout(dirlayout);
	layout->addWidget(&mFiles);
	layout->addLayout(setlayout);
	layout->addLayout(dimlayout);
	layout->addWidget(&mUpload, 1, Qt::AlignHCenter);

	setLayout(layout);
}


void UploadWidget::onChooseDirectoryClick()
{
	QFileDialog fd;
	fd.setFileMode(QFileDialog::Directory);
	if (fd.exec() == QDialog::Accepted)
	{
		mFiles.clear();
		assert(fd.selectedFiles().count() == 1);
		QString path = fd.selectedFiles().at(0);
		QDir chosenDir(path);
		QStringList files = chosenDir.entryList();
		files.removeAll(".");
		files.removeAll("..");
		foreach (QString file, files)
			mFiles.addItem(file.mid(file.lastIndexOf('/') + 1));
		mChosenDirectory.setText(path);
	}
}

void UploadWidget::onGo()
{
	// Check whether argument have been set
	if (!mChosenDirectory.text().length() || !mSetNameEdit.text().length())
	{
		QMessageBox err(QMessageBox::Warning, "Missing fields",
					"Please choose both a pictures directory and a scene's name.");
		err.exec();
		return;
	}

	QStringList arguments;
	QString curPath = QDir::currentPath();
	arguments << mChosenDirectory.text() << curPath + "/results/" + mSetNameEdit.text();
	mUpload.setEnabled(false);
	s_options opt = getOptions();
	createOptionsFile(opt);
	emit processToAdd(mSetNameEdit.text(), arguments);
}

s_options UploadWidget::getOptions()
{
	switch (mQualities.id(mQualities.checkedButton()))
	{
	case LOW :
		return s_options::getOptionsLow();
	case NORMAL :
		return s_options::getOptionsMedium();
	case HIGH :
		return s_options::getOptionsHigh();
	case CUSTOM :
		return mCustomOptions.getOptions();
	}
	return s_options();
}


void UploadWidget::createOptionsFile(s_options& opt)
{
	QFile option(QDir::currentPath() + "/option.txt");
	option.open(QFile::WriteOnly);
	option.write(QString("maxPhotoDimension = %1\n").arg(opt.MaxPhotoDimension).toAscii());
	option.write(QString("BundlerInitPair = %1\n").arg(opt.BundlerInitPair).toAscii());
	option.write(QString("CMVSNbClusters = %1\n").arg(opt.CMVSNbClusters).toAscii());
	option.write(QString("PMVSlevel = %1\n").arg(opt.PMVSlevel).toAscii());
	option.write(QString("PMVScsize = %1\n").arg(opt.PMVScsize).toAscii());
	option.write(QString("PMVSthreshold = %1\n").arg(opt.PMVSthreshold).toAscii());
	option.write(QString("PMVSwsize = %1\n").arg(opt.PMVSwsize).toAscii());
	option.write(QString("PMVSminImageNum = %1\n").arg(opt.PMVSminImageNum).toAscii());
	option.write(QString("PMVSCPU = %1\n").arg(opt.PMVSCPU).toAscii());
	option.write(QString("cutCoef = %1\n").arg(opt.CutCoef).toAscii());
	option.write(QString("poissonDepth = %1\n").arg(opt.PoissonDepth).toAscii());
	option.write(QString("numberOfFaces = %1\n").arg(opt.NumberOfFaces).toAscii());
	option.write(QString("textureSize  = %1\n").arg(opt.TextureSize ).toAscii());
	option.close();
}
