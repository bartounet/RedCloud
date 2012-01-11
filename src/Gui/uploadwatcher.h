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
