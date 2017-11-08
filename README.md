# 3D Maze
### Creating a 3D maze with raycasting
![textured_1](https://github.com/hicarrie/maze/blob/master/screenshots/textured_1.png)
-----

### How to Compile and Run
- [SDL2](https://www.libsdl.org/download-2.0.php) is required to compile and use this program
- Compile with `gcc 4.8.4` with the following flags:
    ``gcc -O2 -g -w -Isrc/headers *.c -lSDL2 -lSDL2_image -lm -o maze `sdl2-config --cflags --libs` `` OR `make -f Makefile`
- Run the maze: `./maze maps/<map_name>`

-----

### Synopsis
This 3D maze uses raycasting to draw the maze walls, utilizing [LodeV's](http://lodev.org/cgtutor/raycasting.html) method of using vectors to calculate ray length. Currently the maze has flat colors; textured walls are a work in progress.

#### Controls
- `W` : move forward
- `S` : move backward
- `A` : rotate camera left
- `D` : rotate camera right
- `Q` : strafe left
- `E` : strafe right
- `F` : toggle fullscreen
- `ESC` : quit

![untextured_3](https://github.com/hicarrie/maze/blob/master/screenshots/textured_3.png)

#### Maps
The maps are defined in 2D arrays in text files, which are parsed when passed as an argument to the maze executable. `0` represents open space, all other integers are drawn as walls.

Example:
```
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 2 2 2 2 2 0 0 0 0 3 0 3 0 3 0 0 0 1
1 0 0 0 0 0 2 0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 2 0 0 0 2 0 0 0 0 3 0 0 0 3 0 0 0 1
1 0 0 0 0 0 2 0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 2 2 0 2 2 0 0 0 0 3 0 3 0 3 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 4 4 4 4 4 4 4 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 4 0 4 0 0 0 0 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 4 0 0 0 0 5 0 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 4 0 4 0 0 0 0 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 4 0 4 4 4 4 4 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 4 4 4 4 4 4 4 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
```

### Upcoming Features
- More robust map parser
- More textures
- Enemies / obstacles
- Maze goal that loads next map
- Rain

### Resources
- [SDL2 API](https://wiki.libsdl.org/CategoryAPI)
- [LazyFoo Beginning Game Programming](http://lazyfoo.net/tutorials/SDL/index.php)
- [Ray-Casting Tutorial For Game Development And Other Purposes by F. Permadi](http://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/)
- [LodeV Raycasting Tutorial](http://lodev.org/cgtutor/raycasting.html)
- [Game Engine Black Book](https://www.amazon.com/Game-Engine-Black-Book-Wolfenstein/dp/1539692876)