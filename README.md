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
![Render 1](gallery/sphere-dielectric.png)


## References
[1] Physically Based Rendering: From Theory to Implementation: https://www.pbrt.org/
[2] The Rendering Equation: https://www.cs.cmu.edu/afs/cs/academic/class/15462-s13/www/lec_slides/86kajiyaRenderingEquation.pdf
[3] Robust Monte Carlo Methods For Light Transport Simulation: https://graphics.stanford.edu/papers/veach_thesis/thesis.pdf
[4] Global Illumination using Photon Maps: https://graphics.stanford.edu/~henrik/papers/ewr7/egwr96.pdf 
[5] Stochastic Progressive Photon Mapping: http://graphics.ucsd.edu/~henrik/papers/stochastic_progressive_photon_mapping.pdf
[6] Metropolis Light Transport: https://graphics.stanford.edu/papers/metro/metro.pdf
