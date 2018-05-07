#ifndef _SCENE_H_
#define _SCENE_H_
// std
#include <fstream>
#include <algorithm>

// Qt
#include <QtOpenGL>

// CGAL
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_plus_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/license/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include "include/cdt.h"
#include "include/dt.h"

#undef min
#undef max

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

class Scene
{
public:
	//typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

	typedef Kernel::FT FT;
	typedef Kernel::Point_2 Point;
	typedef Kernel::Vector_2 Vector;
	typedef Kernel::Triangle_2 Triangle;

	// Constrained Delaunay triangulation
	typedef CGAL::Triangulation_vertex_base_2<Kernel> Vb;
	typedef CGAL::Delaunay_mesh_face_base_2<Kernel> Fb;
	typedef My_vertex_base<Kernel, Vb> MVb;
	typedef My_face_base<Kernel, Fb> MFb;
	typedef CGAL::Triangulation_data_structure_2<MVb, MFb> MTDS;
	typedef CGAL::Exact_intersections_tag Itag;
	typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, MTDS, Itag> MCDT;
	typedef CGAL::Constrained_triangulation_plus_2<MCDT> MCDTP;
	typedef CCDT<MCDTP> CDT;

	// Delaunay triangulation
	typedef CGAL::Triangulation_vertex_base_2<Kernel> Vertex_base;
	typedef Dvertex_base<Vertex_base> DVb;
	typedef CGAL::Triangulation_face_base_2<Kernel> DFb;
	typedef CGAL::Triangulation_data_structure_2<DVb, DFb> TDS;
	typedef MDT<Kernel, TDS> DT;

private:
	CDT m_cdt; // Domain
	DT m_dt; // triangulation
	std::list<Triangle> m_triangles; // inside triangles of domain

	bool m_view_edges;
	Point m_mouse_pos;

public:
	Scene()
	{
		m_view_edges = true;
		m_cdt.add_box();
		this->update_domain();
	}

	~Scene()
	{
		clear();
	}

	void toggle_view_edges() { m_view_edges = !m_view_edges; }

	void clear()
	{
		m_dt.clear();
	}  

	std::vector<Point> get_vertices() {
		std::vector<Point> vertices = m_dt.get_vertices();
		return vertices;
	}


	void add_vertex(const Point& p)
	{
		if (m_cdt.inside(p))
			m_dt.insert(p);
		else
			std::cout << "outside point" << std::endl;
	}

	void set_mouse_pos(const Point& pos) { m_mouse_pos = pos; }

	void load(const QString& filename)
	{
		if (filename.contains(".edg", Qt::CaseInsensitive))
		{
			m_dt.clear();
			m_cdt.clear();
			m_cdt.read_pslg(qPrintable(filename));
			update_domain();
		}
	}

	void add_box()
	{
		m_dt.clear();
		m_cdt.clear();
		m_cdt.add_box();
		update_domain();
	}

	void add_circle()
	{
		m_dt.clear();
		m_cdt.clear();
		m_cdt.add_circle();
		update_domain();
	}

	void update_domain()
	{
		m_cdt.tag_faces_inside_outside();
		m_triangles.clear();
		m_cdt.inside_triangles(std::back_inserter(m_triangles));
	}

	void add_random_vertices(const int nb)
	{
		std::list<Point> points;
		m_cdt.generate_random_points(std::back_inserter(points), nb);
		m_dt.insert(points.begin(), points.end());
	}

	void render()
	{
		m_cdt.gl_draw_inside_faces(200, 200, 255);

		if (m_view_edges)
			m_cdt.gl_draw_constrained_edges(200, 0, 0);

		m_dt.draw_cvt_centroids_intersect(m_triangles.begin(), m_triangles.end());

		m_dt.gl_draw_generators(2.0f, 0, 0, 0);
	}


	void lloyd()	{
		std::cout << "Lloyd iteration...";
		const double av_len = m_dt.lloyd(m_triangles.begin(), m_triangles.end());
		std::cout << "done (average len: " << av_len << ")" << std::endl;
	}

	void lloyd2(std::vector<Point> points) {
		std::cout << "Lloyd iteration...";
		m_dt.lloyd2(m_triangles.begin(), m_triangles.end(),points);
		std::cout << "done " << std::endl;
	}

	std::vector<double> densities()	{
		std::cout << "Computing areas of cells...";
		std::vector<double> densities;
		std::vector<double> areas= m_dt.area_lloyd(m_triangles.begin(), m_triangles.end());
		//std::cout << "done (area of first cell :" << *areas.begin() << ")" << std::endl << *(areas.begin() + 1);

		double density_tot = 0;
		std::vector<double>::iterator v;
		for (v = areas.begin(); v != areas.end(); v++) {
			densities.push_back( 1 / (*v));
			density_tot += 1 / (*v);
		}
		std::cout << "done" <<std::endl << "(average cell density for voronoi: " << density_tot / (densities.end() - densities.begin()) << ")" << std::endl;
		return densities;
	}


	std::map<Point, std::vector<Point>> pixels_by_gen(int h, int w){
		std::map<Point, std::vector<Point>> cells =	m_dt.by_gen(h,w);
		return cells;
	}
};

#endif // _SCENE_H_
