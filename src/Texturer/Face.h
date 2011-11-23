#ifndef FACE_H_INCLUDED
#define FACE_H_INCLUDED

# include <vector>
# include <iostream>
# include "Point3d.h"
# include "Point3dColor.h"
# include "Point2d.h"
# include "Point2dColor.h"
# include "Plan.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class Face
{
public:
	Face (int p1, int p2, int p3);

public:
	static void printInfos (std::vector<Face* >& faces);

public:
    static std::vector<Point3dColor* > points_;

public:
	void associatePoint (Point2dColor* p);

public:
    void setCoordTexture(Point2d* p0, Point2d* p1, Point2d* p2);
    Point2d** getCoordTexture();
    Point3d& getCentroid ();
    Plan& getPlan ();
    Plan& getPlanAN ();
    Plan& getPlanBN ();
    Point3d* getVectA ();
    Point3d* getVectB ();
    Point3d* getOrigine ();
    std::vector<Point2dColor* >& getAssociatedPoints ();
    Color getApproxColor (Point2d p);

public:
    int p0_;
    int p1_;
    int p2_;

private:
    Point3d* o_;
    Point3d* a_;
    Point3d* b_;
    Point2d* coordTexture_[3];
    std::vector<Point2dColor* > associatedPoints_;
    Plan plan_;
    Plan an_;
    Plan bn_;
    Point3d centroid_;
};
// ----------------------------------------------------------------------------
#if 0
std::ostream& operator<<(std::ostream &flux, Face* const& f);
#endif
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif // FACE_H_INCLUDED
