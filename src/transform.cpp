#include"transform.h"

namespace xeno {

// Matrix4x4 Method Definitions

Matrix4x4::Matrix4x4(float mat[4][4]) { memcpy(m, mat, 16 * sizeof(float)); }

Matrix4x4::Matrix4x4(float t00, float t01, float t02, float t03, float t10,
                     float t11, float t12, float t13, float t20, float t21,
                     float t22, float t23, float t30, float t31, float t32,
                     float t33) {
    m[0][0] = t00;
    m[0][1] = t01;
    m[0][2] = t02;
    m[0][3] = t03;
    m[1][0] = t10;
    m[1][1] = t11;
    m[1][2] = t12;
    m[1][3] = t13;
    m[2][0] = t20;
    m[2][1] = t21;
    m[2][2] = t22;
    m[2][3] = t23;
    m[3][0] = t30;
    m[3][1] = t31;
    m[3][2] = t32;
    m[3][3] = t33;
}

Matrix4x4 transpose(const Matrix4x4 &m) {
    return Matrix4x4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1],
                     m.m[1][1], m.m[2][1], m.m[3][1], m.m[0][2], m.m[1][2],
                     m.m[2][2], m.m[3][2], m.m[0][3], m.m[1][3], m.m[2][3],
                     m.m[3][3]);
}

Matrix4x4 inverse(const Matrix4x4 &m) {
    int indxc[4], indxr[4];
    int ipiv[4] = {0, 0, 0, 0};
    float minv[4][4];
    memcpy(minv, m.m, 4 * 4 * sizeof(float));
    for (int i = 0; i < 4; i++) {
        int irow = 0, icol = 0;
        float big = 0.f;
        // Choose pivot
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (std::abs(minv[j][k]) >= big) {
                            big = float(std::abs(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    }// else if (ipiv[k] > 1)
                     //   Error("Singular matrix in MatrixInvert");
                    else DCHECK(ipiv[k] <= 1) << "Singular matrix in MatrixInvert";
                }
            }
        }
        ++ipiv[icol];
        // Swap rows _irow_ and _icol_ for pivot
        if (irow != icol) {
            for (int k = 0; k < 4; ++k) std::swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        //if (minv[icol][icol] == 0.f) Error("Singular matrix in MatrixInvert");
        DCHECK(minv[icol][icol] != 0.f) << "Singular matrix in MatrixInvert";

        // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
        float pivinv = 1. / minv[icol][icol];
        minv[icol][icol] = 1.;
        for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

        // Subtract this row from others to zero out their columns
        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++) minv[j][k] -= minv[icol][k] * save;
            }
        }
    }
    // Swap columns to reflect permutation
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++)
                std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }
    return Matrix4x4(minv);
}

bool solveLinearSystem2x2(const float A[2][2], const float B[2], float *x0,
                          float *x1) {
    float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
    if (std::abs(det) < 1e-10f) return false;
    *x0 = (A[1][1] * B[0] - A[0][1] * B[1]) / det;
    *x1 = (A[0][0] * B[1] - A[1][0] * B[0]) / det;
    if (std::isnan(*x0) || std::isnan(*x1)) return false;
    return true;
}






// Transform Method Definitions
void Transform::print(FILE *f) const { m.print(f); }

Transform Transform::translate(const Vector3f &delta) {
    Matrix4x4 m(1, 0, 0, delta.x, 0, 1, 0, delta.y, 0, 0, 1, delta.z, 0, 0, 0,
                1);
    Matrix4x4 minv(1, 0, 0, -delta.x, 0, 1, 0, -delta.y, 0, 0, 1, -delta.z, 0,
                   0, 0, 1);
    return Transform(m, minv);
}

