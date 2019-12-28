#ifndef __V3D__
#define __V3D__

// Inline call
#ifndef IC
#define IC __forceinline
#endif

template <class T>
struct _vector3
{
public:
    typedef T TYPE;
    typedef _vector3<T> Self;
    typedef Self& SelfRef;
    typedef const Self& SelfCRef;
public:
    T x, y, z;

    // access operators
    ICF T& operator[] (int i) { return *((T*)this + i); }
    ICF T& operator[] (int i) const { return *((T*)this + i); }

    ICF SelfRef set(T _x, T _y, T _z) { x = _x; y = _y; z = _z; return *this; };
    ICF SelfRef set(const _vector3<float>& v) { x = T(v.x); y = T(v.y); z = T(v.z); return *this; };
    ICF SelfRef set(const _vector3<double>& v) { x = T(v.x); y = T(v.y); z = T(v.z); return *this; };
    ICF SelfRef set(float* p) { x = p[0]; y = p[1]; z = p[2]; return *this; };
    ICF SelfRef set(double* p) { x = p[0]; y = p[1]; z = p[2]; return *this; };

    ICF SelfRef add(const Self& v) { x += v.x; y += v.y; z += v.z; return *this; };
    ICF SelfRef add(T s) { x += s; y += s; z += s; return *this; };
    ICF SelfRef add(const Self& a, const Self& v) { x = a.x + v.x; y = a.y + v.y; z = a.z + v.z; return *this; };
    ICF SelfRef add(const Self& a, T s) { x = a.x + s; y = a.y + s; z = a.z + s; return *this; };

    ICF SelfRef sub(const Self& v) { x -= v.x; y -= v.y; z -= v.z; return *this; };
    ICF SelfRef sub(T s) { x -= s; y -= s; z -= s; return *this; };
    ICF SelfRef sub(const Self& a, const Self& v) { x = a.x - v.x; y = a.y - v.y; z = a.z - v.z; return *this; };
    ICF SelfRef sub(const Self& a, T s) { x = a.x - s; y = a.y - s; z = a.z - s; return *this; };

    ICF SelfRef mul(const Self& v) { x *= v.x; y *= v.y; z *= v.z; return *this; };
    ICF SelfRef mul(T s) { x *= s; y *= s; z *= s; return *this; };
    ICF SelfRef mul(const Self& a, const Self& v) { x = a.x*v.x; y = a.y*v.y; z = a.z*v.z; return *this; };
    ICF SelfRef mul(const Self& a, T s) { x = a.x*s; y = a.y*s; z = a.z*s; return *this; };

    ICF SelfRef div(const Self& v) { x /= v.x; y /= v.y; z /= v.z; return *this; };
    ICF SelfRef div(T s) { x /= s; y /= s; z /= s; return *this; };
    ICF SelfRef div(const Self& a, const Self& v) { x = a.x / v.x; y = a.y / v.y; z = a.z / v.z; return *this; };
    ICF SelfRef div(const Self& a, T s) { x = a.x / s; y = a.y / s; z = a.z / s; return *this; };

    IC SelfRef invert() { x = -x; y = -y; z = -z; return *this; }
    IC SelfRef invert(const Self& a) { x = -a.x; y = -a.y; z = -a.z; return *this; }

    IC SelfRef min(const Self& v1, const Self& v2) { x = XrMath::min(v1.x, v2.x); y = XrMath::min(v1.y, v2.y); z = XrMath::min(v1.z, v2.z); return *this; }
    IC SelfRef min(const Self& v) { x = XrMath::min(x, v.x); y = XrMath::min(y, v.y); z = XrMath::min(z, v.z); return *this; }
    IC SelfRef max(const Self& v1, const Self& v2) { x = XrMath::max(v1.x, v2.x); y = XrMath::max(v1.y, v2.y); z = XrMath::max(v1.z, v2.z); return *this; }
    IC SelfRef max(const Self& v) { x = XrMath::max(x, v.x); y = XrMath::max(y, v.y); z = XrMath::max(z, v.z); return *this; }

