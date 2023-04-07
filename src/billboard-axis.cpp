// Bryn Mawr College, alinen, 2020
// edited: awilson, April 2023


/* implements a tree billboard that rotates around the y axis based on user input
*/
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
  
  /* load the textures, get the dimentions for the textures*/
  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("simple-texture",
      "../shaders/simple-texture.vs",
      "../shaders/simple-texture.fs");

    Image img;
    img.load("../textures/tree.png", true);
    renderer.loadTexture("tree", img, 0);

    // set the global image dimentions 
    imgW = img.width();
    imgH = img.height();

    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);

    radius = 10;
    azimuth = 0;
    elevation = 0;
  }

  // UI allows for rotation of billboard
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

  // UI allows for zooming in and out on bilboard
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

  /* draw the billboard and plane with updated model matrix in response to user input*/
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

    
    // float d = std::max(imgW,imgH);
    // scale to correct size, unit cube
    // renderer.scale(vec3(1.0f/d, 1.0f/d, 1.0f));
    // renderer.scale(vec3(imgW, imgH, 1.0f));
    //rotate to follow camera
    vec3 n = normalize(eyePos);
    float theta = atan2(-n.z,n.x)+M_PI_2;

    renderer.push();
    renderer.rotate(theta,vec3(0,1,0));

    // Use the width and the height of the image to scale the billboard
    renderer.scale(vec3(imgW/imgH, imgH/imgH, 1.0f));
    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    renderer.pop();


  

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
