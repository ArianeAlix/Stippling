#ifndef _WINDOW_
#define _WINDOW_

// std
#include <list>
#include <QWidget>

// Qt
#include <QString>
#include "scene.h"
#include "ui_stippling.h"

class MainWindow : public QMainWindow, public Ui_MainWindow
{
	Q_OBJECT

private:
	Scene* m_scene;
	QImage entree;
	QImage sortie;
	QWidget *wdg = new QWidget;
	QGridLayout *gridLayoutPic= new QGridLayout(wdg);
	QLabel *entreeLabel = new QLabel();
	QLabel *sortieLabel = new QLabel();


	unsigned int maxNumRecentFiles;
	QAction* recentFilesSeparator;
	QVector<QAction*> recentFileActs;

public:
	MainWindow();
	~MainWindow();

	void update();

protected:

	protected slots:

	// drag & drop
	void closeEvent(QCloseEvent *event);

public slots:

	    // Data
		void on_actionClear_triggered();
		void on_actionRandom_triggered();
		void on_actionLoad_triggered();
		void on_actionLinear_triggered();
		void open(const QString& filename);
		void clear();
};

#endif // _WINDOW_
