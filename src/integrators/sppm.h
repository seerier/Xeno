#include"integrator.h"

namespace xeno {

class SPPM : public Integrator {
public:
    SPPM(int nIterations, int photonsPerIteration, int maxDepth = 20, float radius = 100.f) :
        photonsPerIteration(photonsPerIteration), nIterations(nIterations), maxDepth(maxDepth), radius(radius) {}

    void Render(Sensor &sensor, const Scene &scene) const override;


private:
    int photonsPerIteration;
    int nIterations;
    int maxDepth;
    float radius;
};

} // namespace xeno