#ifndef _WINDOW_
#define _WINDOW_

// std
#include <list>
#include <QWidget>

// Qt
#include <QString>
#include "scene.h"
#include "ui_stippling.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h> //déja dans dt.h
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>
#include <CGAL/basic.h> //déja dans dt.h
#include <CGAL/Cartesian_converter.h>
#include <CGAL/Triangulation_data_structure_2.h>

// typedefs for defining the adaptor
typedef CGAL::Exact_predicates_inexact_constructions_kernel                  K;
typedef CGAL::Delaunay_triangulation_2<K>                                    DT;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<DT>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<DT> AP;
typedef CGAL::Voronoi_diagram_2<DT, AT, AP>                                  VD;
typedef CGAL::Voronoi_diagram_2< DT, AT, AP >::Face_iterator  Face_iterator;
typedef CGAL::Voronoi_diagram_2< DT, AT, AP >::Site_iterator  Site_iterator;
typedef CGAL::Voronoi_diagram_2< DT, AT, AP >::Vertex_iterator  Vertex_iterator;

// typedef for the result type of the point location
typedef AT::Site_2                    Site_2;
typedef AT::Point_2                   Point_2;
typedef VD::Locate_result             Locate_result;
typedef VD::Vertex_handle             Vertex_handle;
typedef VD::Face_handle               Face_handle;
typedef VD::Halfedge_handle           Halfedge_handle;
typedef VD::Ccb_halfedge_circulator   Ccb_halfedge_circulator;



class MainWindow : public QMainWindow, public Ui_MainWindow
{
	Q_OBJECT

private:
	Scene* m_scene;
	QImage entree;
	QImage sortie;

	QLabel *entreeLabel = new QLabel();
	QLabel *sortieLabel = new QLabel();

	CGAL::Voronoi_diagram_2< DT, AT, AP > Vd;

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
		void on_actionLinearDiff_triggered();
		void on_actionOpti_triggered();
		void majLloyd();
		void on_actionLloyd_triggered();
		void on_actionLloyd2_triggered();
		void on_actionLloydTen_triggered();

		void open_edg(const QString& filename);
		std::vector<double> MainWindow::density_pic();

		void open(const QString& filename);
		void clear();
		int nNeeded();
};

#endif // _WINDOW_
