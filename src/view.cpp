#include "window.hpp"
#include <iostream>
#include <math.h>

#define deg2rad (M_PI / 180.0f) 
#define rad2deg (180.0f / M_PI) 

using namespace Eigen;
typedef sdlWindow::mat4 mat4; 

void sdlWindow::_view_mvp_assign(GLuint MVP) {
  mat4 mvp = proj * model;
  glUniformMatrix4fv(MVP, 1, GL_FALSE, mvp.data());
}

void sdlWindow::_view_mvp_assign(GLuint MV, GLuint P) {
  glUniformMatrix4fv(MV, 1, GL_FALSE, model.data());
  glUniformMatrix4fv(P, 1, GL_FALSE, proj.data());
}

void sdlWindow::_view_mvp_init() {
  mat4 transform = _view_transform(vec3(0, 0, -10), quat::Identity(), vec3(1, 1, 1));
  model = _view_lookat({0, 2, 0}, {0, 0, -10}, {0, 1, 0}) * transform;
  proj = _view_prespective(60, (GLfloat)width/(GLfloat)height, 0.1, 100);
}

void sdlWindow::_view_mvp_fresh() {
  _view_mvp_init();
}

sdlWindow::mat4 sdlWindow::_view_transform(
    sdlWindow::vec3 t, sdlWindow::quat r, sdlWindow::vec3 s)
{
    mat4 transform;
    transform <<
      s.x(), 0, 0, t.x(),
      0, s.y(), 0, t.y(),
      0, 0, s.z(), t.z(),
      0,   0,   0,     1;

    mat4 rotation = mat4::Identity();
    rotation.block(0, 0, 3, 3) = r.toRotationMatrix();

    return rotation * transform;
}


/* credit from:
 * http://spointeau.blogspot.com/2013/12/hello-i-am-looking-at-opengl-3.html
 */
sdlWindow::mat4 sdlWindow::_view_prespective(
    GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    typedef sdlWindow::mat4 mat4; 
    assert(aspect > 0);
    assert(zFar > zNear);

    GLfloat radf = deg2rad * fovy;

    GLfloat tanHalfFovy = tan(radf / 2.0);
    mat4 res = mat4::Zero();
    res(0,0) = 1.0 / (aspect * tanHalfFovy);
    res(1,1) = 1.0 / (tanHalfFovy);
    res(2,2) = - (zFar + zNear) / (zFar - zNear);
    res(3,2) = - 1.0;
    res(2,3) = - (2.0 * zFar * zNear) / (zFar - zNear);
    return res;
}

/* credit from:
 * http://spointeau.blogspot.com/2013/12/hello-i-am-looking-at-opengl-3.html
 */
sdlWindow::mat4 sdlWindow::_view_lookat(
    sdlWindow::vec3 eye, 
    sdlWindow::vec3 center, 
    sdlWindow::vec3 up
)
{
    typedef sdlWindow::mat4 mat4; 
    typedef sdlWindow::vec3 vec3; 

    vec3 f = (center - eye).normalized();
    vec3 u = up.normalized();
    vec3 s = f.cross(u).normalized();
    u = s.cross(f);

    mat4 res;
    res <<  s.x(),s.y(),s.z(),-s.dot(eye),
            u.x(),u.y(),u.z(),-u.dot(eye),
            -f.x(),-f.y(),-f.z(),f.dot(eye),
            0,0,0,1;

    return res;
}

