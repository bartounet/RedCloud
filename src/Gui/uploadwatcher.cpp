#include "uploadwatcher.h"
#include <QVBoxLayout>
#include <QProcess>
#include <cassert>
#include <QDebug>
#include <QAction>
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

UploadWatcher::UploadWatcher(QWidget *parent) :
	QWidget(parent), mSavedResults (QDir::currentPath() + "/results/list.txt")
{
	mActions.append(new QAction("Open Model Directory", this));
	mActions.append(new QAction("Show Pictures", this));
	mActions.append(new QAction("Delete", this));

	connect(mActions[int(DELETE)], SIGNAL(triggered()), this, SLOT(deleteResult()));
	connect(mActions[int(PICTURES)], SIGNAL(triggered()), this, SLOT(showPictures()));
	connect(mActions[int(MODEL)], SIGNAL(triggered()), this, SLOT(showModel()));

	connect(&mProcess, SIGNAL(finished(int)), this, SLOT(onProcessEnded(int)));

	mTree.setColumnCount(2);
	QStringList labels;
	labels << "Scene's Name" << "State"; // << "Remaining Time";
	mTree.setHeaderLabels(labels);
	connect(&mTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
			this, SLOT(getPossibleActions(QTreeWidgetItem*,QTreeWidgetItem*)));

	mSavedResults.open(QFile::ReadOnly);
	while (!mSavedResults.atEnd())
	{
		QString line = mSavedResults.readLine();
		QStringList fields = line.split(' ');
		if (fields.count() == 3)
		{
			fields[2].chop(1);
			QString folder = fields.back();
			fields.pop_back();
			mTree.insertTopLevelItem(0,new QTreeWidgetItem(fields));
			mPictureFolders.push_front(folder);
		}
	}
	mSavedResults.close();

	QVBoxLayout *layout = new QVBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(&mToolBar);
	layout->addWidget(&mTree);
	layout->addWidget(&mLogs);
	setLayout(layout);
}

UploadWatcher::~UploadWatcher()
{
	if (mProcess.state() == QProcess::Running)
		mProcess.kill();
}

void UploadWatcher::onNewProcess(QString name, QStringList arguments)
{
	for (int curItem = 0; curItem < mTree.topLevelItemCount(); ++curItem)
		if (mTree.topLevelItem(curItem)->text(0) == name)
		{
			QMessageBox fail(QMessageBox::Critical, "Can not create result directory",
							"A  previous result has already the same name. "
							"Please delete the previous result or change the name.");
			fail.exec();
			emit processEnded();
			return;
		}
	mProcess.start(QDir::currentPath() + "/redcloud.exe", arguments);
	mLogs.setCurrentProcess(&mProcess);
	mPictureFolders.push_front(arguments.front());
	QStringList fields;
	fields << name << "Computing";
	QTreeWidgetItem *item = new QTreeWidgetItem(fields);
	mTree.insertTopLevelItem(0,item);
}

void UploadWatcher::onProcessEnded(int status)
{
	QProcess* process = (QProcess*) sender();
	assert(process);

	QTreeWidgetItem *curItem = mTree.topLevelItem(0);
	curItem->setText(1, "Terminated");
	if (mTree.indexOfTopLevelItem(mTree.currentItem()) == 0)
		getPossibleActions(mTree.currentItem(), 0);
	qDebug() << "Process finished with code :" << status;
	mSavedResults.open(QFile::Append);
	for (int curCol = 0; curCol < mTree.columnCount(); curCol++)
	{
		mSavedResults.write(curItem->text(curCol).toAscii());
		mSavedResults.write(" ");
	}
	mSavedResults.write(mPictureFolders.front().toAscii());
	mSavedResults.write("\n");
	mSavedResults.close();
	emit processEnded();
}

void UploadWatcher::getPossibleActions(QTreeWidgetItem* current, QTreeWidgetItem* old)
{
	if (!current)
		return;
	mToolBar.clear();
	int index = mTree.indexOfTopLevelItem(current);
	if (index != 0 || mProcess.state() == QProcess::NotRunning)
		mToolBar.addAction(mActions.at(int(MODEL)));
	mToolBar.addAction(mActions.at(int(PICTURES)));
	mToolBar.addAction(mActions.at(int(DELETE)));
}

bool UploadWatcher::removeDir(QString dirPath)
{
	QDir folder(dirPath);
	if (!folder.exists())
		return false;
	folder.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
	foreach(QFileInfo fileInfo, folder.entryInfoList())
	{
		if(fileInfo.isDir())
		{
			if(!removeDir(fileInfo.filePath()))
				return false;
		}
		else if(fileInfo.isFile())
		{
			if(!QFile::remove(fileInfo.filePath()))
			{
				qDebug() << "Unable to remove file : " << fileInfo.filePath();
				return false;
			}
		}
	}

	if(!folder.rmdir(dirPath))
	{
		qDebug() << "Unable to remove folder : " << dirPath << ". Maybe this folder is not empty";
		return false;
	}

	return true;
}

void UploadWatcher::deleteResult()
{
	if (!mTree.currentItem())
		return;
	QMessageBox confirmation(QMessageBox::Warning, "Delete/Cancel",
							 "Are you sure you want to delete this results?",
							 QMessageBox::Yes | QMessageBox::No);
	int code = confirmation.exec();
	if (code == QMessageBox::No)
		return;
	int index = mTree.indexOfTopLevelItem(mTree.currentItem());
	if (index == 0 && mProcess.state() == QProcess::Running)
		mProcess.kill();

	QTreeWidgetItem *curr = mTree.takeTopLevelItem(index);
	QString pathToRemove = QDir::currentPath() + "/results/" + curr->text(0);
	if (!removeDir(pathToRemove) && QDir(pathToRemove).exists())
	{
		QMessageBox fail(QMessageBox::Critical, "Deletion failed",
								 "Unable to delete the directory, please do it manually.");
		fail.exec();
	}
	mSavedResults.open(QFile::WriteOnly);
	for (int currIt = 0; currIt < mTree.topLevelItemCount(); currIt++)
	{
		QTreeWidgetItem* curItem = mTree.topLevelItem(currIt);
		for (int curCol = 0; curCol < mTree.columnCount(); curCol++)
		{
			mSavedResults.write(curItem->text(curCol).toAscii());
			mSavedResults.write(" ");
		}
		mSavedResults.write(mPictureFolders.front().toAscii());
		mSavedResults.write("\n");
	}
	mSavedResults.close();
}

void UploadWatcher::showPictures()
{
	if (!mTree.currentItem())
		return;

	int index = mTree.indexOfTopLevelItem(mTree.currentItem());
	if (!QDir(mPictureFolders[index]).exists())
	{
		QMessageBox fail(QMessageBox::Critical, "Directory does not exist",
								 "The directory that contained the pictures does not exist anymore.");
		fail.exec();
		return;
	}
	QDesktopServices::openUrl(QUrl(QString("file:///") + mPictureFolders[index], QUrl::TolerantMode));
}

void UploadWatcher::showModel()
{
	if (!mTree.currentItem())
		return;

	QString resultfolder = QDir::currentPath() + "/results/" + mTree.currentItem()->text(0) + "/RedCloud";
	if (!QDir(resultfolder).exists())
	{
		QMessageBox fail(QMessageBox::Critical, "Directory does not exist",
								 "The directory that contained the models does not exist anymore.");
		fail.exec();
		return;
	}

	QDesktopServices::openUrl(QUrl(QString("file:///") + resultfolder, QUrl::TolerantMode));
}
