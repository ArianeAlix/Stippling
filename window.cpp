// Qt
#include <QtGui>
#include <QActionGroup>
#include <QFileDialog>
#include <QInputDialog>
#include <QClipboard>

#include "window.h"

#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>
// typedefs for defining the adaptor
typedef CGAL::Exact_predicates_inexact_constructions_kernel                  K;
typedef CGAL::Delaunay_triangulation_2<K>                                    DT;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<DT>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<DT> AP;
typedef CGAL::Voronoi_diagram_2<DT, AT, AP>                                  VD;
// typedef for the result type of the point location
typedef AT::Site_2                    Site_2;
typedef AT::Point_2                   Point_2;
typedef VD::Locate_result             Locate_result;
typedef VD::Vertex_handle             Vertex_handle;
typedef VD::Face_handle               Face_handle;
typedef VD::Halfedge_handle           Halfedge_handle;
typedef VD::Ccb_halfedge_circulator   Ccb_halfedge_circulator;



MainWindow::MainWindow() :
	QMainWindow(), Ui_MainWindow(),
	maxNumRecentFiles(15), recentFileActs(15)
{
	setupUi(this);

	// init scene
	m_scene = new Scene;
	//viewer->set_scene(m_scene);

	// Handling actions
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
	delete m_scene;
}

void MainWindow::update()
{
	//viewer->repaint();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
}

void MainWindow::clear() {
	for (int ii = 0; ii < sortie.width(); ii++) {
		for (int jj = 0; jj < sortie.height(); jj++) {
			sortie.setPixel(ii, jj, QColor(255, 255, 255).rgb());
		}
	}

	sortieLabel->setPixmap(QPixmap::fromImage(sortie));
	gridLayout->addWidget(sortieLabel);
}

void MainWindow::on_actionClear_triggered()
{
	clear();
}

void MainWindow::on_actionRandom_triggered() {
	clear();

	vector<Site_2> vertices;

	for (int ii = 0; ii < sortie.width(); ii++) {
		for (int jj = 0; jj < sortie.height(); jj++) {
			if (double(rand()) / double(RAND_MAX) > 0.9) { //On colorie en noir 10% des points
				sortie.setPixel(ii, jj, QColor(0, 0, 0).rgb());
				vertices.push_back(Site_2(ii, jj));
			}
		}
	}

	CGAL::Voronoi_diagram_2< DT, AT, AP > Vd;
	Vd.insert(vertices.begin(), vertices.end());

	cout << "Number of vertices in the Voronoi Diagram: " << Vd.number_of_vertices() << endl;
	cout << "Number of faces in the Voronoi Diagram: " << Vd.number_of_faces() << endl;

	sortieLabel->setPixmap(QPixmap::fromImage(sortie));
	gridLayout->addWidget(sortieLabel);

}


void MainWindow::on_actionLinear_triggered() {
	clear();

	vector<Site_2> vertices;
	double density = 0;

	
	for (int jj = 0; jj < sortie.height(); jj++) {
		density = 0;//On remet à 0 à chaque ligne
		for (int ii = 0; ii < sortie.width(); ii++) {
			if (density >= 1) { //On colorie en noir le pixel quand on a atteint une desnité cumulée de 1
				sortie.setPixel(ii, jj, QColor(0, 0, 0).rgb());
				vertices.push_back(Site_2(ii, jj));
				density = 0;
			}
			else {
				int gray = qGray(entree.pixel(ii, jj));
				density += 1-(double(gray) / 255.0);
			}
		}
	}

	CGAL::Voronoi_diagram_2< DT, AT, AP > Vd;
	Vd.insert(vertices.begin(), vertices.end());

	cout << "Number of vertices in the Voronoi Diagram: " << Vd.number_of_vertices() << endl;
	cout << "Number of faces in the Voronoi Diagram: " << Vd.number_of_faces() << endl;

	sortieLabel->setPixmap(QPixmap::fromImage(sortie));
	gridLayout->addWidget(sortieLabel);

}




void MainWindow::open(const QString& filename) {
	this->resize(0, 0);

	QApplication::setOverrideCursor(Qt::WaitCursor);
	QApplication::restoreOverrideCursor();

	entree.load(filename);

	//Convertir en noir et blanc
	for (int ii = 0; ii < entree.width(); ii++) {
		for (int jj = 0; jj < entree.height(); jj++) {
			int gray = qGray(entree.pixel(ii, jj));
			entree.setPixel(ii, jj, QColor(gray, gray, gray).rgb());
		}
	}

	//Mettre l'image chargée dans une nouvelle fenêtre
	entreeLabel->setPixmap(QPixmap::fromImage(entree));
	gridLayoutPic->addWidget(entreeLabel);
	wdg->adjustSize();

	//Image en sortie pour les points
	sortie = entree; //Pour avoir les même dimensions

	//Tout en blanc
	for (int ii = 0; ii < sortie.width(); ii++) {
		for (int jj = 0; jj < sortie.height(); jj++) {
			sortie.setPixel(ii, jj, QColor(255, 255, 255).rgb());
		}
	}

	sortieLabel->setPixmap(QPixmap::fromImage(sortie));
	gridLayout->addWidget(sortieLabel);

	//Paramètres pour les fenêtres
	wdg->setWindowTitle("Original picture");
	wdg->show();
}



void MainWindow::on_actionLoad_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Choose picture"), ".");
	if (!fileName.isEmpty())
		open(fileName);
}