Transform Transform::scale(float x, float y, float z) {
    Matrix4x4 m(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
    Matrix4x4 minv(1 / x, 0, 0, 0, 0, 1 / y, 0, 0, 0, 0, 1 / z, 0, 0, 0, 0, 1);
    return Transform(m, minv);
}

Transform Transform::rotateX(float theta) {
    float sinTheta = std::sin(radians(theta));
    float cosTheta = std::cos(radians(theta));
    Matrix4x4 m(1, 0, 0, 0, 0, cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0,
                0, 0, 0, 1);
    return Transform(m, transpose(m));
}

Transform Transform::rotateY(float theta) {
    float sinTheta = std::sin(radians(theta));
    float cosTheta = std::cos(radians(theta));
    Matrix4x4 m(cosTheta, 0, sinTheta, 0, 0, 1, 0, 0, -sinTheta, 0, cosTheta, 0,
                0, 0, 0, 1);
    return Transform(m, transpose(m));
}

Transform Transform::rotateZ(float theta) {
    float sinTheta = std::sin(radians(theta));
    float cosTheta = std::cos(radians(theta));
    Matrix4x4 m(cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 1);
    return Transform(m, transpose(m));
}

Transform Transform::rotate(float theta, const Vector3f &axis) {
    Vector3f a = normalize(axis);
    float sinTheta = std::sin(radians(theta));
    float cosTheta = std::cos(radians(theta));
    Matrix4x4 m;
    // Compute rotation of first basis vector
    m.m[0][0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
    m.m[0][1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
    m.m[0][2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;
    m.m[0][3] = 0;

    // Compute rotations of second and third basis vectors
    m.m[1][0] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
    m.m[1][1] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
    m.m[1][2] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;
    m.m[1][3] = 0;

    m.m[2][0] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
    m.m[2][1] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
    m.m[2][2] = a.z * a.z + (1 - a.z * a.z) * cosTheta;
    m.m[2][3] = 0;
    return Transform(m, transpose(m));
}

Transform Transform::lookAt(const Point3f &pos, const Point3f &look, const Vector3f &up) {
    Matrix4x4 cameraToWorld;
    // Initialize fourth column of viewing matrix
    cameraToWorld.m[0][3] = pos.x;
    cameraToWorld.m[1][3] = pos.y;
    cameraToWorld.m[2][3] = pos.z;
    cameraToWorld.m[3][3] = 1;

    // Initialize first three columns of viewing matrix
    Vector3f dir = normalize(look - pos);
    
    //if (Cross(Normalize(up), dir).Length() == 0) {
    //    Error(
    //        "\"up\" vector (%f, %f, %f) and viewing direction (%f, %f, %f) "
    //        "passed to LookAt are pointing in the same direction.  Using "
    //        "the identity transformation.",
    //        up.x, up.y, up.z, dir.x, dir.y, dir.z);
    //    return Transform();
    //}
    DCHECK(cross(normalize(up), dir).length() != 0) <<
        "up vector and viewing direction "
        "passed to LookAt are pointing in the same direction.  Using "
        "the identity transformation.";


    Vector3f right = normalize(cross(normalize(up), dir));
    Vector3f newUp = cross(dir, right);
    cameraToWorld.m[0][0] = right.x;
    cameraToWorld.m[1][0] = right.y;
    cameraToWorld.m[2][0] = right.z;
    cameraToWorld.m[3][0] = 0.;
    cameraToWorld.m[0][1] = newUp.x;
    cameraToWorld.m[1][1] = newUp.y;
    cameraToWorld.m[2][1] = newUp.z;
    cameraToWorld.m[3][1] = 0.;
    cameraToWorld.m[0][2] = dir.x;
    cameraToWorld.m[1][2] = dir.y;
    cameraToWorld.m[2][2] = dir.z;
    cameraToWorld.m[3][2] = 0.;
    return Transform(inverse(cameraToWorld), cameraToWorld);
}

Transform Transform::cameraToWorld(const Point3f &pos, const Point3f &look, const Vector3f &up) {
    Matrix4x4 cameraToWorld;
    // Initialize fourth column of viewing matrix
    cameraToWorld.m[0][3] = pos.x;
    cameraToWorld.m[1][3] = pos.y;
    cameraToWorld.m[2][3] = pos.z;
    cameraToWorld.m[3][3] = 1;

    // Initialize first three columns of viewing matrix
    Vector3f dir = normalize(look - pos);
    
    //if (Cross(Normalize(up), dir).Length() == 0) {
    //    Error(
    //        "\"up\" vector (%f, %f, %f) and viewing direction (%f, %f, %f) "
    //        "passed to LookAt are pointing in the same direction.  Using "
    //        "the identity transformation.",
    //        up.x, up.y, up.z, dir.x, dir.y, dir.z);
    //    return Transform();
    //}
    DCHECK(cross(normalize(up), dir).length() != 0) <<
        "up vector and viewing direction "
        "passed to LookAt are pointing in the same direction.  Using "
        "the identity transformation.";


    Vector3f right = normalize(cross(normalize(up), dir));
    Vector3f newUp = cross(dir, right);
    cameraToWorld.m[0][0] = right.x;
    cameraToWorld.m[1][0] = right.y;
    cameraToWorld.m[2][0] = right.z;
    cameraToWorld.m[3][0] = 0.;
    cameraToWorld.m[0][1] = newUp.x;
    cameraToWorld.m[1][1] = newUp.y;
    cameraToWorld.m[2][1] = newUp.z;
    cameraToWorld.m[3][1] = 0.;
    cameraToWorld.m[0][2] = dir.x;
    cameraToWorld.m[1][2] = dir.y;
    cameraToWorld.m[2][2] = dir.z;
    cameraToWorld.m[3][2] = 0.;
    return Transform(cameraToWorld, inverse(cameraToWorld));
}

Transform Transform::orthographic(float zNear, float zFar) {
    return scale(1, 1, 1 / (zFar - zNear)) * translate(Vector3f(0, 0, -zNear));
}

Transform Transform::perspective(float fov, float n, float f) {
    // Perform projective divide for perspective projection
    Matrix4x4 persp(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, f / (f - n), -f * n / (f - n),
                    0, 0, 1, 0);

    // Scale canonical perspective view to specified field of view
    float invTanAng = 1 / std::tan(radians(fov) / 2);
    return scale(invTanAng, invTanAng, 1) * Transform(persp);
}

Bounds3f Transform::operator()(const Bounds3f &b) const {
    const Transform &M = *this;
    Bounds3f ret(M(Point3f(b.pMin.x, b.pMin.y, b.pMin.z)));
    ret = ret.expand(M(Point3f(b.pMax.x, b.pMin.y, b.pMin.z)));
    ret = ret.expand(M(Point3f(b.pMin.x, b.pMax.y, b.pMin.z)));
    ret = ret.expand(M(Point3f(b.pMin.x, b.pMin.y, b.pMax.z)));
    ret = ret.expand(M(Point3f(b.pMin.x, b.pMax.y, b.pMax.z)));
    ret = ret.expand(M(Point3f(b.pMax.x, b.pMax.y, b.pMin.z)));
    ret = ret.expand(M(Point3f(b.pMax.x, b.pMin.y, b.pMax.z)));
    ret = ret.expand(M(Point3f(b.pMax.x, b.pMax.y, b.pMax.z)));
    return ret;
}

Transform Transform::operator*(const Transform &t2) const {
    return Transform(Matrix4x4::multiply(m, t2.m), Matrix4x4::multiply(t2.mInv, mInv));
}

Interaction Transform::operator()(const Interaction &r) const {
    Interaction intr;
    const Transform &t = *this;
    intr.p = t(r.p);
    intr.n = normalize(t(r.n));
    intr.wo = normalize(t(r.wo));
    intr.uv = r.uv;
    intr.shape = r.shape;
    intr.primitive = r.primitive;
    intr.material = r.material;
    return intr;
}

bool Transform::swapHandedness() const {
    float det = m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) -
                m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) +
                m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);
    return det < 0;
}












} // namespace xeno