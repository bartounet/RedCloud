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
