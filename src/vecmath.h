#pragma once

#include"xeno.h"

#include<iterator>

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

    Vector2<T> operator*(const Vector2<T> &v) const {
        return Vector2<T>(x * v.x, y * v.y);
    }

    Vector2<T> &operator*=(const Vector2<T> &v) {
        x *= v.x;
        y *= v.y;
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
    return (v.x > v.y) ? 0 : 1;
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
    explicit Vector3(const Normal3<T> &n);

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

    Vector3<T> operator*(const Vector3<T> &v) const {
        return Vector3<T>(x * v.x, y * v.y, z * v.z);
    }

    Vector3<T> &operator*=(const Vector3<T> &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
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
        DCHECK(i >= 0 && i <= 2);
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
    return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
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
    return (v.x > v.y) ? 0 : 1;
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
        DCHECK(i >= 0 && i <= 2);
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
    return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
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
        DCHECK(i >= 0 && i <= 2);
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
inline Vector3<T> operator+(const Vector3<T> &v, const Normal3<T> &n) {
    return Vector3<T>(v.x + n.x, v.y + n.y, v.z + n.z);
}

template <typename T>
inline Vector3<T> operator+(const Normal3<T> &n, const Vector3<T> &v) {
    return Vector3<T>(v.x + n.x, v.y + n.y, v.z + n.z);
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
    return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
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





template <typename T>
class Bounds2 {
public:
    // Bounds2 Public Methods
    Bounds2() {
        T min = std::numeric_limits<T>::lowest();
        T max = std::numeric_limits<T>::max();
        pMin = Point2<T>(max, max);
        pMax = Point2<T>(min, min);
    }

    explicit Bounds2(const Point2<T> &p) :pMin(p), pMax(p) {}

    Bounds2(const Point2<T> a, const Point2<T> b) {
        pMin = Point2<T>(std::min(a.x, b.x), std::min(a.y, b.y));
        pMax = Point2<T>(std::max(a.x, b.x), std::max(a.y, b.y));
    }

    template <typename U>
    explicit operator Bounds2<U>() const {
        return Bounds2<U>((Point2<U>)pMin, (Point2<U>)pMax);
    }

    bool operator==(const Bounds2<T> &b) const {
        return pMin == b.pMin && pMax == b.pMax;
    }

    bool operator!=(const Bounds2<T> &b) const {
        return pMin != b.pMin || pMax != b.pMax;
    }

    Point2<T> &operator[](int i) {
        DCHECK(i == 0 || i == 1);
        if (i == 0) return pMin;
        return pMax;
    }

    Point2<T> const &operator[](int i) const {
        DCHECK(i == 0 || i == 1);
        if (i == 0) return pMin;
        return pMax;
    }

    Point2<T> corner(int i) const {
        DCHECK(i >= 0 && i <= 3);
            return Point2<T>((*this)[i & 1].x,
                (*this)[(i & 2) ? 1 : 0].y);
    }

    Vector2<T> diagonal() const {
        return pMax - pMin;
    }

    int maxExtent() const {
        Vector2<T> diag = diagnal();
        if (diag.x > diag.y) return 0;
        return 1;
    }

    int minExtent() const {
        Vector2<T> diag = diagnal();
        if (diag.x < diag.y) return 0;
        return 1;
    }

    friend std::ostream &operator<<(std::ostream &os, const Bounds2<T> &b) {
        os << "[ " << b.pMin << "-" << b.pMax << " ]";
        return os;
    }

    Bounds2<T> expand(const Bounds2<T> &b) const {
        Bounds2<T> rec;
        rec.pMin = min(pMin, b.pMin);
        rec.pMax = min(pMax, b.pMax);
        return rec;
    }

    Bounds2<T> expand(const Point2<T> &p) const {
        Bounds2<T> rec;
        rec.pMin = min(pMin, p);
        rec.pMax = min(pMax, p);
        return rec;
    }

    template <typename U>
    Bounds2<T> expand(U delta) const {
        return Bounds2<T>(pMin - Vector2<T>(delta, delta),
                        pMax + Vector2<T>(delta, delta));
    }

    Bounds2<T> intersection(const Bounds2<T> &b) const {
        Bounds2<T> rec;
        rec.pMin = max(pMin, b.pMin);
        rec.pMax = min(pMax, b.pMax);
        return rec;
    }

    bool contains(const Point2<T> &p) const {
        return (pMin.x <= p.x && pMin.y <= p.y && pMax.x >= p.x && pMax.y >= p.y);
    }

    bool contains(const Bounds2<T> b) const {
        return (pMin.x <= b.pMin.x && pMin.y <= b.pMin.y && pMax.x >= b.pMax.x && pMax.y >= b.pMax.y);
    }

    bool overlaps(const Bounds2<T> b) const {
        bool x = (pMin.x <= b.pMax.x && pMax.x >= b.pMin.x);
        bool y = (pMin.y <= b.pMax.y && pMax.y >= b.pMin.y);
        return x && y;
    }

    bool containsExclusive(const Point2<T> &p) const {
    return (pMin.x < p.x && pMin.y < p.y && pMax.x > p.x && pMax.y > p.y);
    }

    bool containsExclusive(const Bounds2<T> b) const {
    return (pMin.x < b.pMin.x && pMin.y < b.pMin.y && pMax.x > b.pMax.x && pMax.y > b.pMax.y);
    }

    bool overlapsExclusive(const Bounds2<T> b) const {
    bool x = (pMin.x < b.pMax.x && pMax.x > b.pMin.x);
    bool y = (pMin.y < b.pMax.y && pMax.y > b.pMin.y);
    return x && y;
    }

    // Distance to a point. Return 0 if the point is inside.
    template <typename U>
    float distanceSquaredTo(const Point2<U> p) const {
        float x = std::max({ 0.f, pMin.x - p.x, p.x - pMax.x });
        float y = std::max({ 0.f, pMin.y - p.y, p.y - pMax.y });
        return x * x + y * y;
    }

    template <typename U>
    float distanceTo(const Point2<U> b) const {
        return std::sqrt(distanceSquaredTo(b));
    }

    // Distance to a Bounds2<U>. Return 0 if they overlap.
    template <typename U>
    float distanceSquaredTo(const Bounds2<U> b) const {
        float x = std::max({ 0.f, pMin.x - b.pMax.x, b.pMin.x - pMax.x });
        float y = std::max({ 0.f, pMin.y - b.pMax.y, b.pMin.y - pMax.y });
        return x * x + y * y;
    }

    template <typename U>
    float distanceTo(const Bounds2<U> p) const {
        return std::sqrt(distanceSquaredTo(p));
    }



    Point2<T> lerp(const Point2<float> &t) const {
        return Point2<T>((1 - t.x) * pMin.x + t.x * pMax.x, (1 - t.y) * pMin.y + t.y * pMax.y);
    }


    // Bounds2 Public Data
    Point2<T> pMin, pMax;
};


template <typename T>
class Bounds3 {
public:
    // Bounds3 Public Methods
    Bounds3() {
        T min = std::numeric_limits<T>::lowest();
        T max = std::numeric_limits<T>::max();
        pMin = Point3<T>(max, max, max);
        pMax = Point3<T>(min, min, min);
    }

    explicit Bounds3(const Point3<T> &p) :pMin(p), pMax(p) {}

    Bounds3(const Point3<T> a, const Point3<T> b) {
        pMin = Point3<T>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
        pMax = Point3<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }

    template <typename U>
    explicit operator Bounds3<U>() const {
        return Bounds3<U>((Point3<U>)pMin, (Point3<U>)pMax);
    }

    bool operator==(const Bounds3<T> &b) const {
        return pMin == b.pMin && pMax == b.pMax;
    }

    bool operator!=(const Bounds3<T> &b) const {
        return pMin != b.pMin || pMax != b.pMax;
    }

    Point3<T> &operator[](int i) {
        DCHECK(i == 0 || i == 1);
        if (i == 0) return pMin;
        return pMax;
    }

    Point3<T> const &operator[](int i) const {
        DCHECK(i == 0 || i == 1);
        if (i == 0) return pMin;
        return pMax;
    }

    Point3<T> corner(int i) const {
        DCHECK(i >= 0 && i <= 7);
        return Point3<T>((*this)[i & 1].x,
                        (*this)[(i & 2) ? 1 : 0].y
                        (*this)[(i & 4) ? 1 : 0].z);
    }

    Vector3<T> diagonal() const {
        return pMax - pMin;
    }

    int maxExtent() const {
        Vector3<T> diag = diagnal();
        return (diag.x > diag.y) ? (diag.x > diag.z ? 0 : 2) : (diag.y > diag.z ? 1 : 2);
    }

    int minExtent() const {
        Vector3<T> diag = diagnal();
        return (diag.x < diag.y) ? (diag.x < diag.z ? 0 : 2) : (diag.y < diag.z ? 1 : 2);
    }

    friend std::ostream &operator<<(std::ostream &os, const Bounds3<T> &b) {
        os << "[ " << b.pMin << "-" << b.pMax << " ]";
        return os;
    }

    Bounds3<T> expand(const Bounds3<T> &b) const {
        Bounds3<T> rec;
        rec.pMin = min(pMin, b.pMin);
        rec.pMax = min(pMax, b.pMax);
        return rec;
    }

    Bounds3<T> expand(const Point3<T> &p) const {
        Bounds3<T> rec;
        rec.pMin = min(pMin, p);
        rec.pMax = min(pMax, p);
        return rec;
    }

    template <typename U>
    Bounds3<T> expand(U delta) const {
        return Bounds3<T>(pMin - Vector3<T>(delta, delta, delta),
                        pMax + Vector3<T>(delta, delta, delta));
    }

    Bounds3<T> intersection(const Bounds3<T> &b) const {
        Bounds3<T> rec;
        rec.pMin = max(pMin, b.pMin);
        rec.pMax = min(pMax, b.pMax);
        return rec;
    }

    bool contains(const Point3<T> &p) const {
        return (pMin.x <= p.x && pMin.y <= p.y && pMin.z <= p.z && pMax.x >= p.x && pMax.y >= p.y && pMax.z >= p.z);
    }

    bool contains(const Bounds3<T> b) const {
        return (pMin.x <= b.pMin.x && pMin.y <= b.pMin.y && pMin.z <= b.pMin.z && pMax.x >= b.pMax.x && pMax.y >= b.pMax.y && pMax.z >= b.pMax.z);
    }

    bool overlaps(const Bounds3<T> b) const {
        bool x = (pMin.x <= b.pMax.x && pMax.x >= b.pMin.x);
        bool y = (pMin.y <= b.pMax.y && pMax.y >= b.pMin.y);
        bool z = (pMin.z <= b.pMax.z && pMax.z >= b.pMin.z);
        return x && y && z;
    }

    bool containsExclusive(const Point3<T> &p) const {
    return (pMin.x < p.x && pMin.y < p.y && pMin.z < p.z && pMax.x > p.x && pMax.y > p.y && pMax.z > p.z);
    }

    bool containsExclusive(const Bounds3<T> b) const {
    return (pMin.x < b.pMin.x && pMin.y < b.pMin.y && pMin.z < b.pMin.z && pMax.x > b.pMax.x && pMax.y > b.pMax.y && pMax.z > b.pMax.z);
    }

    bool overlapsExclusive(const Bounds3<T> b) const {
    bool x = (pMin.x < b.pMax.x && pMax.x > b.pMin.x);
    bool y = (pMin.y < b.pMax.y && pMax.y > b.pMin.y);
    bool z = (pMin.z < b.pMax.z && pMax.z > b.pMin.z);
    return x && y && z;
    }

    // Distance to a point. Return 0 if the point is inside.
    template <typename U>
    float distanceSquaredTo(const Point3<U> p) const {
        float x = std::max({ 0.f, pMin.x - p.x, p.x - pMax.x });
        float y = std::max({ 0.f, pMin.y - p.y, p.y - pMax.y });
        float z = std::max({ 0.f, pMin.z - p.z, p.z - pMax.z });
        return x * x + y * y + z * z;
    }

    template <typename U>
    float distanceTo(const Point3<U> p) const {
        return std::sqrt(distanceSquaredTo(p));
    }

    // Distance to a Bounds3<U>. Return 0 if they overlap.
    template <typename U>
    float distanceSquaredTo(const Bounds3<U> b) const {
        float x = std::max({ 0.f, pMin.x - b.pMax.x, b.pMin.x - pMax.x });
        float y = std::max({ 0.f, pMin.y - b.pMax.y, b.pMin.y - pMax.y });
        float z = std::max({ 0.f, pMin.z - b.pMax.z, b.pMin.z - pMax.z });
        return x * x + y * y + z * z;
    }

    template <typename U>
    float distanceTo(const Bounds3<U> b) const {
        return std::sqrt(distanceSquaredTo(b));
    }


    Point3<T> lerp(const Point3<float> &t) const {
        return Point3<T>((1 - t.x) * pMin.x + t.x * pMax.x, (1 - t.y) * pMin.y + t.y * pMax.y,
                        (1 - t.z) * pMin.z + t.z * pMax.z);
    }



    // Bounds3 Public Data
    Point3<T> pMin, pMax;
};

template <typename T>
inline Bounds2<T> aabbUnion(const Bounds2<T> &a, const Bounds2<T> &b) {
    return Bounds2<T>(min(a.pMin, b.pMin), max(a.pMax, b.pMax));
}

template <typename T>
inline Bounds2<T> aabbUnion(const Bounds2<T> &a, const Point2<T> &p) {
    return Bounds2<T>(min(a.pMin, p), max(a.pMax, p));
}

template <typename T>
inline Bounds2<T> aabbUnion(const Point2<T> &p, const Bounds2<T> &a) {
    return Bounds2<T>(min(a.pMin, p), max(a.pMax, p));
}

template <typename T>
inline Bounds3<T> aabbUnion(const Bounds3<T> &a, const Bounds3<T> &b) {
    return Bounds3<T>(min(a.pMin, b.pMin), max(a.pMax, b.pMax));
}

template <typename T>
inline Bounds3<T> aabbUnion(const Bounds3<T> &a, const Point3<T> &p) {
    return Bounds3<T>(min(a.pMin, p), max(a.pMax, p));
}

template <typename T>
inline Bounds3<T> aabbUnion(const Point3<T> &p, const Bounds3<T> &a) {
    return Bounds3<T>(min(a.pMin, p), max(a.pMax, p));
}



// type defining
typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Point2<int> Point2i;
typedef Point2<float> Point2f;
typedef Point3<int> Point3i;
typedef Point3<float> Point3f;
typedef Normal3<int> Normal3i;
typedef Normal3<float> Normal3f;
typedef Bounds2<int> Bounds2i;
typedef Bounds2<float> Bounds2f;
typedef Bounds3<int> Bounds3i;
typedef Bounds3<float> Bounds3f;




// An iterator to loop for Bounds2i
class Bounds2iIterator : public std::forward_iterator_tag {
  public:
    Bounds2iIterator(const Bounds2i &b, const Point2i &pt)
        : p(pt), bounds(&b) {}
    Bounds2iIterator operator++() {
        advance();
        return *this;
    }
    Bounds2iIterator operator++(int) {
        Bounds2iIterator old = *this;
        advance();
        return old;
    }
    bool operator==(const Bounds2iIterator &bi) const {
        return p == bi.p && bounds == bi.bounds;
    }
    bool operator!=(const Bounds2iIterator &bi) const {
        return p != bi.p || bounds != bi.bounds;
    }

    Point2i operator*() const { return p; }

  private:
    void advance() {
        ++p.x;
        if (p.x == bounds->pMax.x) {
            p.x = bounds->pMin.x;
            ++p.y;
        }
    }
    Point2i p;
    const Bounds2i *bounds;
};


// Bounds2Iterator related functions
inline Bounds2iIterator begin(const Bounds2i &b) {
    return Bounds2iIterator(b, b.pMin);
}

inline Bounds2iIterator end(const Bounds2i &b) {
    // Normally, the ending point is at the minimum x value and one past
    // the last valid y value.
    Point2i pEnd(b.pMin.x, b.pMax.y);
    // However, if the bounds are degenerate, override the end point to
    // equal the start point so that any attempt to iterate over the bounds
    // exits out immediately.
    if (b.pMin.x >= b.pMax.x || b.pMin.y >= b.pMax.y)
        pEnd = b.pMin;
    return Bounds2iIterator(b, pEnd);
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



// Member Function Definitions
template <typename T>
inline Vector2<T>::Vector2(const Point2<T> &p) : x(p.x), y(p.y) {}

template <typename T>
inline Vector3<T>::Vector3(const Point3<T> &p) : x(p.x), y(p.y), z(p.z) {}

template <typename T>
inline Vector3<T>::Vector3(const Normal3<T> &n) : x(n.x), y(n.y), z(n.z) {}





// Utility Functions
template <typename T>
inline Vector3<T> cross(const Normal3<T> &u, const Normal3<T> &v) {
    double ux = u.x, uy = u.y, uz = u.z;
    double vx = v.x, vy = v.y, vz = v.z;
    return Vector3<T>((uy * vz) - (uz * vy), (uz * vx) - (ux * vz),
        (ux * vy) - (uy * vx));
}

template <typename T>
inline Vector3<T> cross(const Normal3<T> &u, const Vector3<T> &v) {
    double ux = u.x, uy = u.y, uz = u.z;
    double vx = v.x, vy = v.y, vz = v.z;
    return Vector3<T>((uy * vz) - (uz * vy), (uz * vx) - (ux * vz),
        (ux * vy) - (uy * vx));
}

template <typename T>
inline Vector3<T> cross(const Vector3<T> &u, const Normal3<T> &v) {
    double ux = u.x, uy = u.y, uz = u.z;
    double vx = v.x, vy = v.y, vz = v.z;
    return Vector3<T>((uy * vz) - (uz * vy), (uz * vx) - (ux * vz),
        (ux * vy) - (uy * vx));
}

template <typename T>
inline Vector3<T> cross(const Vector3<T> &u, const Vector3<T> &v) {
    double ux = u.x, uy = u.y, uz = u.z;
    double vx = v.x, vy = v.y, vz = v.z;
    return Vector3<T>((uy * vz) - (uz * vy), (uz * vx) - (ux * vz),
        (ux * vy) - (uy * vx));
}

template <typename T>
inline Normal3<T> faceForward(const Normal3<T> &n, const Vector3<T> &v) {
    return (dot(n, v) < 0.f) ? -n : n;
}

template <typename T>
inline Normal3<T> faceForward(const Normal3<T> &n, const Normal3<T> &n2) {
    return (dot(n, n2) < 0.f) ? -n : n;
}

template <typename T>
inline Vector3<T> faceForward(const Vector3<T> &v, const Vector3<T> &v2) {
    return (dot(v, v2) < 0.f) ? -v : v;
}

template <typename T>
inline Vector3<T> faceForward(const Vector3<T> &v, const Normal3<T> &n2) {
    return (dot(v, n2) < 0.f) ? -v : v;
}

template <typename T>
inline Point2<T> lerp(float t, const Point2<T> &a, const Point2<T> &b) {
    return (1 - t) * a + t * b;
}

template <typename T>
inline Point3<T> lerp(float t, const Point3<T> &a, const Point3<T> &b) {
    return (1 - t) * a + t * b;
}

template <typename T>
inline void coordinateSystem(const Vector3<T> &v1, Vector3<T> *v2,
                             Vector3<T> *v3) {
    if (std::abs(v1.x) > std::abs(v1.y))
        *v2 = Vector3<T>(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
    else
        *v2 = Vector3<T>(0, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
    *v3 = cross(v1, *v2);
}

inline Vector3f sphericalDirection(float sinTheta, float cosTheta, float phi) {
    return Vector3f(sinTheta * std::cos(phi), sinTheta * std::sin(phi),
                    cosTheta);
}

inline Vector3f sphericalDirection(float sinTheta, float cosTheta, float phi,
                                   const Vector3f &x, const Vector3f &y,
                                   const Vector3f &z) {
    return sinTheta * std::cos(phi) * x + sinTheta * std::sin(phi) * y +
           cosTheta * z;
}

inline float sphericalTheta(const Vector3f &v) {
    return std::acos(clamp(v.z, -1, 1));
}

inline float sphericalPhi(const Vector3f &v) {
    float p = std::atan2(v.y, v.x);
    return (p < 0) ? (p + 2 * Pi) : p;
}




inline Vector3f cosineSampleHemisphere(const Point2f &sample) {
    float phi = 2 * Pi * sample.x;
    return Vector3f(cosf(phi) * sqrtf(sample.y), sinf(phi) * sqrtf(sample.y), sqrtf(1 - sample.y));
}


inline Point3f offsetRayOrigin(const Point3f &p, const Normal3f &n, const Vector3f &w) {
    Vector3f offset = Vector3f(n) * offsetScale;
    if (dot(n, w) < 0)offset *= -1;
    return p + offset;
}




} // namespace xeno