#ifndef _SCENE_H_
#define _SCENE_H_

// std
#include <fstream>
#include <algorithm>
#include <list>
#include <queue>

// Qt
#include <QtOpenGL>

#include <GL/gl.h>

// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Cartesian.h>

using namespace std;

#undef min
#undef max

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

class Scene
{
public:
	typedef Kernel::FT FT;
	typedef Kernel::Point_2 Point;


private:
	// input points and hull
	std::list<Point> m_points;
	std::list<Point> m_hull;
	std::list<Point> m_lower_hull;
	std::list<Point> m_upper_hull;

	Point m_mouse_pos;

public:    
	Scene()
	{
	}

	~Scene()
	{
	}    

	void clear()
	{
		m_points.clear();
	}

	void random(int w, int h)
	{
		clear();

		for(int i = 0; i < 100; i++)
		{
			const double x = random_int(0, w);
			const double y = random_int(0, h);
			m_points.push_back(Point(x, y));
		}
	}

	double random_int(const int min, const int max)
	{
		int range = max - min;
		return min + (int(rand()) / int(RAND_MAX)) * range;
	}

	void render()
	{
		// render points
		::glColor3ub(0, 0, 0); // black color
		::glPointSize(2.0f);

		::glBegin(GL_POINTS);
		std::list<Point>::iterator it;
		for(it = m_points.begin(); it != m_points.end(); it++)
		{
			const Point& p = *it;
			::glVertex2d(p.x(), p.y());
		}
		::glEnd();


		// render Voronoi ?
		/*
		::glColor3ub(0, 255,0); // green color
		::glLineWidth(3.0f);

		::glBegin(GL_LINE_STRIP); //  makes an open polyline
		for (it = m_upper_hull.begin(); it != m_upper_hull.end(); it++)
		{
			const Point& p = *it;
			::glVertex2d(p.x(), p.y());
		}
		::glEnd();
		*/
	}
	//Insert Voronoi ?
	
};

#endif // _SCENE_H_