    IC SelfRef abs(const Self& v) { x = XrMath::abs(v.x); y = XrMath::abs(v.y); z = XrMath::abs(v.z); return *this; }
    ICF BOOL similar(const Self& v, T E = XrMath::EPS_L) const { return XrMath::abs(x - v.x) < E && XrMath::abs(y - v.y) < E && XrMath::abs(z - v.z) < E; };

    IC SelfRef set_length(T l)
    {
        mul(l / magnitude());
        return *this;
    }

    // Align vector3 by axis (!y)
    IC SelfRef align()
    {
        y = 0;
        if (XrMath::abs(z) >= XrMath::abs(x)) { z /= XrMath::abs(z ? z : 1); x = 0; }
        else { x /= XrMath::abs(x); z = 0; }
        return *this;
    }

    // Squeeze
    IC SelfRef squeeze(T Epsilon)
    {
        if (XrMath::abs(x) < Epsilon) x = 0;
        if (XrMath::abs(y) < Epsilon) y = 0;
        if (XrMath::abs(z) < Epsilon) z = 0;
        return *this;
    }

    // Clamp vector3
    IC SelfRef clamp(const Self& min, const Self& max)
    {
		XrMath::clamp(x, min.x, max.x);
		XrMath::clamp(y, min.y, max.y);
		XrMath::clamp(z, min.z, max.z);
        return *this;
    }

    IC SelfRef clamp(const Self& _v)
    {
        Self v;
        v.x = XrMath::abs(_v.x);
        v.y = XrMath::abs(_v.y);
        v.z = XrMath::abs(_v.z);
		XrMath::clamp(x, -v.x, v.x);
		XrMath::clamp(y, -v.y, v.y);
		XrMath::clamp(z, -v.z, v.z);
        return *this;
    }

    // Interpolate vectors (inertion)
    IC SelfRef inertion(const Self& p, T v)
    {
        T inv = 1.f - v;
        x = v*x + inv*p.x;
        y = v*y + inv*p.y;
        z = v*z + inv*p.z;
        return *this;
    }
    IC SelfRef average(const Self& p)
    {
        x = (x + p.x)*0.5f;
        y = (y + p.y)*0.5f;
        z = (z + p.z)*0.5f;
        return *this;
    }
    IC SelfRef average(const Self& p1, const Self& p2)
    {
        x = (p1.x + p2.x)*0.5f;
        y = (p1.y + p2.y)*0.5f;
        z = (p1.z + p2.z)*0.5f;
        return *this;
    }
    IC SelfRef lerp(const Self& p1, const Self& p2, T t)
    {
        T invt = 1.f - t;
        x = p1.x*invt + p2.x*t;
        y = p1.y*invt + p2.y*t;
        z = p1.z*invt + p2.z*t;
        return *this;
    }

    // Direct vector3 from point P by dir D with length M
    IC SelfRef mad(const Self& d, T m)
    {
        x += d.x*m;
        y += d.y*m;
        z += d.z*m;
        return *this;
    }
    IC SelfRef mad(const Self& p, const Self& d, T m)
    {
        x = p.x + d.x*m;
        y = p.y + d.y*m;
        z = p.z + d.z*m;
        return *this;
    }
    IC SelfRef mad(const Self& d, const Self& s)
    {
        x += d.x*s.x;
        y += d.y*s.y;
        z += d.z*s.z;
        return *this;
    }
    IC SelfRef mad(const Self& p, const Self& d, const Self& s)
    {
        x = p.x + d.x*s.x;
        y = p.y + d.y*s.y;
        z = p.z + d.z*s.z;
        return *this;
    }

    // SQ magnitude
    IC T square_magnitude(void) const
    {
        return x*x + y*y + z*z;
    }
    // magnitude
    IC T magnitude(void) const
    {
        return XrMath::sqrt(square_magnitude());
    }

    // Normalize
    IC T normalize_magn(void)
    {
        VERIFY(square_magnitude() > std::numeric_limits<T>::min());
        T len = magnitude();
        T inv_len = T(1) / len;
        x *= inv_len;
        y *= inv_len;
        z *= inv_len;
        return len;
    }

