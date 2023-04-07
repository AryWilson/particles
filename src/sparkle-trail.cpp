// Bryn Mawr College, alinen, 2020
// edited: awilson, April 2023
//

/* implements an object pool and an emmited using billboards
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

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float rot;
  float size;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }
  // set up object pool
  void setup() {
    setWindowSize(1000, 1000);
    createConfetti(50);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  //initialize objects in pool
  void createConfetti(int size)
  {
    renderer.loadTexture("particle", "../textures/star4.png", 0);
    for (int i = 0; i < size; i++)
    {
      Particle particle;
      particle.color = vec4(agl::randomUnitCube(), 1);
      particle.size = 0.25;
      particle.rot = 0.0;
      particle.pos = agl::randomUnitCube();
      particle.vel = agl::randomUnitCube();
      mParticles.push_back(particle);
    }
  }

  /* creates one new on-screen particle if object pool has invisible particle
  * updates color, size, rotation, and position of all other particles in pool
  */
  void updateConfetti(float dt)
  {
    bool one = agl::random() > 0.5;

    for (int i = 0; i < mParticles.size(); i++){
      if(one && mParticles[i].color.w <=0){
        //one new particle
        mParticles[i].color = vec4(agl::randomUnitCube(), 1.0);
        mParticles[i].size = 0.25;
        mParticles[i].rot = agl::random();
        mParticles[i].pos = position;
        mParticles[i].vel = -velocity;
        one = false;

      } else{
        // updates the transparency
        mParticles[i].color.w -= dt;
        // updates the size
        mParticles[i].size += 0.25*dt;
        // updates the rotation
        mParticles[i].rot += dt;
        // updates the pos by vel
        mParticles[i].pos += dt*mParticles[i].vel;
      }

    }
  }

  // render all sprites in pool
  void drawConfetti()
  {
    renderer.texture("image", "particle");
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
    }
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

  void keyUp(int key, int mods) {
  }

  // update global velocity and position. call shader
  void draw() {
    // renderer.blendMode(agl::ADD); //done in setup
    renderer.beginShader("sprite");
    // renderer.texture("image","particle"); // done in draw

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);


    // send start in circle relative to current time

    // current velocity dependent on deltaTime since last frame
    // change in position from last frame position to current fram position
    ct = fmod((elapsedTime()),4*M_PI);
    float dx = cos(dt()) + agl::random() - 0.5f;
    float dy = sin(dt()) + agl::random() - 0.5f;
    velocity = vec3(dx,dy,0);
    
    // calculate new positon for star particle
    position.x = cos(ct);
    position.y = sin(ct); 

    renderer.sprite(position, vec4(1.0f), 0.25f);

    updateConfetti(dt());
    drawConfetti();
    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);
  vec3 velocity = vec3(0, 0, 0);
  float ct = 0;

  std::vector<Particle> mParticles;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
