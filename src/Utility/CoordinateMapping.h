// $Id$
//==============================================================================
//!
//! \file CoordinateMapping.h
//!
//! \date May 18 2010
//!
//! \author Knut Morten Okstad / SINTEF
//!
//! \brief Utilities for coordinate mapping transformations.
//!
//==============================================================================

#ifndef _COORDINATE_MAPPING_H
#define _COORDINATE_MAPPING_H

#include "matrix.h"

class Vec3;


namespace utl
{
  //! \brief Set up the Jacobian matrix of the coordinate mapping.
  //! \param[out] J The inverse of the Jacobian matrix
  //! \param[out] dNdX First order derivatives of basis functions, w.r.t. X
  //! \param[in] X Matrix of element nodal coordinates
  //! \param[in] dNdu First order derivatives of basis functions
  //! \param[in] computeGradient If \e false, skip calculation of \a dNdX
  //! \return The Jacobian determinant
  Real Jacobian(matrix<Real>& J, matrix<Real>& dNdX,
                const matrix<Real>& X, const matrix<Real>& dNdu,
                bool computeGradient = true);

  //! \brief Set up the Jacobian matrix of the coordinate mapping along an edge.
  //! \param[out] J The inverse of the Jacobian matrix
  //! \param[out] t Unit tangent vector along the edge
  //! \param[out] dNdX 1st order derivatives of basis functions, w.r.t. X
  //! \param[in] X Matrix of element nodal coordinates
  //! \param[in] dNdu First order derivatives of basis functions
  //! \param[in] tangent Parametric tangent direction along the edge
  //! \return The curve dilation of the edge
  Real Jacobian(matrix<Real>& J, Vec3& t, matrix<Real>& dNdX,
                const matrix<Real>& X, const matrix<Real>& dNdu,
                size_t tangent);

  //! \brief Set up the Jacobian matrix of the coordinate mapping on a boundary.
  //! \param[out] J The inverse of the Jacobian matrix
  //! \param[out] n Outward-directed unit normal vector on the boundary
  //! \param[out] dNdX 1st order derivatives of basis functions, w.r.t. X
  //! \param[in] X Matrix of element nodal coordinates
  //! \param[in] dNdu First order derivatives of basis functions
  //! \param[in] t1 First parametric tangent direction of the boundary
  //! \param[in] t2 Second parametric tangent direction of the boundary
  //! \return The surface/curve dilation of the boundary
  Real Jacobian(matrix<Real>& J, Vec3& n, matrix<Real>& dNdX,
                const matrix<Real>& X, const matrix<Real>& dNdu,
                size_t t1, size_t t2);

  //! \brief Set up the Hessian matrix of the coordinate mapping.
  //! \param[out] H The Hessian matrix
  //! \param[out] d2NdX2 Second order derivatives of basis functions, w.r.t. X
  //! \param[in] Ji The inverse of the Jacobian matrix
  //! \param[in] X Matrix of element nodal coordinates
  //! \param[in] d2Ndu2 Second order derivatives of basis functions
  //! \param[in] dNdX First order derivatives of basis functions
  //! \param[in] geoMapping If true, calculate geometry mapping
  //! \return \e false if matrix dimensions are incompatible, otherwise \e true
  //! \details If geoMapping is true, H is output, else H is input and assumed to be already calculated.
  bool Hessian(matrix3d<Real>& H, matrix3d<Real>& d2NdX2,
               const matrix<Real>& Ji, const matrix<Real>& X,
               const matrix3d<Real>& d2Ndu2, const matrix<Real>& dNdX,
               bool geoMapping=true);

  //! \brief Compute the stabilization matrix \b G from the Jacobian inverse.
  //! \param[in] Ji The inverse of the Jacobian matrix
  //! \param[in] du Element lengths in each parametric direction
  //! \param[out] G The stabilization matrix (used in CFD simulators)
  void getGmat(const matrix<Real>& Ji, const Real* du, matrix<Real>& G);
}

#endif
