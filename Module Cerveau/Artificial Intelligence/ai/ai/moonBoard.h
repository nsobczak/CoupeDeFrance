/**
 *    \file moonBoard.h
 *    \brief plateau de jeu
 *
 *    \author Nicolas SOBCZAK
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________

//TODO: replace limit by the right dimensions
#define _X_LIMIT_ 3.0
#define _Y_LIMIT_ 2.0

struct CylinderPosition
{
        int cylinder_id;
        float cylinder_x;
        float cylinder_y;
        CylinderPosition() : cylinder_id(0), cylinder_x(0), cylinder_y(0){
        }
        CylinderPosition(int id, float x, float y) : cylinder_id(id), cylinder_x(x), cylinder_y(y){
        }
};

struct MoonBase
{
        int moonBase_id;
        float moonBase_x1;
        float moonBase_y1;
        float moonBase_x2;
        float moonBase_y2;
        float moonBase_x3;
        float moonBase_y3;
        float moonBase_x4;
        float moonBase_y4;
        MoonBase() : moonBase_id(0), moonBase_x1(0), moonBase_y1(0), moonBase_x2(0), moonBase_y2(0),
                moonBase_x3(0), moonBase_y3(0), moonBase_x4(0), moonBase_y4(0){
        }
        MoonBase(int id, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) :
                moonBase_id(id), moonBase_x1(x1), moonBase_y1(y1), moonBase_x2(x2), moonBase_y2(y2),
                moonBase_x3(x3), moonBase_y3(y3), moonBase_x4(x4), moonBase_y4(y4){
        }
};
