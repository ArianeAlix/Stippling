// Qt
#include <QtGui>
#include <QActionGroup>
#include <QFileDialog>
#include <QInputDialog>
#include <QClipboard>

#include "window.h"
#include "scene.h"

#include <fstream>
#include<iostream>
using namespace std;



MainWindow::MainWindow() :
	QMainWindow(), Ui_MainWindow(),
	maxNumRecentFiles(15), recentFileActs(15)
{
	setupUi(this);


	// Handling actions
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
	delete m_scene;
}

void MainWindow::update()
{
	viewer->repaint();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
}


int MainWindow::nNeeded() {
	double density = 0;

	for (int jj = 0; jj < entree.height(); jj++) {
		for (int ii = 0; ii < entree.width(); ii++) {
			int gray = qGray(entree.pixel(ii, jj));
			density += 1 - (double(gray) / 255.0);
		}
	}
	return ceil(density);
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
	Vd.clear();

	actionRandom->setEnabled(true);
	actionLinear->setEnabled(true);
	actionLinearDiff->setEnabled(true);

	actionClear->setEnabled(false);
	actionOpti->setEnabled(false);
	actionLloyd->setEnabled(false);
	actionLloyd2->setEnabled(false);
	actionLloydTen->setEnabled(false);
	lld->close();
}

void MainWindow::on_actionRandom_triggered() {
	clear();
	Vd.clear();

	vector<Site_2> vertices;
	int N = nNeeded();
	
	for (int ii = 0; ii < sortie.width(); ii++) {
		for (int jj = 0; jj < sortie.height(); jj++) {
			if (double(rand()) / double(RAND_MAX) > 0.90/*1- double(N)/(sortie.width()*sortie.height())*/) { //On colorie en noir 10% des points
				sortie.setPixel(ii, jj, QColor(0, 0, 0).rgb());
				vertices.push_back(Site_2(ii, jj));
			}
		}
	}

	Vd.insert(vertices.begin(), vertices.end());

	cout << "Number of vertices in the Voronoi Diagram: " << Vd.number_of_vertices() << endl;
	cout << "Number of faces in the Voronoi Diagram: " << Vd.number_of_faces() << endl;




	sortieLabel->setPixmap(QPixmap::fromImage(sortie));
	gridLayout->addWidget(sortieLabel);

	actionClear->setEnabled(true);
	actionOpti->setEnabled(true);

}




void MainWindow::on_actionLinear_triggered() {
	Vd.clear();

	clear();

	vector<Site_2> vertices;
	double density = 0;
	double err_quant = 0; //erreur de quantification
	
	for (int jj = 0; jj < sortie.height(); jj++) {
		density = 0;//On remet à 0 à chaque ligne
		for (int ii = 0; ii < sortie.width(); ii++) {
			int gray = qGray(entree.pixel(ii, jj));
			density += 1 - (double(gray) / 255.0);
			if (density >= 1) { //On colorie en noir le pixel quand on a atteint une densité cumulée de 1
				sortie.setPixel(ii, jj, QColor(0, 0, 0).rgb());
				vertices.push_back(Site_2(ii, jj));

				err_quant += 1-density;

				density = 0;
			}
		}
	}

	Vd.insert(vertices.begin(), vertices.end());

	cout << "Number of vertices in the Voronoi Diagram: " << Vd.number_of_vertices() << endl;

	sortieLabel->setPixmap(QPixmap::fromImage(sortie));
	gridLayout->addWidget(sortieLabel,0,0,2,1);

	string erreur = "Erreur de quantification: " + to_string(err_quant / (vertices.end()-vertices.begin()) *100) + "%";
	const char * err_char = erreur.c_str();
	setStatusTip(QApplication::translate("MainWindow", err_char, nullptr));
	//cout << "Erreur de quantification: " << err_quant / N * 100 << "%" << endl;

	actionClear->setEnabled(true);
	actionOpti->setEnabled(true);
}