    ICF SelfRef normalize(void)
    {
        VERIFY(square_magnitude() > std::numeric_limits<T>::min());
        T mag = XrMath::sqrt(T(1) / (x*x + y*y + z*z));
        x *= mag;
        y *= mag;
        z *= mag;
        return *this;
    }

    // Safe-Normalize
    ICF SelfRef normalize_safe(void)
    {
        T magnitude = x*x + y*y + z*z;
        if (magnitude > std::numeric_limits<T>::min())
        {
            magnitude = XrMath::sqrt(1 / magnitude);
            x *= magnitude;
            y *= magnitude;
            z *= magnitude;
        }
        return *this;
    }

    // Normalize
    ICF SelfRef normalize(const Self& v)
    {
        VERIFY((v.x*v.x + v.y*v.y + v.z*v.z) > flt_zero);
        T mag = XrMath::sqrt(1 / (v.x*v.x + v.y*v.y + v.z*v.z));
        x = v.x*mag;
        y = v.y*mag;
        z = v.z*mag;
        return *this;
    }

    // Safe-Normalize
    ICF SelfRef normalize_safe(const Self& v)
    {
        T magnitude = v.x*v.x + v.y*v.y + v.z*v.z;
        if (magnitude > std::numeric_limits<T>::min())
        {
            magnitude = XrMath::sqrt(1 / magnitude);
            x = v.x*magnitude;
            y = v.y*magnitude;
            z = v.z*magnitude;
        }
        return *this;
    }
    IC SelfRef random_dir(CRandom& R = ::Random)
    {
        //z = R.randF(-1,1);
        z = XrMath::cos(R.randF(XrMath::M_PI));
        T a = R.randF(XrMath::PI_MUL_2);
        T r = XrMath::sqrt(1 - z*z);
        T sa = XrMath::sin(a);
        T ca = XrMath::cos(a);
        x = r * ca;
        y = r * sa;
        return *this;
    };
    IC SelfRef random_dir(const Self& ConeAxis, float ConeAngle, CRandom& R = ::Random)
    {
        Self rnd;
        rnd.random_dir(R);
        mad(ConeAxis, rnd, R.randF(tanf(ConeAngle)));
        normalize();
        return *this;
    }
    IC SelfRef random_point(const Self& BoxSize, CRandom& R = ::Random)
    {
        x = R.randFs(BoxSize.x);
        y = R.randFs(BoxSize.y);
        z = R.randFs(BoxSize.z);
        return *this;
    }
    IC SelfRef random_point(T r, CRandom& R = ::Random)
    {
        random_dir(R);
        mul(R.randF(r));
        return *this;
    }

    // DotProduct
    ICF T dotproduct(const Self& v) const // v1*v2
    {
        return x*v.x + y*v.y + z*v.z;
    }

    // CrossProduct
    ICF SelfRef crossproduct(const Self& v1, const Self& v2) // (v1,v2) -> this
    {
        x = v1.y * v2.z - v1.z * v2.y;
        y = v1.z * v2.x - v1.x * v2.z;
        z = v1.x * v2.y - v1.y * v2.x;
        return *this;
    }

    // Distance calculation
    IC T distance_to_xz(const Self& v) const
    {
        return XrMath::sqrt((x - v.x)*(x - v.x) + (z - v.z)*(z - v.z));
    }
    IC T distance_to_xz_sqr(const Self& v) const
    {
        return (x - v.x)*(x - v.x) + (z - v.z)*(z - v.z);
    }

