# Xeno Renderer

A physically-based renderer implementing Monte-Carlo light transport algorithms.

## Features

- Path tracing with importance sampling and multiple importance sampling (MIS)
- Bidirectional path tracing
- Stochastic Progressive Photon Mapping
- Multithreaded rendering for efficient performance

## Building

Xeno uses [cmake](http://www.cmake.org/) for its build system.
```bash
git clone https://github.com/seerier/Xeno.git
cd Xeno
mkdir build && cd build
cmake ..
make -j8
```

## Usage

To render a scene:
```bash
xeno -f scene.json
```

For more information:
```bash
xeno -help
```

## Selected Images
![Render 1](gallery/sphere-dielectric.png)


## References

[1] [Physically Based Rendering: From Theory to Implementation](https://www.pbrt.org/)

[2] [The Rendering Equation](https://www.cs.cmu.edu/afs/cs/academic/class/15462-s13/www/lec_slides/86kajiyaRenderingEquation.pdf)

[3] [Robust Monte Carlo Methods For Light Transport Simulation](https://graphics.stanford.edu/papers/veach_thesis/thesis.pdf)

[4] [Global Illumination using Photon Maps](https://graphics.stanford.edu/~henrik/papers/ewr7/egwr96.pdf)

[5] [Stochastic Progressive Photon Mapping](http://graphics.ucsd.edu/~henrik/papers/stochastic_progressive_photon_mapping.pdf)

[6] [Metropolis Light Transport](https://graphics.stanford.edu/papers/metro/metro.pdf)