void MainWindow::on_actionLinearDiff_triggered() {
	Vd.clear();

	clear();

	vector<Site_2> vertices;
	double density = 0;
	double residu = 0;
	double err_quant = 0; //erreur de quantification

	//On crée une copie de l'image, dans laquelle on stocke la valeur des pixels sous forme de densité
	//et on peut passer par des valeurs négatives ou supérieures à 1
	vector<double> copie= {};
	for (int jj = 0; jj < sortie.height(); jj++) {
		for (int ii = 0; ii < sortie.width(); ii++) {
			int gray = qGray(entree.pixel(ii, jj));
			copie.push_back( 1 - (double(gray) / 255.0));
		}
	}


	for (int jj = 0; jj < sortie.height(); jj++) {
		density = 0;//On remet à 0 à chaque ligne
		residu = 0;
		for (int ii = 0; ii < sortie.width(); ii++) {
			//cout << copie[ii + jj * sortie.width()] << endl;
			density += copie[ii + jj * sortie.width()];
			if (density >= 0.5) { //On colorie en noir le pixel quand on a atteint une densité cumulée de 0.5
				sortie.setPixel(ii, jj, QColor(0, 0, 0).rgb());
				vertices.push_back(Site_2(ii, jj));

				//On transmet le résidu (qui peut être négatif) au pixel suivant de la ligne s'il existe
				residu = density - 1;
				//cout << residu << endl;
				if (ii < sortie.width()-1) {
					copie[ii + 1 + jj * sortie.width()] += residu;
				}

				err_quant += 1-density;
				density = 0;
			}			
		}
	}

	Vd.insert(vertices.begin(), vertices.end());

	std::cout << "Number of vertices in the Voronoi Diagram: " << Vd.number_of_vertices() << endl;

	sortieLabel->setPixmap(QPixmap::fromImage(sortie));
	gridLayout->addWidget(sortieLabel, 0, 0, 2, 1);

	string erreur = "Erreur de quantification: " + to_string(err_quant / (vertices.end() - vertices.begin()) * 100) + "%";
	const char * err_char = erreur.c_str();
	setStatusTip(QApplication::translate("MainWindow", err_char, nullptr));
	//cout << "Erreur de quantification: " << err_quant / N * 100 << "%" << endl;

	actionClear->setEnabled(true);
	actionOpti->setEnabled(true);
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



void MainWindow::on_actionLoad_triggered(){
	//On active les boutons de génération de points
	actionRandom->setEnabled(true);
	actionLinear->setEnabled(true);
	actionLinearDiff->setEnabled(true);

	QString fileName = QFileDialog::getOpenFileName(this, tr("Choose picture"), ".");
	if (!fileName.isEmpty())
		open(fileName);
}


void MainWindow::open_edg(const QString& filename){
	QApplication::setOverrideCursor(Qt::WaitCursor);
	m_scene->load(filename);
	QApplication::restoreOverrideCursor();

	for (Site_iterator it = Vd.sites_begin(); it != Vd.sites_end();it++) {
		m_scene->add_vertex(Point_2(it->x(),entree.height()-it->y()));
	}

	std::vector<Site_2> vertices = m_scene->get_vertices();

	cout << "Number of vertices in the Voronoi Diagram: " << Vd.number_of_vertices() << endl;
	
	std::vector<double> densities_vor=m_scene->densities(); // vector of the areas of the cell

	update();
}


void MainWindow::on_actionOpti_triggered(){
	actionLloyd->setEnabled(true);
	actionLloyd2->setEnabled(true);
	actionLloydTen->setEnabled(true);

	actionOpti->setEnabled(false);
	actionRandom->setEnabled(false);
	actionLinear->setEnabled(false);
	actionLinearDiff->setEnabled(false);

	ofstream rectangle;
	rectangle.open("rectangle.edg");
	rectangle << 4 << endl;
	
	double h = entree.width();
	double w = entree.height();

	double ratio = max(w, h);

	rectangle << 0 << " " << 0 << " " << 0 << " " << w << endl;
	rectangle << 0 << " " << 0 << " " << h << " " << 0 << endl;
	rectangle << h << " " << 0 << " " << h << " " << w << endl;
	rectangle << 0 << " " << w << " " << h << " " << w << endl;

	m_scene = new Scene;
	viewer->set_camera(h/2,w/2,1.5/ratio);
	viewer->set_scene(m_scene);

	gridLayoutLld->addWidget(viewer, 0, 0, 1, 1);

	lld->resize(h, w);
	lld->setWindowTitle("Lloyd algorithm");
	lld->show();

	QString fileName = "rectangle.edg";
	if (!fileName.isEmpty())
		open_edg(fileName);
}


std::vector<double> MainWindow::density_pic() {
	std::vector<double> densities_pic;
	std::map<Site_2, std::vector<Site_2>> cells = m_scene->pixels_by_gen(entree.height(), entree.width());

	for (Site_iterator it = Vd.sites_begin(); it != Vd.sites_end(); it++) {//On parcourt les générateurs
		double density = 0;
		for (vector<Site_2>::iterator it2 = cells[Site_2(it->x(),it->y())].begin(); it2 != cells[Site_2(it->x(), it->y())].end(); it2++) {
			//Puis les points qui lui sont associés
			int gray = qGray(entree.pixel(it2->x(), it2->y()));
			density += 1 - (double(gray) / 255.0);
		}
		//On ajoute la moyenne des densités pour la cellule dans le vecteur densities_pic
		densities_pic.push_back(density / (cells[Site_2(it->x(), it->y())].end() - cells[Site_2(it->x(), it->y())].begin()));
	}
	return densities_pic;
}


void MainWindow::majLloyd() {

	//On met à jour le voronoi et l'image halftone
	clear();
	Vd.clear();
	std::vector<Site_2> vertices = m_scene->get_vertices();

	Vd.insert(vertices.begin(), vertices.end());

	//cout << "Number of vertices in the Voronoi Diagram: " << Vd.number_of_vertices() << endl;

	for (Site_iterator it = Vd.sites_begin(); it != Vd.sites_end(); it++) {
		sortie.setPixel(it->x(), entree.height() - it->y(), QColor(0, 0, 0).rgb());
	}
	sortieLabel->setPixmap(QPixmap::fromImage(sortie));


	//density of the half tone pic by voronoi cell
	std::vector<double> densities_vor = m_scene->densities();

	//density of the normal picture
	std::vector<double> densities_pic = density_pic();
	double density_pic_tot = 0;
	for (vector<double>::iterator it = densities_pic.begin(); it != densities_pic.end(); it++) {
		density_pic_tot += *it;
	}

	//On affiche la densité moyenne de l'image pour les cellules
	std::cout << "(average cell density for the original pic: " << density_pic_tot / (densities_pic.end() - densities_pic.begin()) << ")" << std::endl;
	std::cout << "Number of cells : " << (densities_pic.end() - densities_pic.begin()) << std::endl << std::endl;

	QApplication::restoreOverrideCursor();
	update();

}

void MainWindow::on_actionLloyd_triggered() {
	QApplication::setOverrideCursor(Qt::WaitCursor);

	int h = entree.height();
	int w = entree.width();

	m_scene->lloyd();
	majLloyd();
}

void MainWindow::on_actionLloyd2_triggered(){
	QApplication::setOverrideCursor(Qt::WaitCursor);

	int h = entree.height();
	int w = entree.width();

	std::vector<Site_2> points = {};

	std::map<Site_2, std::vector<Site_2>> cells = m_scene->pixels_by_gen(h,w);

	for (Site_iterator it = Vd.sites_begin(); it != Vd.sites_end(); it++) {//On parcourt les générateurs
		double bary_x = 0;
		double bary_y = 0;
		double density = 0;
		//std::cout <<  cells[Site_2(it->x(),h-it->y())].end() - cells[Site_2(it->x(),h- it->y())].begin() <<std::endl;
		for (vector<Site_2>::iterator it2 = cells[Site_2(it->x(), h- it->y())].begin(); it2 != cells[Site_2(it->x(),h- it->y())].end(); it2++) {
			//Puis les points qui lui sont associés
			//On calcule le barycentre des pixels pondérés par leur densité en noir
			int gray = qGray(entree.pixel(it2->x(), h - it2->y()));
			density+= (1 - (double(gray) / 255.0));
			bary_x += it2->x()*(1 - (double(gray) / 255.0));
			bary_y += it2->y()*(1 - (double(gray) / 255.0));
		}
		points.push_back(Site_2(int(bary_x/density), int(bary_y/density)));
	}

	m_scene->lloyd2(points);

	majLloyd();

	//On désactive après une itération (sinon ca fait n'importe quoi dès la 2eme)
	actionLloyd2->setEnabled(false);
}


void MainWindow::on_actionLloydTen_triggered() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	for (int i = 0; i < 10; i++){
		m_scene->lloyd();
	}

	//On met à jour le voronoi et l'image halftone
	clear();
	Vd.clear();
	std::vector<Site_2> vertices = m_scene->get_vertices();

	Vd.insert(vertices.begin(), vertices.end());
	for (Site_iterator it = Vd.sites_begin(); it != Vd.sites_end(); it++) {
		sortie.setPixel(it->x(), entree.height() - it->y(), QColor(0, 0, 0).rgb());
	}
	sortieLabel->setPixmap(QPixmap::fromImage(sortie));


	//density of the half tone pic by voronoi cell
	std::vector<double> densities_vor = m_scene->densities();

	//density of the normal picture
	std::vector<double> densities_pic = density_pic();
	double density_pic_tot = 0;
	for (vector<double>::iterator it = densities_pic.begin(); it != densities_pic.end(); it++) {
		density_pic_tot += *it;
	}

	//On affiche la densité moyenne de l'image pour les cellules
	std::cout << "(average cell density for the original pic: " << density_pic_tot / (densities_pic.end() - densities_pic.begin()) << ")" << std::endl;
	std::cout << "Number of cells : " << (densities_pic.end() - densities_pic.begin()) << std::endl << std::endl;

	QApplication::restoreOverrideCursor();
	update();

}