    // Distance calculation
    ICF T distance_to_sqr(const Self& v) const
    {
        return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z);
    }

    // Distance calculation
    ICF T distance_to(const Self& v) const
    {
        return  XrMath::sqrt(distance_to_sqr(v));
    }

    // Barycentric coords
    IC SelfRef from_bary(const Self& V1, const Self& V2, const Self& V3, T u, T v, T w)
    {
        x = V1.x*u + V2.x*v + V3.x*w;
        y = V1.y*u + V2.y*v + V3.y*w;
        z = V1.z*u + V2.z*v + V3.z*w;
        return *this;
    }
    IC SelfRef from_bary(const Self& V1, const Self& V2, const Self& V3, const Self& B)
    {
        from_bary(V1, V2, V3, B.x, B.y, B.z);
        return *this;
    }

    IC SelfRef from_bary4(const Self& V1, const Self& V2, const Self& V3, const Self& V4, T u, T v, T w, T t)
    {
        x = V1.x*u + V2.x*v + V3.x*w + V4.x*t;
        y = V1.y*u + V2.y*v + V3.y*w + V4.y*t;
        z = V1.z*u + V2.z*v + V3.z*w + V4.z*t;
        return *this;
    }

    IC SelfRef mknormal_non_normalized(const Self& p0, const Self& p1, const Self& p2)
    {
        _vector3<T> v01, v12;
        v01.sub(p1, p0);
        v12.sub(p2, p1);
        crossproduct(v01, v12);
        return *this;
    };
    IC SelfRef mknormal(const Self& p0, const Self& p1, const Self& p2)
    {
        mknormal_non_normalized(p0, p1, p2);
        normalize_safe();
        return *this;
    };
    IC SelfRef setHP(T h, T p)
    {
        T _ch = XrMath::cos(h), _cp = XrMath::cos(p), _sh = XrMath::sin(h), _sp = XrMath::sin(p);
        x = -_cp*_sh;
        y = _sp;
        z = _cp*_ch;
        return *this;
    }
    ICF void getHP(T& h, T& p) const
    {
        float hyp;

        if (XrMath::fis_zero(x) && XrMath::fis_zero(z))
        {
            h = 0.0f;
            if (!XrMath::fis_zero(float(y))) p = (y > 0.0f) ? XrMath::PI_DIV_2 : -XrMath::PI_DIV_2;
            else p = 0.0f;
        }
        else
        {
            if (XrMath::fis_zero(z)) h = (x > 0.0f) ? -XrMath::PI_DIV_2 : XrMath::PI_DIV_2;
            else if (z < 0.0f) h = -(atanf(x / z) - XrMath::M_PI);
            else h = -atanf(x / z);
            hyp = XrMath::sqrt(x*x + z*z);
            if (XrMath::fis_zero(float(hyp))) p = (y > 0.0f) ? XrMath::PI_DIV_2 : -XrMath::PI_DIV_2;
            else p = atanf(y / hyp);
        }
    }
    ICF float getH() const
    {
        if (XrMath::fis_zero(x) && XrMath::fis_zero(z))
        {
            return 0.0f;
        }
        else
        {
            if (XrMath::fis_zero(z)) return (x > 0.0f) ? -XrMath::PI_DIV_2 : XrMath::PI_DIV_2;
            else if (z < 0.0f) return -(atanf(x / z) - XrMath::M_PI);
            else return -atanf(x / z);
        }
    }
    ICF float getP() const
    {
        if (XrMath::fis_zero(x) && XrMath::fis_zero(z))
        {
            if (!XrMath::fis_zero(float(y))) return (y > 0.0f) ? XrMath::PI_DIV_2 : -XrMath::PI_DIV_2;
            else return 0.0f;
        }
        else
        {
            float hyp = XrMath::sqrt(x*x + z*z);
            if (XrMath::fis_zero(float(hyp))) return (y > 0.0f) ? XrMath::PI_DIV_2 : -XrMath::PI_DIV_2;
            else return atanf(y / hyp);
        }
    }
    IC SelfRef reflect(const Self& dir, const Self& norm)
    {
        return mad(dir, norm, -2 * dir.dotproduct(norm));
    }
    IC SelfRef slide(const Self& dir, const Self& norm)
    {
        // non normalized
        return mad(dir, norm, -dir.dotproduct(norm));
    }
    IC static void generate_orthonormal_basis(const _vector3<T>& dir, _vector3<T>& up, _vector3<T>& right)
    {
        T fInvLength;

        if (XrMath::abs(dir.x) >= XrMath::abs(dir.y))
        {
            // W.x or W.z is the largest magnitude component, swap them
            fInvLength = 1.f / XrMath::sqrt(dir.x*dir.x + dir.z*dir.z);
            up.x = -dir.z*fInvLength;
            up.y = 0.0f;
            up.z = +dir.x*fInvLength;
        }
        else
        {
            // W.y or W.z is the largest magnitude component, swap them
            fInvLength = 1.f / XrMath::sqrt(dir.y*dir.y + dir.z*dir.z);
            up.x = 0.0f;
            up.y = +dir.z*fInvLength;
            up.z = -dir.y*fInvLength;
        }

        right.crossproduct(up, dir); //. <->
    }
    IC static void generate_orthonormal_basis_normalized(_vector3<T>& dir, _vector3<T>& up, _vector3<T>& right)
    {
        T fInvLength;
        dir.normalize();
        if (XrMath::fsimilar(dir.y, 1.f, XrMath::EPS))
        {
            up.set(0.f, 0.f, 1.f);
            fInvLength = 1.f / XrMath::sqrt(dir.x*dir.x + dir.y*dir.y);
            // cross (up,dir) and normalize (right)
            right.x = -dir.y * fInvLength;
            right.y = dir.x * fInvLength;
            right.z = 0.f;
            // cross (dir,right)
            up.x = -dir.z * right.y;
            up.y = dir.z * right.x;
            up.z = dir.x * right.y - dir.y * right.x;
        }
        else
        {
            up.set(0.f, 1.f, 0.f);
            fInvLength = 1.f / XrMath::sqrt(dir.x*dir.x + dir.z*dir.z);
            // cross (up,dir) and normalize (right)
            right.x = dir.z * fInvLength;
            right.y = 0.f;
            right.z = -dir.x * fInvLength;
            // cross (dir,right)
            up.x = dir.y * right.z;
            up.y = dir.z * right.x - dir.x * right.z;
            up.z = -dir.y * right.x;
        }
    }
};
typedef _vector3<float> Fvector;
typedef _vector3<float> Fvector3;
typedef _vector3<double> Dvector;
typedef _vector3<double> Dvector3;
typedef _vector3<s32> Ivector;
typedef _vector3<s32> Ivector3;

