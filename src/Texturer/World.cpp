#include <algorithm>
#include <list>
#include <math.h>
#include <iostream>
#include <assert.h>

#include "World.h"

/** @brief associatePoints
  *
  * @todo: document this function
  */
bool World::associatePoints()
{
    std::vector<Point3dColor* >::iterator currPoint = points_.begin ();
    std::vector<Point3dColor* >::iterator pointEnd = points_.end ();

    int nbPointOnePercent = points_.size () / 100;
    int count = 0;
    int percent = 0;

    for (; currPoint != pointEnd; ++currPoint)
    {
        std::list<Face* >* closeFaces = getNeighbour(**currPoint);

        //std::cout << *currPoint << std::endl;

        std::list<Face* >::iterator currFace = closeFaces->begin ();
        std::list<Face* >::iterator faceEnd = closeFaces->end ();

        for (; currFace != faceEnd; ++currFace)
        {
            //std::cout << "Current Face" << *currFace << std::endl;

            Point2dColor* p = projectionOnTriangle((**currPoint), (**currFace));
            if (p)
            {
                (*currFace)->associatePoint(p);
                break;
            }
        }

        count++;
        if (count == nbPointOnePercent)
        {
            std::cout << "|";
            percent++;
            count = 0;
        }
    }



    std::cout << std::endl;
    return false;
}


Point2dColor* World::projectionOnTriangle(Point3dColor& point, Face& face)
{
    Plan plan = face.getPlanBN();
    Point3d* vect = face.getVectA();

    float u = (plan.d_ + plan.a_ * point.x_ + plan.b_ * point.y_ + plan.c_ * point.z_) /
              (plan.a_ * vect->x_ + plan.b_ * vect->y_ + plan.c_ * vect->z_);

    plan = face.getPlanAN();
    vect = face.getVectB();

    float v = (plan.d_ + plan.a_ * point.x_ + plan.b_ * point.y_ + plan.c_ * point.z_) /
              (plan.a_ * vect->x_ + plan.b_ * vect->y_ + plan.c_ * vect->z_);

    //std::cout << "u = " << u << std::endl;
    //std::cout << "v = " << v << std::endl << std::endl;

    if (u < 0 || v < 0 || u + v > 1)
        return NULL;

    return new Point2dColor(u, v, point.color_);
}

std::list<Face* >* World::getNeighbour (Point3d& p)
{

    //std::cout << "getNeighbour: " << &p << std::endl;


    std::list<Face* >* fList = new std::list<Face* > ();
    std::list<float > distList;

    Point3d min = Point3d(p.x_ - maxFaceDist_, p.y_ - maxFaceDist_, p.z_ - maxFaceDist_);
    Point3d max = Point3d(p.x_ + maxFaceDist_, p.y_ + maxFaceDist_, p.z_ + maxFaceDist_);


    for (float i = min.x_; i <= max.x_; i += gridSize_)
        for (float j = min.y_; j <= max.y_; j += gridSize_)
            for (float k = min.z_; k <= max.z_; k += gridSize_)
            {
                std::vector<Face*> fSubList = getAtIndex(i, j, k);

                std::vector<Face*>::iterator currSubListFace = fSubList.begin ();
                std::vector<Face*>::iterator subListFaceEnd = fSubList.end ();

                for (; currSubListFace != subListFaceEnd; ++currSubListFace)
                {
                    Face* f = *currSubListFace;

                    //std::cout << "current close face: " << f << std::endl;

                    float dist = pow(f->getCentroid ().x_ - p.x_, 2) +
                                 pow(f->getCentroid ().y_ - p.y_, 2) +
                                 pow(f->getCentroid ().z_ - p.z_, 2);


                    std::list<float >::iterator currDist = distList.begin ();
                    std::list<float >::iterator distEnd = distList.end ();
                    std::list<Face* >::iterator currFace = fList->begin ();


                    for (; currDist != distEnd; ++currDist, currFace++)
                        if (dist <= *currDist)
                            break;

                    for (; currDist != distEnd; ++currDist, currFace++)
                        if (dist == *currDist && *currFace == f)
                            break;

                    if (*currFace != f)
                    {
                        distList.insert (currDist, dist);
                        fList->insert (currFace, f);
                    }

                }
            }

    return fList;
}

