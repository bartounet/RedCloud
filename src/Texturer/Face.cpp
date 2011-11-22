#include "Face.h"
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <list>
#include <utility>

void Face::printInfos (std::vector<Face* >& faces)
{
    size_t nbAssociatedPoint_ = 0;
    size_t minAssociatedPoint_ = 10000000;
    size_t maxAssociatedPoint_ = 0;

    for (size_t i = 0; faces.size () > i; i++)
    {
        nbAssociatedPoint_ += faces[i]->getAssociatedPoints ().size ();
        minAssociatedPoint_ = std::min (minAssociatedPoint_, faces[i]->getAssociatedPoints ().size ());
        maxAssociatedPoint_ = std::max (maxAssociatedPoint_, faces[i]->getAssociatedPoints ().size ());
    }

    std::cout << "moy = " << (nbAssociatedPoint_ + 0.) / faces.size () << std::endl;
    std::cout << "min = " << minAssociatedPoint_ << std::endl;
    std::cout << "max = " << maxAssociatedPoint_ << std::endl;
}

std::vector<Point2dColor* >& Face::getAssociatedPoints ()
{
    return associatedPoints_;
}

std::vector<Point3dColor* > Face::points_ = std::vector<Point3dColor* > ();

/** @brief Face
  *
  * @todo: document this function
  */

 Face::Face(int p1, int p2, int p3)
    : plan_(Plan(0, 0, 0, 0)),
      an_(Plan(0, 0, 0, 0)),
      bn_(Plan(0, 0, 0, 0)),
      centroid_(Point3d((Face::points_[p1]->x_ + Face::points_[p2]->x_ + Face::points_[p3]->x_) / 3,
                        (Face::points_[p1]->y_ + Face::points_[p2]->y_ + Face::points_[p3]->y_) / 3,
                        (Face::points_[p1]->z_ + Face::points_[p2]->z_ + Face::points_[p3]->z_) / 3)),
      p0_(p1),
      p1_(p2),
      p2_(p3)
{
    coordTexture_[0] = NULL;
    coordTexture_[1] = NULL;
    coordTexture_[2] = NULL;


    o_ = new Point3d (Face::points_[p1]->x_, Face::points_[p1]->y_, Face::points_[p1]->z_);

    a_ = new Point3d (Face::points_[p2]->x_ - Face::points_[p1]->x_,
                      Face::points_[p2]->y_ - Face::points_[p1]->y_,
                      Face::points_[p2]->z_ - Face::points_[p1]->z_);
    b_ = new Point3d (Face::points_[p3]->x_ - Face::points_[p1]->x_,
                      Face::points_[p3]->y_ - Face::points_[p1]->y_,
                      Face::points_[p3]->z_ - Face::points_[p1]->z_);


    plan_.a_ = a_->y_ * b_->z_- a_->z_ * b_->y_;
    plan_.b_ = -1 * (a_->x_ * b_->z_ - a_->z_ * b_->x_);
    plan_.c_ = a_->x_ * b_->y_ - a_->y_ * b_->x_;
    plan_.d_ = -o_->x_ * plan_.a_ - o_->y_ * plan_.b_ - o_->z_ * plan_.c_;

    an_.a_ = a_->y_ * plan_.c_- a_->z_ * plan_.b_;
    an_.b_ = -1 * (a_->x_ * plan_.c_ - a_->z_ * plan_.a_);
    an_.c_ = a_->x_ * plan_.b_ - a_->y_ * plan_.a_;
    an_.d_ = -o_->x_ * an_.a_ - o_->y_ * an_.b_ - o_->z_ * an_.c_;

    bn_.a_ = b_->y_ * plan_.c_- b_->z_ * plan_.b_;
    bn_.b_ = -1 * (b_->x_ * plan_.c_ - b_->z_ * plan_.a_);
    bn_.c_ = b_->x_ * plan_.b_ - b_->y_ * plan_.a_;
    bn_.d_ = -o_->x_ * bn_.a_ - o_->y_ * bn_.b_ - o_->z_ * bn_.c_;


    Point2dColor* p1_2D = new Point2dColor (0, 0, Face::points_[p1]->color_);
    Point2dColor* p2_2D = new Point2dColor (1, 0, Face::points_[p2]->color_);
    Point2dColor* p3_2D = new Point2dColor (0, 1, Face::points_[p3]->color_);

    associatePoint (p1_2D);
    associatePoint (p2_2D);
    associatePoint (p3_2D);

}

void Face::setCoordTexture(Point2d* p0, Point2d* p1, Point2d* p2)
{
    coordTexture_[0] = p0;
    coordTexture_[1] = p1;
    coordTexture_[2] = p2;
}

Point3d* Face::getOrigine ()
{
    return o_;
}

void Face::associatePoint(Point2dColor* p)
{
    associatedPoints_.push_back (p);
}

Point3d& Face::getCentroid()
{
    return centroid_;
}

Plan& Face::getPlan()
{
    return plan_;
}

Plan& Face::getPlanAN()
{
    return an_;
}

Plan& Face::getPlanBN()
{
    return bn_;
}

Point3d* Face::getVectA()
{
    return a_;
}

Point3d* Face::getVectB()
{
    return b_;
}

Point2d** Face::getCoordTexture()
{
    return coordTexture_;
}

Color Face::getApproxColor (Point2d p)
{
    Color closestColor = Color (0, 0, 0);
    std::list<std::pair <Color, float> > pointSorted = std::list<std::pair<Color, float> > ();

    assert (associatedPoints_.size () >= 3);

    for (size_t i = 0; i < associatedPoints_.size (); i++)
    {
        float dist = pow (associatedPoints_[i]->x_ - p.x_, 2) + pow (associatedPoints_[i]->y_ - p.y_, 2);
        std::list<std::pair <Color, float> >::iterator cur = pointSorted.begin ();
        std::list<std::pair <Color, float> >::iterator end = pointSorted.end ();

        for (; cur != end; cur++)
        {
            if (dist < cur->second)
                break;
        }
        pointSorted.insert (cur, std::pair<Color, float> (associatedPoints_[i]->color_, dist));
    }

    assert (pointSorted.size () >= 3);

    std::list<std::pair <Color, float> >::iterator cur = pointSorted.begin ();

    Color c0 = cur->first;
    float d0 = cur->second;
    cur++;

    Color c1 = cur->first;
    float d1 = cur->second;
    cur++;
    Color c2 = cur->first;
    float d2 = cur->second;

    float sommeDeProduitNormatif = d0 * d2 + d1 * d2 + d1 * d0;

    return Color (  c0.r_ * (d1 * d2) / sommeDeProduitNormatif + c1.r_ * (d0 * d2) / sommeDeProduitNormatif + c2.r_ * (d0 * d1) / sommeDeProduitNormatif,
                    c0.g_ * (d1 * d2) / sommeDeProduitNormatif + c1.g_ * (d0 * d2) / sommeDeProduitNormatif + c2.g_ * (d0 * d1) / sommeDeProduitNormatif,
                    c0.b_ * (d1 * d2) / sommeDeProduitNormatif + c1.b_ * (d0 * d2) / sommeDeProduitNormatif + c2.b_ * (d0 * d1) / sommeDeProduitNormatif);
}


/*
std::ostream& operator<<( std::ostream &flux, Face* const& f)
{
    flux << "X";
    return flux;
}
*/
