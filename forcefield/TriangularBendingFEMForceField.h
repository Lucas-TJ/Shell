/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_FORCEFIELD_TRIANGULAR_BENDING_FEM_FORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_TRIANGULAR_BENDING_FEM_FORCEFIELD_H

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/core/componentmodel/behavior/ForceField.h>
#include <sofa/core/componentmodel/topology/BaseMeshTopology.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/component/topology/TriangleData.h>
#include <newmat/newmat.h>
#include <newmat/newmatap.h>

#include <sofa/helper/system/thread/CTime.h>

#include <sofa/helper/gl/BasicShapes.h>

namespace sofa
{

namespace component
{

namespace forcefield
{

using namespace sofa::defaulttype;
using sofa::helper::vector;
using namespace sofa::component::topology;
using namespace sofa::core::componentmodel::behavior;

/// This class can be overridden if needed for additionnal storage within template specializations.
template<class DataTypes>
class TriangularBendingFEMForceFieldInternalData
{
public:
};


template<class DataTypes>
class TriangularBendingFEMForceField : public core::componentmodel::behavior::ForceField<DataTypes>, public virtual core::objectmodel::BaseObject
{
public:
        SOFA_CLASS(SOFA_TEMPLATE(TriangularBendingFEMForceField,DataTypes), SOFA_TEMPLATE(core::componentmodel::behavior::ForceField,DataTypes));

        typedef core::componentmodel::behavior::ForceField<DataTypes> Inherited;
	typedef typename DataTypes::VecCoord VecCoord;
	typedef typename DataTypes::VecDeriv VecDeriv;
	typedef typename DataTypes::VecReal VecReal;
	typedef VecCoord Vector;
	typedef typename DataTypes::Coord    Coord   ;
	typedef typename DataTypes::Deriv    Deriv   ;
	typedef typename Coord::value_type   Real    ;
	typedef Vec<3,Real> Vec3;

	typedef sofa::core::componentmodel::topology::BaseMeshTopology::index_type Index;
	typedef sofa::core::componentmodel::topology::BaseMeshTopology::Triangle Triangle;
	typedef sofa::core::componentmodel::topology::BaseMeshTopology::SeqTriangles SeqTriangles;

protected:

	typedef Vec<6, Real> Displacement;                                      ///< the displacement vector
        typedef Vec<9, Real> DisplacementBending;                               ///< the displacement vector for bending
	typedef Mat<3, 3, Real> MaterialStiffness;				///< the matrix of material stiffness
	typedef Mat<6, 3, Real> StrainDisplacement;				///< the strain-displacement matrix
        typedef Mat<3, 9, Real> StrainDisplacementBending;
	typedef Mat<3, 3, Real > Transformation;				///< matrix for rigid transformations like rotations
        typedef Mat<6, 6, Real> StiffnessMatrix;
        typedef Mat<9, 9, Real> StiffnessMatrixBending;
        typedef Mat<18, 18, Real> StiffnessMatrixGlobalSpace;

	sofa::core::componentmodel::topology::BaseMeshTopology* _topology;
        sofa::core::componentmodel::topology::BaseMeshTopology* _topologyHigh;

        TriangularBendingFEMForceFieldInternalData<DataTypes> data;
        friend class TriangularBendingFEMForceFieldInternalData<DataTypes>;

public:

    	class TriangleInformation
        {
            public:

                helper::fixed_array <Vec3, 2> restLocalPositions;
                helper::fixed_array <Quat, 3> restLocalOrientations;

                /// material stiffness matrices of each tetrahedron
                MaterialStiffness materialMatrix;
                // the strain-displacement matrices vector
                StrainDisplacement strainDisplacementMatrix;
                // the strain-displacement matrices vector
                StrainDisplacementBending strainDisplacementMatrix1;
                StrainDisplacementBending strainDisplacementMatrix2;
                StrainDisplacementBending strainDisplacementMatrix3;
                // Indices of each vertex
                Index a, b, c;
                // Local coordinates
                Vec3 localB, localC;
                // Transformation rotation;
                Quat Qframe;
                // Stiffness matrix K = J * M * Jt
                StiffnessMatrix stiffnessMatrix;
                // Stiffness matrix for bending K = Jt * M * J
                StiffnessMatrixBending stiffnessMatrixBending;
                
                // Third of the surface
                Real thirdSurface;
                // Variables needed for drawing the shell
                Vec<9, Real> u; // displacement vector
                Mat<9, 9, Real> invC; // inverse of C (used in bending mode only)
                Vec<9, Real> coefficients; // coefficients Ci computed from tinfo->invC * (tinfo->u + tinfo->u_flat)
                Vec <9, Real> u_rest; // difference between the initial position and the flate position to allow the use of an initial deformed shape