std::vector<Face*>& World::getAtIndex(float x, float y, float z)
{
    int i = (int)((x - min_.x_) / gridSize_);
    int j = (int)((y - min_.y_) / gridSize_);
    int k = (int)((z - min_.z_) / gridSize_);

    //std::cout << "[" << x << "," << y << ","<< z << "]" << "[" << i << "," << j << ","<< k << "]" << std::endl;

    assert(i >= 0 && i < faces_.size ());
    assert(j >= 0 && j < faces_[0].size ());
    assert(k >= 0 && k < faces_[0][0].size ());

    return faces_[i][j][k];
}

/** @brief World
  *
  * @todo: document this function
  */
 World::World(std::vector<Point3dColor* >& points, std::vector<Face* >& faces, Point3d& min, Point3d& max)
    : points_(points),
      //gridSize_ (0.025),
      //maxFaceDist_ (0.0175),
      gridSize_ (1),
      maxFaceDist_ (0.5),
      min_ (min),
      max_ (max)
{
    // create grid
    for (int i = 0; i <= (max.x_ - min.x_) / gridSize_; ++i)
    {
        faces_.push_back(std::vector<std::vector<std::vector<Face* > > > ());
        for (int j = 0; j <= (max.y_ - min.y_) / gridSize_; ++j)
        {
            faces_[i].push_back(std::vector<std::vector<Face* > > ());
            for (int k = 0; k <= (max.z_ - min.z_) / gridSize_; ++k)
            {
                faces_[i][j].push_back(std::vector<Face* > ());
            }
        }
    }


    // insert faces
    std::vector<Face* >::iterator currFace = faces.begin ();
    std::vector<Face* >::iterator faceEnd = faces.end ();

    for (; currFace != faceEnd; ++currFace)
    {
        Face* face = *currFace;

        // insert face in all case include in bounding box
        Point3d p0 = *(face->getOrigine ());
        Point3d p1 = Point3d (face->getVectA ()->x_ + p0.x_, face->getVectA ()->y_ + p0.y_, face->getVectA ()->z_ + p0.z_);
        Point3d p2 = Point3d (face->getVectB ()->x_ + p0.x_, face->getVectB ()->y_ + p0.y_, face->getVectB ()->z_ + p0.z_);

        Point3d bbMin = Point3d(std::min(std::min(p0.x_, p1.x_), p2.x_),
                                std::min(std::min(p0.y_, p1.y_), p2.y_),
                                std::min(std::min(p0.z_, p1.z_), p2.z_));

        Point3d bbMax = Point3d(std::max(std::max(p0.x_, p1.x_), p2.x_),
                                std::max(std::max(p0.y_, p1.y_), p2.y_),
                                std::max(std::max(p0.z_, p1.z_), p2.z_));

        //std::cout << "gridSize: " << gridSize_ << std::endl;
        //std::cout << "bbMin.x_: " << bbMin.y_ << std::endl;
        //std::cout << "bbMax.x_: " << bbMax.y_ << std::endl;
        //exit (1);
        for (float i = bbMin.x_; i <= bbMax.x_; i += gridSize_)
            for (float j = bbMin.y_; j <= bbMax.y_; j += gridSize_)
                for (float k = bbMin.z_; k <= bbMax.z_; k += gridSize_)
                    getAtIndex(i, j, k).push_back(face);
        //std::cout << "end " << std::endl;

        //if (loop++ == 3)exit (1);

    }



/*
    for (int k = 0; k <= (max.z_ - min.z_) / gridSize_; ++k)
    {
        for (int j = 0; j <= (max.y_ - min.y_) / gridSize_; ++j)
        {
            for (int i = 0; i <= (max.x_ - min.x_) / gridSize_; ++i)
            {
                if (faces_[i][j][k].size())
                    std::cout << "X";
                else
                    std::cout << "0";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }

*/
}

