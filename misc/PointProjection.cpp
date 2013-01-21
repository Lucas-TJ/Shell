//
// Class for performing projection of points onto triangular surface
//

#include "../initPluginShells.h"
#include "PointProjection.inl"
//#include <sofa/core/ObjectFactory.h>

namespace sofa
{

//SOFA_DECL_CLASS(PointProjection)

#ifndef SOFA_FLOAT
template class SOFA_SHELLS_API PointProjection<double>;
#endif //SOFA_FLOAT
#ifndef SOFA_DOUBLE
template class SOFA_SHELLS_API PointProjection<float>;
#endif //SOFA_DOUBLE

}