                TriangleInformation() { }

                /// Output stream
                inline friend std::ostream& operator<< ( std::ostream& os, const TriangleInformation& /*ti*/ )
                {
                    return os;
                }

                /// Input stream
                inline friend std::istream& operator>> ( std::istream& in, TriangleInformation& /*ti*/ )
                {
                    return in;
                }
        };

        TriangularBendingFEMForceField();

	virtual ~TriangularBendingFEMForceField();
	virtual void init();
	virtual void reinit();
	virtual void addForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v);
	virtual void addDForce (VecDeriv& df, const VecDeriv& dx);
        virtual void addKToMatrix(sofa::defaulttype::BaseMatrix *mat, SReal /*k*/, unsigned int &offset);
	virtual double getPotentialEnergy(const VecCoord& x);
	virtual void handleTopologyChange();

        sofa::core::componentmodel::topology::BaseMeshTopology* getTopology() {return _topology;}
        TriangleData<TriangleInformation>& getTriangleInfo() {return triangleInfo;}

	Data<Real> f_poisson;
	Data<Real> f_young;
        Data<bool> f_bending;
        Data <Real> f_thickness;
        Data <Real> f_membraneRatio;
        Data <Real> f_bendingRatio;
        Data<bool> refineMesh;
        Data<VecCoord> targetVertices;
        Data<SeqTriangles> targetTriangles;

protected :

        TriangleData<TriangleInformation> triangleInfo;

	void computeDisplacement(Displacement &Disp, const VecCoord &x, const Index elementIndex);
        void computeDisplacementBending(DisplacementBending &Disp, const VecCoord &x, const Index elementIndex);
	void computeStrainDisplacementMatrix(StrainDisplacement &J, const Vec3& b, const Vec3& c);
        void computeStrainDisplacementMatrixBending(TriangleInformation *tinfo, const Vec3& b, const Vec3& c);
        void tensorFlatPlate(Mat<3, 9, Real>& D, const Vec3 &P);
        void computeStiffnessMatrix(StiffnessMatrix &K, const StrainDisplacement &J, const MaterialStiffness &M);
        void computeStiffnessMatrixBending(StiffnessMatrixBending &K, TriangleInformation *tinfo);
	void computeForce(Displacement &F, const Displacement& D, const Index elementIndex);
        void computeForceBending(DisplacementBending &F, const DisplacementBending& D, const Index elementIndex);

	static void TRQSTriangleCreationFunction (int , void* , TriangleInformation &, const Triangle& , const sofa::helper::vector< unsigned int > &, const sofa::helper::vector< double >&);

	/// f += Kx where K is the stiffness matrix and x a displacement
	virtual void applyStiffness(VecDeriv& f, const VecDeriv& dx, const Index elementIndex);
	virtual void computeMaterialStiffness(const int i);

	void initTriangle(const int i, const Index&a, const Index&b, const Index&c);
	void computeRotation(Quat &Qframe, const VecCoord &p, const Index &a, const Index &b, const Index &c);
	void accumulateForce(VecDeriv& f, const VecCoord & p, const Index elementIndex);

        void convertStiffnessMatrixToGlobalSpace(StiffnessMatrixGlobalSpace &K_gs, TriangleInformation *tinfo);

        void testAddDforce(void);

        void generateCylinder(void);
        void refineCoarseMeshToTarget(void);
        void subdivide(const Vec3& a, const Vec3& b, const Vec3& c, sofa::helper::vector<Vec3> &subVertices, SeqTriangles &subTriangles);
        void addVertexAndFindIndex(sofa::helper::vector<Vec3> &subVertices, const Vec3 &vertex, int &index);
        void movePoint(Vec3& pointToMove);
        void FindClosestGravityPoints(const Vec3& point, sofa::helper::vector<Vec3>& listClosestPoints);
};


#if defined(WIN32) && !defined(SOFA_COMPONENT_FORCEFIELD_TRIANGULAR_BENDING_FEM_FORCEFIELD_CPP)
#pragma warning(disable : 4231)
#ifndef SOFA_FLOAT
extern template class TriangularBendingFEMForceField<defaulttype::Rigid3dTypes>;
#endif
#ifndef SOFA_DOUBLE
extern template class TriangularBendingFEMForceField<defaulttype::Rigid3fTypes>;
#endif
#endif

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif
