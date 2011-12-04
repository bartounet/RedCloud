#include <math.h>
#include <fstream>
#include <assert.h>

#include "Texture.h"


/** @brief createMapping
  *
  * @todo: document this function
  */
void Texture::createMapping()
{
    sizeTriangle_ = (-5 + sqrt(9 + 4 * pow((float)width_, 2) / faces_.size ())) / 2;

    std::cout << "size triangle : " << sizeTriangle_ << std::endl;

    int l = 0;
    int c = 0;


    for (size_t i = 0; faces_.size () > i; i++)
    {
        if (i % 2 == 0)
        {
            Point2d* p0 = new Point2d(0.5 + c * (sizeTriangle_ + 4), 0.5 + l * (sizeTriangle_ + 1));
            Point2d* p1 = new Point2d(p0->x_, p0->y_ + sizeTriangle_);
            Point2d* p2 = new Point2d(p0->x_ + sizeTriangle_, p0->y_);
            faces_[i]->setCoordTexture(p0, p1, p2);

            assert (p0->x_ >= 0);
            assert (p0->x_ <= img_.size ());
            assert (p0->y_ >= 0);
            assert (p0->y_ <= img_[0].size ());
            assert (p1->x_ >= 0);
            assert (p1->x_ <= img_.size ());
            assert (p1->y_ >= 0);
            assert (p1->y_ <= img_[0].size ());
            assert (p2->x_ >= 0);
            assert (p2->x_ <= img_.size ());
            assert (p2->y_ >= 0);
            assert (p2->y_ <= img_[0].size ());
            assert (p1->y_ - p0->y_ == sizeTriangle_);
            assert (p2->x_ - p0->x_ == sizeTriangle_);

        }
        else
        {
            Point2d* p0 = new Point2d(sizeTriangle_ + 4 - 0.5 + c * (sizeTriangle_ + 4), sizeTriangle_ + 1 - 0.5 + l * (sizeTriangle_ + 1));
            Point2d* p1 = new Point2d(p0->x_, p0->y_ - sizeTriangle_);
            Point2d* p2 = new Point2d(p0->x_ - sizeTriangle_, p0->y_);
            faces_[i]->setCoordTexture(p0, p1, p2);

            assert (p0->x_ >= 0);
            assert (p0->x_ <= img_.size ());
            assert (p0->y_ >= 0);
            assert (p0->y_ <= img_[0].size ());
            assert (p1->x_ >= 0);
            assert (p1->x_ <= img_.size ());
            assert (p1->y_ >= 0);
            assert (p1->y_ <= img_[0].size ());
            assert (p2->x_ >= 0);
            assert (p2->x_ <= img_.size ());
            assert (p2->y_ >= 0);
            assert (p2->y_ <= img_[0].size ());
            assert (p0->y_ - p1->y_ == sizeTriangle_);
            assert (p0->x_ - p2->x_ == sizeTriangle_);

            c++;
            if ((c + 1) * (sizeTriangle_ + 4) > width_)
            {
                c = 0;
                l++;
            }
        }
    }
}

/** @brief Texture
  *
  * @todo: document this function
  */
Texture::Texture(size_t w, std::vector<Face* >& faces)
        : width_ (w),
        heigth_ (w),
        faces_ (faces)
{
    assert (w % 4 == 0);

    std::cout << "texture size : " << width_ << "x" << heigth_ << std::endl;

    for (size_t i = 0; i < width_; i++)
    {
        img_.push_back (std::vector<Color > ());
        for (size_t j = 0; j < heigth_; j++)
        {
            img_[i].push_back (Color (0, 0, 0));
        }
    }
}

/** @brief getSizeTriangle
  *
  * @todo: document this function
  */

size_t Texture::getSizeTriangle()
{
    return sizeTriangle_;
}



/** @brief fill
  *
  * @todo: document this function
  */
void Texture::fill()
{
    for (size_t f = 0; f < faces_.size (); f++)
    {
        Point2d vectU = Point2d(faces_[f]->getCoordTexture ()[1]->x_ - faces_[f]->getCoordTexture ()[0]->x_,
                                faces_[f]->getCoordTexture ()[1]->y_ - faces_[f]->getCoordTexture ()[0]->y_);
        Point2d vectV = Point2d(faces_[f]->getCoordTexture ()[2]->x_ - faces_[f]->getCoordTexture ()[0]->x_,
                                faces_[f]->getCoordTexture ()[2]->y_ - faces_[f]->getCoordTexture ()[0]->y_);




        for (float i = 0; i <= sizeTriangle_; i += 1)
            for (float j = 0; j <= sizeTriangle_; j += 1)
            {
                if (i + j <= sizeTriangle_ + 1)
                {
                    float u = i / sizeTriangle_;
                    float v = j / sizeTriangle_;

                    float x = faces_[f]->getCoordTexture ()[0]->x_ + u * vectU.x_ + v * vectV.x_;
                    float y = faces_[f]->getCoordTexture ()[0]->y_ + u * vectU.y_ + v * vectV.y_;

                    img_[(int)floor(x)][(int)floor(y)] = faces_[f]->getApproxColor (Point2d (u, v));
                }
            }
    }
/*
    for (size_t f = 0; f < faces_.size (); f++)
    {
        Point2d vectU = Point2d(faces_[f]->getCoordTexture ()[1]->x_ - faces_[f]->getCoordTexture ()[0]->x_,
                                faces_[f]->getCoordTexture ()[1]->y_ - faces_[f]->getCoordTexture ()[0]->y_);
        Point2d vectV = Point2d(faces_[f]->getCoordTexture ()[2]->x_ - faces_[f]->getCoordTexture ()[0]->x_,
                                faces_[f]->getCoordTexture ()[2]->y_ - faces_[f]->getCoordTexture ()[0]->y_);



        std::vector<Point2dColor* > assocPoints = faces_[f]->getAssociatedPoints ();

        for (int i = 3; i < assocPoints.size (); i++)
        {
              float x = faces_[f]->getCoordTexture ()[0]->x_ + assocPoints[i]->x_ * vectU.x_ + assocPoints[i]->y_ * vectV.x_;
              float y = faces_[f]->getCoordTexture ()[0]->y_ + assocPoints[i]->x_ * vectU.y_ + assocPoints[i]->y_ * vectV.y_;
            img_[(int)floor(x)][(int)floor(y)] = Color (255, 0, 0);

        }
    }

*/
}

/** @brief dump
  *
  * @todo: document this function
  */
bool Texture::dump(std::string filename)
{
    std::ofstream fichier(filename.c_str (), std::ios::out | std::ios::trunc | std::ios::binary);

    if (fichier)
    {
        fichier << "P6" << std::endl;
        fichier << img_.size() << " " << img_[0].size () << std::endl;
        fichier << "255" << ' ';
        for (size_t j = 0; j < img_[0].size (); j++)
            for (size_t i = 0; i < img_.size (); i++)
                fichier << img_[i][j].r_ << img_[i][j].g_ << img_[i][j].b_;

        fichier.close();
        return true;
    }
    return false;
}

