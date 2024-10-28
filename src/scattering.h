#pragma once

#include "vecmath.h"
#include <complex>

namespace xeno {


inline float FrComplex(float cosTheta_i, std::complex<float> eta) {
    using Complex = std::complex<float>;
    cosTheta_i = clamp(cosTheta_i, 0, 1);
    // Compute complex $\cos\,\theta_\roman{t}$ for Fresnel equations using Snell's law
    float sin2Theta_i = 1 - square(cosTheta_i);
    Complex sin2Theta_t = sin2Theta_i / square(eta);
    Complex cosTheta_t = std::sqrt(1 - sin2Theta_t);

    Complex r_parl = (eta * cosTheta_i - cosTheta_t) / (eta * cosTheta_i + cosTheta_t);
    Complex r_perp = (cosTheta_i - eta * cosTheta_t) / (cosTheta_i + eta * cosTheta_t);
    return (std::norm(r_parl) + std::norm(r_perp)) / 2;
}

inline Spectrum FrComplex(float cosTheta_i, Spectrum eta, Spectrum k) {
    Spectrum result;
    for (int i = 0; i < 3; ++i)
        result[i] = FrComplex(cosTheta_i, std::complex<float>(eta[i], k[i]));
    return result;
}



inline float Cos2Theta(Vector3f w) {
    return square(w.z);
}


inline float Sin2Theta(Vector3f w) {
    return std::max<float>(0, 1 - Cos2Theta(w));
}

inline float Tan2Theta(Vector3f w) {
    return Sin2Theta(w) / Cos2Theta(w);
}

inline float CosPhi(Vector3f w) {
    float sinTheta = SinTheta(w);
    return (sinTheta == 0) ? 1 : clamp(w.x / sinTheta, -1, 1);
}
inline float SinPhi(Vector3f w) {
    float sinTheta = SinTheta(w);
    return (sinTheta == 0) ? 0 : clamp(w.y / sinTheta, -1, 1);
}



// TrowbridgeReitzDistribution Definition
class TrowbridgeReitzDistribution {
  public:
    // TrowbridgeReitzDistribution Public Methods
    TrowbridgeReitzDistribution() = default;
    TrowbridgeReitzDistribution(float ax, float ay)
        : alpha_x(ax), alpha_y(ay) {
        if (!EffectivelySmooth()) {
            // If one direction has some roughness, then the other can't
            // have zero (or very low) roughness; the computation of |e| in
            // D() blows up in that case.
            alpha_x = std::max<float>(alpha_x, 1e-4f);
            alpha_y = std::max<float>(alpha_y, 1e-4f);
        }
    }

    inline float D(Vector3f wm) const {
        float tan2Theta = Tan2Theta(wm);
        if (std::isinf(tan2Theta))
            return 0;
        float cos4Theta = square(Cos2Theta(wm));
        if (cos4Theta < 1e-16f)
            return 0;
        float e = tan2Theta * (square(CosPhi(wm) / alpha_x) + square(SinPhi(wm) / alpha_y));
        return 1 / (Pi * alpha_x * alpha_y * cos4Theta * square(1 + e));
    }

    bool EffectivelySmooth() const { return std::max(alpha_x, alpha_y) < 1e-3f; }

    float G1(Vector3f w) const { return 1 / (1 + Lambda(w)); }

    float Lambda(Vector3f w) const {
        float tan2Theta = Tan2Theta(w);
        if (IsInf(tan2Theta))
            return 0;
        float alpha2 = square(CosPhi(w) * alpha_x) + square(SinPhi(w) * alpha_y);
        return (std::sqrt(1 + alpha2 * tan2Theta) - 1) / 2;
    }

    float G(Vector3f wo, Vector3f wi) const { return 1 / (1 + Lambda(wo) + Lambda(wi)); }

    float D(Vector3f w, Vector3f wm) const {
        return G1(w) / std::abs(w.z) * D(wm) * absDot(w, wm);
    }

    float PDF(Vector3f w, Vector3f wm) const { return D(w, wm); }

    Vector3f Sample_wm(Vector3f w, Point2f u) const {
        // Transform _w_ to hemispherical configuration
        Vector3f wh = normalize(Vector3f(alpha_x * w.x, alpha_y * w.y, w.z));
        if (wh.z < 0)
            wh = -wh;

        // Find orthonormal basis for visible normal sampling
        Vector3f T1 = (wh.z < 0.99999f) ? normalize(cross(Vector3f(0, 0, 1), wh))
                                        : Vector3f(1, 0, 0);
        Vector3f T2 = cross(wh, T1);

        // Generate uniformly distributed points on the unit disk
        Point2f p = SampleUniformDiskPolar(u);

        // Warp hemispherical projection for visible normal sampling
        float h = std::sqrt(1 - square(p.x));
        p.y = lerp((1 + wh.z) / 2, h, p.y);

        // Reproject to hemisphere and transform normal to ellipsoid configuration
        float pz = std::sqrt(std::max<float>(0, 1 - lengthSquared(Vector2f(p))));
        Vector3f nh = p.x * T1 + p.y * T2 + pz * wh;
        CHECK_RARE(1e-5f, nh.z == 0);
        return normalize(
            Vector3f(alpha_x * nh.x, alpha_y * nh.y, std::max<float>(1e-6f, nh.z)));
    }

    std::string ToString() const;

    static float RoughnessToAlpha(float roughness) { return std::sqrt(roughness); }

    void Regularize() {
        if (alpha_x < 0.3f)
            alpha_x = clamp(2 * alpha_x, 0.1f, 0.3f);
        if (alpha_y < 0.3f)
            alpha_y = clamp(2 * alpha_y, 0.1f, 0.3f);
    }

  private:
    // TrowbridgeReitzDistribution Private Members
    float alpha_x, alpha_y;
};

}  // namespace xeno
