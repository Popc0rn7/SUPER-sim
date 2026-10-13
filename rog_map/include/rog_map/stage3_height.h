#pragma once
#include <cstdlib>
#include <limits>
#include <algorithm>
namespace stage3_height {
inline double low() { const char *p=std::getenv("SUPER_CENTER_Z_MIN"); return p ? std::atof(p) : -std::numeric_limits<double>::infinity(); }
inline double high() { const char *p=std::getenv("SUPER_CENTER_Z_MAX"); return p ? std::atof(p) : std::numeric_limits<double>::infinity(); }
inline bool allowed(double z) { return z >= low() && z <= high(); }
inline double cell_height(double z, double resolution) {
  if (z < low() && z + resolution*.5 > low()) return std::min(low()+.01,z+resolution*.5-.001);
  if (z > high() && z - resolution*.5 < high()) return std::max(high()-.01,z-resolution*.5+.001);
  return z;
}
template<class V> void clip(V &lo, V &hi) { lo.z()=std::max(lo.z(),low()); hi.z()=std::min(hi.z(),high()); }
}
