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

	void createOptionsFile(s_options& opt);

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
