# Custom Scene Exporter

Custom scene exporter to export Unity created scenes into other engines and frameworks.
![image](https://user-images.githubusercontent.com/1466920/152702332-dce4997b-219e-4ef6-bcf1-7001027dad03.png)

- Exports scenes from Unity to custom scene format
- Imports exported scenes into Unity
- Examples for raylib, MonoGame and Irrlicht using exported custom scene format
- Rotations are exported in degrees (degree to radiant conversion: `rot.x * PI/180`) or quaternion
- Default assets folder for other engines is `assets`
- Magic number `lvl1`
- Format: 
    - `model`: Model filename. If gameobject has no model it is null (for lights and cameras for example)
    - `name`: Name of the gameobject
    - `tag`: Tag of the gameobject
    - `position`: Gameobject position xyz
    - `rotation`: Gameobject rotation **in degrees** xyz or quaternion
    - `scale`: Gameobject scale xyz

## File Example
```c
lvl1
# model name tag pos rot scale
cube.obj,cube,tag,24.5,0,7.333,0,2,0,1,1,1
star.obj,star,player,55.3,4,8.333,0,12,0,1,1,1
ball.obj,ball,enemy,3.4,4,128.3,0,12.5,0,1,1,1

```