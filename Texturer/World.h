#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

# include <vector>
# include <list>
# include "Point3d.h"
# include "Point3dColor.h"
# include "Face.h"

class World
{
  private:
    std::vector<Point3dColor* >& points_;
    std::vector<std::vector<std::vector<std::vector<Face* > > > > faces_;
    float gridSize_;
    float maxFaceDist_;
    Point3d& min_;
    Point3d& max_;

    std::list<Face*>* getNeighbour (Point3d& p);
    Point2dColor* projectionOnTriangle(Point3dColor& point, Face& face);
    std::vector<Face*>& getAtIndex(float x, float y, float z);
  public:
    World (std::vector<Point3dColor* >& points, std::vector<Face* >& faces, Point3d& min, Point3d& max);
    bool associatePoints ();
};



#endif // WORLD_H_INCLUDED
