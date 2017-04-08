/**
 *    \file moonBoard.h
 *    \brief plateau de jeu
 *
 *    \author Nicolas SOBCZAK
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________

//TODO: replace limit by the right dimensions
#define x_limit 1000
#define y_limit 1000


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
