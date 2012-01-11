#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QTabWidget>
#include <QTextEdit>
#include <QProcess>
#include <QTimer>

class LogWidget : public QDialog
{
    Q_OBJECT
public:
    explicit LogWidget(QWidget *parent = 0);
	void setCurrentProcess(QProcess* p);
private:
	QTabWidget mTabs;
	QTextEdit mError;
	QTextEdit mOutput;
	QProcess* mCurrentProcess;
	QTimer mRefresh;
signals:

public slots:
	void refreshLogs();
	void stopRefreshing();
};

#endif // LOGWIDGET_H
