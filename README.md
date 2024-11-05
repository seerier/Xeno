# Xeno Renderer
Hi! This is my own physically based renderer. It's name comes from my favorite video game, Xenoblade:)

## Building the code
### Cloning:
```bash
git clone https://github.com/seerier/Xeno.git
```

### Building:
Xeno uses [cmake](http://www.cmake.org/) for its build system

## Usage
To render a scene, use the following command:
```bash
xeno -f scene.json
```
where filepath is the absolute path to the json scene file.

You can alse use:
```bash
xeno -help
```
for more information.

## Selected Images
![Render 1](gallery/sphere-caustics-MISPathTracer1000_1000-2048spp-depth10.png)
![render 2](gallery/veach-bidir-MISPathTracer1024_1024-2048spp-depth10.png)
