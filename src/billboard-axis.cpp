// Bryn Mawr College, alinen, 2020
//
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <vector>
#include <time.h> 
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("simple-texture",
      "../shaders/simple-texture.vs",
      "../shaders/simple-texture.fs");

    Image img;
    img.load("../textures/tree.png", true);
    renderer.loadTexture("tree", img, 0);
    // TODO: Use the width and the height of the image to scale the billboard
    imgW = img.width();
    imgH = img.height();

    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);

    radius = 10;
    azimuth = 0;
    elevation = 0;
  }


  void mouseMotion(int x, int y, int dx, int dy) {
    if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)) {
        azimuth -= dx*(0.02f);
        azimuth = fmod(azimuth,2*M_PI);

        elevation += dy*(0.02f);
        // elevation += M_PI_2;
        // elevation = fmod(elevation,M_PI);
        // elevation -= M_PI_2;

        if(elevation>=M_PI_2){elevation = M_PI_2 - 0.01f;}
        if(elevation<=-1*M_PI_2){elevation = -1*M_PI_2 + 0.01f;}

         
        float x = radius * sin(azimuth) * cos(elevation);
        float y = radius * sin(elevation);
        float z = radius * cos(azimuth) * cos(elevation);
        eyePos = vec3(x,y,z);
         
      } 
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
      radius+=dy;
    if(radius<=0){
      radius = 1;
    }

    float x = radius * sin(azimuth) * cos(elevation);
    float y = radius * sin(elevation);
    float z = radius * cos(azimuth) * cos(elevation);
    eyePos = vec3(x,y,z);
  }

  void draw() {
    renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);

    // draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();

    // draw tree
    renderer.texture("Image", "tree");
    renderer.push();
    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    renderer.pop();

    
    float d = std::max(imgW,imgH);
    //scale to correct size, unit cube
    renderer.scale(vec3(1.0f/d, 1.0f/d, 1.0f));
    //rotate to follow camera
    float theta = atan(eyePos.z/eyePos.x);
    renderer.rotate(theta,vec3(0,1,0));
  
  // I did some work on the particle hw, but i couldn't get the graphics envioroment set up on my computer so I couldn't test anything i wrote (usually i would go into the lab)

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  float imgW;
  float imgH;
  float radius;
  float azimuth;
  float elevation; 
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
