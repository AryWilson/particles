# particles

Implements particle demos

![Alt text](/images/sparkles.gif?raw=true)

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ start particles.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
particles/build $ ../bin/Debug/billboard-axis.exe
particles/build $ ../bin/Debug/explosion.exe
particles/build $ ../bin/Debug/sparkle-trail.exe
particles/build $ ../bin/Debug/sparkles.exe
particles/build $ ../bin/Debug/demo.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ make
```

To run each program from build, you would type

```
particles/build $ ../bin/billboard-axis
particles/build $ ../bin/sparkles
particles/build $ ../bin/sparkle-trail
particles/build $ ../bin/explosion
particles/build $ ../bin/demo
```

## Demo of basic features

Single axis billboard implemented with a shader.
![Alt text](/images/tree.gif?raw=true)

Animated explotion implemented with a sprite board.
![Alt text](/images/sprite.gif?raw=true)

Particle that moves around in a circle with a trail that moves tangentially off of it's path. Implemented with an object pool.
![Alt text](/images/trail.gif?raw=true)



## Unique features 

An animated sprite. The fire sprite moves up an down every few seconds. Every time the frame changes the opacity of the spite resets to 1. Otherwise the sprite looses opacity (fades). To account for the blurriness of the animation, the framerate is slower.
![Alt text](/images/fire.gif?raw=true)


