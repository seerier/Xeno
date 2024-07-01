#pragma once

#include"xeno.h"

namespace xeno {

// Vector2 Definition
template <typename T>
class Vector2 {
public:
    // Vector2 Public Methods
    Vector2() { x = y = 0; }
    Vector2(T a, T b) :x(a), y(b) {}
    explicit Vector2(const Point2<T> &p);

    Vector2<T> operator+(const Vector2<T> &v) const {
        return Vector2<T>(x + v.x, y + v.y);
    }

    Vector2<T> &operator+=(const Vector2<T> &v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2<T> operator-(const Vector2<T> &v) const {
        return Vector2<T>(x - v.x, y - v.y);
    }

    Vector2<T> &operator-=(const Vector2<T> &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    template <typename U>
    Vector2<T> operator*(U v) const {
        return Vector2<T>(x * v, y * v);
    }

    template <typename U>
    Vector2<T> &operator*=(U v) {
        x *= v;
        y *= v;
        return *this;
    }

    template <typename U>
    Vector2<T> operator/(U v) const {
        DCHECK_NE(v, 0);
        float inv = 1.f / v;
        return Vector2<T>(x * inv, y * inv);
    }

    template <typename U>
    Vector2<T> &operator/=(U v) {
        DCHECK_NE(v, 0);
        float inv = 1.f / v;
        x *= inv;
        y *= inv;
        return *this;
    }

    bool operator==(const Vector2<T> &v) const {
        return (x == v.x && y == v.y);
    }

    bool operator!=(const Vector2<T> &v) const {
        return (x != v.x || y != v.y);
    }

    Vector2<T> operator-() const {
        return Vector2<T>(-x, -y);
    }

    T operator[](int i) const {
        DCHECK(i >= 0 && i <= 1);
        if (i == 0) return x;
        return y;
    }

    T &operator[](int i) {
        DCHECK(i >= 0 && i <= 1);
        if (i == 0) return x;
        return y;
    }

    bool hasNaN() const {
        return isNaN(x) || isNaN(y);
    }

    float lengthSquared() const {
        return x * x + y * y;
    }

    float length() const {
        return std::sqrt(lengthSquared());
    }



    // Vector2 Public Data
    T x, y;
};


// Vector2 Utility Functions
template <typename T, typename U>
inline Vector2<T> operator*(U v, const Vector2<T> &vec) {
    return vec * v;
}

template <typename T>
inline Vector2<T> abs(const Vector2<T> &v) {
    return Vector2<T>(std::abs(v.x), std::abs(v.y));
}

template <typename T>
inline T dot(const Vector2<T> &u, const Vector2<T> &v) {
    return u.x * v.x + u.y * v.y;
}

template <typename T>
inline T absDot(const Vector2<T> &u, const Vector2<T> &v) {
    return std::abs(dot(u, v));
}

template <typename T>
inline T maxComponent(const Vector2<T> &v) {
    return std::max(v.x, v.y);
}

template <typename T>
inline T minComponent(const Vector2<T> &v) {
    return std::min(v.x, v.y);
}

template <typename T>
inline int maxDimension(const Vector2<T> &v) {
    (v.x > v.y) ? 0 : 1;
}

template <typename T>
inline Vector2<T> min(const Vector2<T> &u, const Vector2<T> &v) {
    return Vector2<T>(std::min(u.x, v.x), std::min(u.y, v.y));
}

template <typename T>
inline Vector2<T> max(const Vector2<T> &u, const Vector2<T> &v) {
    return Vector2<T>(std::max(u.x, v.x), std::max(u.y, v.y));
}

template <typename T>
inline Vector2<T> normalize(const Vector2<T> &v) {
    DCHECK_NE(v, Vector2<T>(0, 0));
    return v / v.length();
}


//Vector3 Definition
template <typename T>
class Vector3 {
public:
    // Vector3 Public Methods
    Vector3() { x = y = z = 0; }
    Vector3(T a, T b, T c) :x(a), y(b), z(c) {}
    explicit Vector3(const Point3<T> &p);

    Vector3<T> operator+(const Vector3<T> &v) const {
        return Vector3<T>(x + v.x, y + v.y, z + v.z);
    }

    Vector3<T> &operator+=(const Vector3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3<T> operator-(const Vector3<T> &v) const {
        return Vector3<T>(x - v.x, y - v.y, z - v.z);
    }

    Vector3<T> &operator-=(const Vector3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template <typename U>
    Vector3<T> operator*(U v) const {
        return Vector3<T>(x * v, y * v, z * v);
    }

    template <typename U>
    Vector3<T> &operator*=(U v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    template <typename U>
    Vector3<T> operator/(U v) const {
        DCHECK_NE(v, 0);
        float inv = 1.f / v;
        return Vector3<T>(x * inv, y * inv, z * inv);
    }

    template <typename U>
    Vector3<T> &operator/=(U v) {
        DCHECK_NE(v, 0);
        float inv = 1.f / v;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    bool operator==(const Vector3<T> &v) const {
        return (x == v.x && y == v.y && z == v.z);
    }

    bool operator!=(const Vector3<T> &v) const {
        return (x != v.x || y != v.y || z != v.z);
    }

    Vector3<T> operator-() const {
        return Vector3<T>(-x, -y, -z);
    }

    T operator[](int i) const {
        DCHECK(i >= 0 && i <= 2);
        if (i == 0) return x;
        else if (i == 1) return y;
        return z;
    }

    T &operator[](int i) {
        DCHECK(i >= 0 && i <= 1);
        if (i == 0) return x;
        else if (i == 1) return y;
        return z;
    }

    bool hasNaN() const {
        return isNaN(x) || isNaN(y) || isNaN(z);
    }

    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return std::sqrt(lengthSquared());
    }



    // Vector3 Public Data
    T x, y, z;
};


// Vector3 Utility Functions
template <typename T, typename U>
inline Vector3<T> operator*(U v, const Vector3<T> &vec) {
    return vec * v;
}

template <typename T>
inline Vector3<T> abs(const Vector3<T> &v) {
    return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
inline T dot(const Vector3<T> &u, const Vector3<T> &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

template <typename T>
inline T absDot(const Vector3<T> &u, const Vector3<T> &v) {
    return std::abs(dot(u, v));
}

template <typename T>
inline T maxComponent(const Vector3<T> &v) {
    return std::max(v.x, std::max(v.y, v.z));
}

template <typename T>
inline T minComponent(const Vector3<T> &v) {
    return std::min(v.x, std::min(v.y, v.z));
}

template <typename T>
inline int maxDimension(const Vector3<T> &v) {
    (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
}

template <typename T>
inline Vector3<T> min(const Vector3<T> &u, const Vector3<T> &v) {
    return Vector3<T>(std::min(u.x, v.x), std::min(u.y, v.y), std::min(u.z, v.z));
}

template <typename T>
inline Vector3<T> max(const Vector3<T> &u, const Vector3<T> &v) {
    return Vector3<T>(std::max(u.x, v.x), std::max(u.y, v.y), std::max(u.z, v.z));
}

template <typename T>
inline Vector3<T> normalize(const Vector3<T> &v) {
    DCHECK_NE(v, Vector3<T>(0, 0, 0));
    return v / v.length();
}

// Cross Product
template <typename T>
inline Vector3<T> cross(const Vector3<T> &u, const Vector3<T> &v) {
    double ux = u.x, uy = u.y, uz = u.z;
    double vx = v.x, vy = v.y, vz = v.z;
    return Vector3<T>((uy * vz) - (uz * vy), (uz * vx) - (vx * uz),
        (ux * vy) - (uy * vx));
}



// Point2 Definition
template <typename T>
class Point2 {
public:
    // Point2 Public Methods
    Point2() { x = y = 0; }
    Point2(T a, T b) :x(a), y(b) {}
    template <typename U>
    explicit Point2(const Point2<U> &p)
        :x((T)p.x), y((T)p.y) {}

    Point2<T> operator+(const Vector2<T> &v) const {
        return Point2<T>(x + v.x, y + v.y);
    }

    Point2<T> &operator+=(const Vector2<T> &v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2<T> operator-(const Point2<T> &v) const {
        return Vector2<T>(x - v.x, y - v.y);
    }

    Point2<T> operator-(const Vector2<T> &v) const {
        return Vector2<T>(x - v.x, y - v.y);
    }

    Point2<T> &operator-=(const Vector2<T> &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    template <typename U>
    Point2<T> operator*(U v) const {
        return Point2<T>(x * v, y * v);
    }

    template <typename U>
    Point2<T> &operator*=(U v) {
        x *= v;
        y *= v;
        return *this;
    }

    template <typename U>
    Point2<T> operator/(U v) const {
        DCHECK_NE(v, 0);
        float inv = 1.f / v;
        return Point2<T>(x * inv, y * inv);
    }

    template <typename U>
    Point2<T> &operator/=(U v) {
        DCHECK_NE(v, 0);
        float inv = 1.f / v;
        x *= inv;
        y *= inv;
        return *this;
    }

    bool operator==(const Point2<T> &v) const {
        return (x == v.x && y == v.y);
    }

    bool operator!=(const Point2<T> &v) const {
        return (x != v.x || y != v.y);
    }

    Point2<T> operator-() const {
        return Point2<T>(-x, -y);
    }

    T operator[](int i) const {
        DCHECK(i >= 0 && i <= 1);
        if (i == 0) return x;
        return y;
    }

    T &operator[](int i) {
        DCHECK(i >= 0 && i <= 1);
        if (i == 0) return x;
        return y;
    }

    bool hasNaN() const {
        return isNaN(x) || isNaN(y);
    }



    // Point2 Public Data
    T x, y;
};


// Point2 Utility Functions
template <typename T, typename U>
inline Point2<T> operator*(U v, const Point2<T> &vec) {
    return vec * v;
}

template <typename T>
inline Point2<T> abs(const Point2<T> &v) {
    return Point2<T>(std::abs(v.x), std::abs(v.y));
}

template <typename T>
inline T maxComponent(const Point2<T> &v) {
    return std::max(v.x, v.y);
}

template <typename T>
inline T minComponent(const Point2<T> &v) {
    return std::min(v.x, v.y);
}

template <typename T>
inline int maxDimension(const Point2<T> &v) {
    (v.x > v.y) ? 0 : 1;
}

template <typename T>
inline Point2<T> min(const Point2<T> &u, const Point2<T> &v) {
    return Point2<T>(std::min(u.x, v.x), std::min(u.y, v.y));
}

template <typename T>
inline Point2<T> max(const Point2<T> &u, const Point2<T> &v) {
    return Point2<T>(std::max(u.x, v.x), std::max(u.y, v.y));
}

template <typename T>
inline float distance(const Point2<T> &u, const Point2<T> &v) {
    return (u - v).length();
}

template <typename T>
inline float distanceSquared(const Point2<T> &u, const Point2<T> &v) {
    return (u - v).lengthSquared();
}


// Point3 Definition
template <typename T>
class Point3 {
public:
    // Point3 Public Methods
    Point3() { x = y = z = 0; }
    Point3(T a, T b, T c) :x(a), y(b), z(c) {}
    template <typename U>
    explicit Point3(const Point3<U> &p)
        :x((T)p.x), y((T)p.y), z((T)p.z) {}

    Point3<T> operator+(const Vector3<T> &v) const {
        return Point3<T>(x + v.x, y + v.y, z + v.z);
    }

    Point3<T> &operator+=(const Vector3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3<T> operator-(const Point3<T> &v) const {
        return Vector3<T>(x - v.x, y - v.y, z - v.z);
    }

    Point3<T> operator-(const Vector3<T> &v) const {
        return Vector3<T>(x - v.x, y - v.y, z - v.z);
    }

    Point3<T> &operator-=(const Vector3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template <typename U>
    Point3<T> operator*(U v) const {
        return Point3<T>(x * v, y * v, z * v);
    }

    template <typename U>
    Point3<T> &operator*=(U v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    template <typename U>
    Point3<T> operator/(U v) const {
        DCHECK_NE(v, 0);
        float inv = 1.f / v;
        return Point3<T>(x * inv, y * inv, z * inv);
    }

    template <typename U>
    Point3<T> &operator/=(U v) {
        DCHECK_NE(v, 0);
        float inv = 1.f / v;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    bool operator==(const Point3<T> &v) const {
        return (x == v.x && y == v.y && z == v.z);
    }

    bool operator!=(const Point3<T> &v) const {
        return (x != v.x || y != v.y || z != v.z);
    }

    Point3<T> operator-() const {
        return Point3<T>(-x, -y, -z);
    }

    T operator[](int i) const {
        DCHECK(i >= 0 && i <= 2);
        if (i == 0) return x;
        else if (i == 1) return y;
        return z;
    }

    T &operator[](int i) {
        DCHECK(i >= 0 && i <= 1);
        if (i == 0) return x;
        else if (i == 1) return y;
        return z;
    }

    bool hasNaN() const {
        return isNaN(x) || isNaN(y) || isNaN(z);
    }



    // Point3 Public Data
    T x, y, z;
};


// Point3 Utility Functions
template <typename T, typename U>
inline Point3<T> operator*(U v, const Point3<T> &vec) {
    return vec * v;
}

template <typename T>
inline Point3<T> abs(const Point3<T> &v) {
    return Point3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
inline T maxComponent(const Point3<T> &v) {
    return std::max(v.x, std::max(v.y, v.z));
}

template <typename T>
inline T minComponent(const Point3<T> &v) {
    return std::min(v.x, std::min(v.y, v.z));
}

template <typename T>
inline int maxDimension(const Point3<T> &v) {
    (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
}

template <typename T>
inline Point3<T> min(const Point3<T> &u, const Point3<T> &v) {
    return Point3<T>(std::min(u.x, v.x), std::min(u.y, v.y), std::min(u.z, v.z));
}

template <typename T>
inline Point3<T> max(const Point3<T> &u, const Point3<T> &v) {
    return Point3<T>(std::max(u.x, v.x), std::max(u.y, v.y), std::max(u.z, v.z));
}

template <typename T>
inline float distance(const Point3<T> &u, const Point3<T> &v) {
    return (u - v).length();
}

template <typename T>
inline float distanceSquared(const Point3<T> &u, const Point3<T> &v) {
    return (u - v).lengthSquared();
}



//Normal3 Definition
template <typename T>
class Normal3 {
public:
    // Normal3 Public Methods
    Normal3() { x = y = z = 0; }
    Normal3(T a, T b, T c) :x(a), y(b), z(c) {}
    explicit Normal3(const Vector3<T> &v) :x(v.x), y(v.y), z(v.z) {}

    Normal3<T> operator+(const Normal3<T> &v) const {
        return Normal3<T>(x + v.x, y + v.y, z + v.z);
    }

    Normal3<T> &operator+=(const Normal3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Normal3<T> operator-(const Normal3<T> &v) const {
        return Normal3<T>(x - v.x, y - v.y, z - v.z);
    }

    Normal3<T> &operator-=(const Normal3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template <typename U>
    Normal3<T> operator*(U v) const {
        return Normal3<T>(x * v, y * v, z * v);
    }

    template <typename U>
    Normal3<T> &operator*=(U v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    template <typename U>
    Normal3<T> operator/(U v) const {
        DCHECK_NE(v, 0);
        float inv = 1.f / v;
        return Normal3<T>(x * inv, y * inv, z * inv);
    }

    template <typename U>
    Normal3<T> &operator/=(U v) {
        DCHECK_NE(v, 0);
        float inv = 1.f / v;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    bool operator==(const Normal3<T> &v) const {
        return (x == v.x && y == v.y && z == v.z);
    }

    bool operator!=(const Normal3<T> &v) const {
        return (x != v.x || y != v.y || z != v.z);
    }

    Normal3<T> operator-() const {
        return Normal3<T>(-x, -y, -z);
    }

    T operator[](int i) const {
        DCHECK(i >= 0 && i <= 2);
        if (i == 0) return x;
        else if (i == 1) return y;
        return z;
    }

    T &operator[](int i) {
        DCHECK(i >= 0 && i <= 1);
        if (i == 0) return x;
        else if (i == 1) return y;
        return z;
    }

    bool hasNaN() const {
        return isNaN(x) || isNaN(y) || isNaN(z);
    }

    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return std::sqrt(lengthSquared());
    }



    // Normal3 Public Data
    T x, y, z;
};


// Normal3 Utility Functions
template <typename T, typename U>
inline Normal3<T> operator*(U v, const Normal3<T> &vec) {
    return vec * v;
}

template <typename T>
inline Normal3<T> abs(const Normal3<T> &v) {
    return Normal3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
inline T dot(const Normal3<T> &u, const Normal3<T> &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

template <typename T>
inline T dot(const Normal3<T> &u, const Vector3<T> &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

template <typename T>
inline T dot(const Vector3<T> &u, const Normal3<T> &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

template <typename T>
inline T absDot(const Normal3<T> &u, const Normal3<T> &v) {
    return std::abs(dot(u, v));
}

template <typename T>
inline T absDot(const Normal3<T> &u, const Vector3<T> &v) {
    return std::abs(dot(u, v));
}

template <typename T>
inline T absDot(const Vector3<T> &u, const Normal3<T> &v) {
    return std::abs(dot(u, v));
}

template <typename T>
inline T maxComponent(const Normal3<T> &v) {
    return std::max(v.x, std::max(v.y, v.z));
}

template <typename T>
inline T minComponent(const Normal3<T> &v) {
    return std::min(v.x, std::min(v.y, v.z));
}

template <typename T>
inline int maxDimension(const Normal3<T> &v) {
    (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
}

template <typename T>
inline Normal3<T> min(const Normal3<T> &u, const Normal3<T> &v) {
    return Normal3<T>(std::min(u.x, v.x), std::min(u.y, v.y), std::min(u.z, v.z));
}

template <typename T>
inline Normal3<T> max(const Normal3<T> &u, const Normal3<T> &v) {
    return Normal3<T>(std::max(u.x, v.x), std::max(u.y, v.y), std::max(u.z, v.z));
}

template <typename T>
inline Normal3<T> normalize(const Normal3<T> &v) {
    return v / v.length();
}

// Cross Product
template <typename T>
inline Vector3<T> cross(const Normal3<T> &u, const Normal3<T> &v) {
    double ux = u.x, uy = u.y, uz = u.z;
    double vx = v.x, vy = v.y, vz = v.z;
    return Vector3<T>((uy * vz) - (uz * vy), (uz * vx) - (vx * uz),
        (ux * vy) - (uy * vx));
}

template <typename T>
inline Vector3<T> cross(const Normal3<T> &u, const Vector3<T> &v) {
    double ux = u.x, uy = u.y, uz = u.z;
    double vx = v.x, vy = v.y, vz = v.z;
    return Vector3<T>((uy * vz) - (uz * vy), (uz * vx) - (vx * uz),
        (ux * vy) - (uy * vx));
}


template <typename T>
inline Vector3<T> cross(const Vector3<T> &u, const Normal3<T> &v) {
    double ux = u.x, uy = u.y, uz = u.z;
    double vx = v.x, vy = v.y, vz = v.z;
    return Vector3<T>((uy * vz) - (uz * vy), (uz * vx) - (vx * uz),
        (ux * vy) - (uy * vx));
}



// Stream Output

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Vector2<T> &v) {
    os << "[ " << v.x << ", " << v.y << " ]";
    return os;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Vector3<T> &v) {
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
    return os;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Point2<T> &v) {
    os << "[ " << v.x << ", " << v.y << " ]";
    return os;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Point3<T> &v) {
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
    return os;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Normal3<T> &v) {
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
    return os;
}




// Function Definitions
template <typename T>
inline Vector2<T>::Vector2(const Point2<T> &p) : x(p.x), y(p.y) {}

template <typename T>
inline Vector3<T>::Vector3(const Point3<T> &p) : x(p.x), y(p.y), z(p.z) {}



// type defining
typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Point3<int> Point3i;
typedef Point3<float> Point3f;
typedef Normal3<int> Normal3i;
typedef Normal3<float> Normal3f;


} // namespace xeno