template <class T>
BOOL _valid(const _vector3<T>& v) { return _valid((T)v.x) && _valid((T)v.y) && _valid((T)v.z); }

//////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4244)

IC BOOL exact_normalize(float* a)
{
    double sqr_magnitude = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    double epsilon = 1.192092896e-05F;
    if (sqr_magnitude > epsilon)
    {
        double l =XrMath::rsqrt(sqr_magnitude);
        a[0] *= l;
        a[1] *= l;
        a[2] *= l;
        return TRUE;
    }
    double a0, a1, a2, aa0, aa1, aa2, l;
    a0 = a[0];
    a1 = a[1];
    a2 = a[2];
    aa0 = XrMath::abs(a0);
    aa1 = XrMath::abs(a1);
    aa2 = XrMath::abs(a2);
    if (aa1 > aa0)
    {
        if (aa2 > aa1)
        {
            goto aa2_largest;
        }
        else   // aa1 is largest
        {
            a0 /= aa1;
            a2 /= aa1;
            l = XrMath::rsqrt(a0*a0 + a2*a2 + 1);
            a[0] = a0*l;
            a[1] = (double)XrMath::copysign(l, a1);
            a[2] = a2*l;
        }
    }
    else
    {
        if (aa2 > aa0)
        {
        aa2_largest: // aa2 is largest
            a0 /= aa2;
            a1 /= aa2;
            l = XrMath::rsqrt(a0*a0 + a1*a1 + 1);
            a[0] = a0*l;
            a[1] = a1*l;
            a[2] = (double)XrMath::copysign(l, a2);
        }
        else   // aa0 is largest
        {
            if (aa0 <= 0)
            {
                // dDEBUGMSG ("vector has zero size"); ... this messace is annoying
                a[0] = 0; // if all a's are zero, this is where we'll end up.
                a[1] = 1; // return a default unit length vector.
                a[2] = 0;
                return FALSE;
            }
            a1 /= aa0;
            a2 /= aa0;
            l = XrMath::rsqrt(a1*a1 + a2*a2 + 1);
            a[0] = (double)XrMath::copysign(l, a0);
            a[1] = a1*l;
            a[2] = a2*l;
        }
    }
    return TRUE;
}
IC BOOL exact_normalize(Fvector3& a) { return exact_normalize(&a.x); }
#pragma warning(pop)

#endif
