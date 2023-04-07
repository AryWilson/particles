// Bryn Mawr College, alinen, 2020
// edited: awilson, April 2023

/*uses a shader to use a sprite board as part of an animation. Sprites are animated prior to the shader being used.*/

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

  /* load the spite texture and the shader that implements frame changes*/
  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("billboard-animated", 
      "../shaders/billboard-animated.vs", 
      "../shaders/billboard-animated.fs");

    renderer.loadTexture("fire", 
      "../textures/ParticleFlamesSheet.png", 0);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }


  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
    eyePos.z += dy;
  }

  /* calculate the change in frame rate, 
  * update the opacity, 
  * update the position,
  * call the shader*/
  void draw() {
    renderer.beginShader("billboard-animated");
    renderer.texture("image", "fire");

    // 30 fps => each frame 1/30 long, e.g. when time = 1s, we play frame 30
    // frame = round(((clock()%1000)/1000.0f)*30.0f); // 30 sprites
    float c = clock();
    frame = round(((c)/1000.0f)); // infinite sprites not 30 fps
    if(oldT + 0.5 < elapsedTime()){
        // every half second, the sprite chanegd direction
      bol = -1.0f*bol;
      oldT = elapsedTime();
    } 
    upPos.y += bol*dt();

    // every time the frame changes, reset the alpha, otherwise decrease alpha
    if(fmod(c,1000)<=10){
      alpha=1.0f;
    } else {
      alpha -= 0.05f;
    }

    renderer.setUniform("Frame", frame);
    renderer.setUniform("Rows", numRows);
    renderer.setUniform("Cols", numCols);
    renderer.setUniform("topDown", true);


    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    renderer.sprite(upPos, vec4(vec3(1.0f),alpha), 1.0);

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 upPos = vec3(0, 0, 0);
  int frame = 0;
  float oldT = 0;
  int numRows = 4;
  int numCols = 8;
  float bol = 0.25;
  float alpha = 1.0;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
