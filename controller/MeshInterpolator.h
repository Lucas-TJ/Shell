#ifndef SOFA_COMPONENT_CONTROLLER_MESHINTERPOLATION_H
#define SOFA_COMPONENT_CONTROLLER_MESHINTERPOLATION_H

#include <sofa/component/component.h>
#include <sofa/component/controller/Controller.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/VecTypes.h>


namespace sofa
{

namespace component
{

namespace controller
{

template<class DataTypes>
class MeshInterpolator : public Controller
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(MeshInterpolator,DataTypes),Controller);

    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename Coord::value_type Real;

protected:

    MeshInterpolator();

    virtual ~MeshInterpolator();

public:

    virtual void init();
    virtual void reinit();

    virtual std::string getTemplateName() const
    {
        return templateName(this);
    }

    static std::string templateName(const MeshInterpolator<DataTypes>* = NULL)
    {
        return DataTypes::Name();
    }

    Data<Real>              f_startTime;
    Data<unsigned int>      f_nbSteps;
    Data<Real>              f_increment;
    Data<VecCoord>          f_startPosition;
    Data<VecCoord>          f_endPosition;

    Data<VecCoord>          f_position;

    virtual void onEndAnimationStep(const double dt);

private:

    unsigned int    stepCounter;
    Real            alpha;

    void interpolate();
};


#if defined(WIN32) && !defined(SOFA_COMPONENT_CONTROLLER_MESHINTERPOLATION_CPP)
#pragma warning(disable : 4231)
#ifndef SOFA_FLOAT
template class SOFA_SHELLS_API MeshInterpolator<defaulttype::Vec1dTypes>;
template class SOFA_SHELLS_API MeshInterpolator<defaulttype::Vec2dTypes>;
template class SOFA_SHELLS_API MeshInterpolator<defaulttype::Vec3dTypes>;
template class SOFA_SHELLS_API MeshInterpolator<defaulttype::Rigid2dTypes>;
template class SOFA_SHELLS_API MeshInterpolator<defaulttype::Rigid3dTypes>;
#endif //SOFA_FLOAT
#ifndef SOFA_DOUBLE
template class SOFA_SHELLS_API MeshInterpolator<defaulttype::Vec1fTypes>;
template class SOFA_SHELLS_API MeshInterpolator<defaulttype::Vec2fTypes>;
template class SOFA_SHELLS_API MeshInterpolator<defaulttype::Vec3fTypes>;
template class SOFA_SHELLS_API MeshInterpolator<defaulttype::Rigid2fTypes>;
template class SOFA_SHELLS_API MeshInterpolator<defaulttype::Rigid3fTypes>;
#endif //SOFA_DOUBLE

#endif
} // namespace controller

} // namespace component

} // namespace sofa

#endif //SOFA_COMPONENT_CONTROLLER_MESHINTERPOLATION_H
