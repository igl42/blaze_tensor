//=================================================================================================
/*!
//  \file blaze_tensor/math/typetraits/IsSubtensor.h
//  \brief Header file for the IsSubtensor type trait
//
//  Copyright (C) 2012-2018 Klaus Iglberger - All Rights Reserved
//  Copyright (C) 2018 Hartmut Kaiser - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
*/
//=================================================================================================

#ifndef _BLAZE_TENSOR_MATH_TYPETRAITS_ISSUBTENSOR_H_
#define _BLAZE_TENSOR_MATH_TYPETRAITS_ISSUBTENSOR_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/util/IntegralConstant.h>

#include <blaze_tensor/math/views/Forward.h>

namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Compile time check for submatrices.
// \ingroup math_type_traits
//
// This type trait tests whether or not the given template parameter is a subtensor (i.e. a view
// on the part of a dense or sparse tensor). In case the type is a subtensor, the \a value member
// constant is set to \a true, the nested type definition \a Type is \a TrueType, and the class
// derives from \a TrueType. Otherwise \a value is set to \a false, \a Type is \a FalseType, and
// the class derives from \a FalseType.

   \code
   using blaze::aligned;

   using MatrixType1 = blaze::StaticMatrix<int,10UL,16UL>;
   using MatrixType2 = blaze::DynamicMatrix<double>;
   using MatrixType3 = blaze::CompressedMatrix<float>;

   MatrixType1 A;
   MatrixType2 B( 100UL, 200UL );
   MatrixType3 C( 200UL, 250UL );

   using SubtensorType1 = decltype( blaze::subtensor<2UL,2UL,4UL,8UL>( A ) );
   using SubtensorType2 = decltype( blaze::subtensor<aligned>( B, 24UL, 8UL, 8UL, 32UL ) );
   using SubtensorType3 = decltype( blaze::subtensor( C, 13UL, 5UL, 7UL, 17UL ) );

   blaze::IsSubtensor< SubtensorType1 >::value       // Evaluates to 1
   blaze::IsSubtensor< const SubtensorType2 >::Type  // Results in TrueType
   blaze::IsSubtensor< volatile SubtensorType3 >     // Is derived from TrueType
   blaze::IsSubtensor< MatrixType1 >::value          // Evaluates to 0
   blaze::IsSubtensor< const MatrixType2 >::Type     // Results in FalseType
   blaze::IsSubtensor< volatile MatrixType3 >        // Is derived from FalseType
   \endcode
*/
template< typename T >
struct IsSubtensor
   : public FalseType
{};
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the IsSubtensor type trait for 'Subtensor'.
// \ingroup math_type_traits
*/
template< typename MT, AlignmentFlag AF, size_t... CSAs >
struct IsSubtensor< Subtensor<MT,AF,CSAs...> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the IsSubtensor type trait for 'const Subtensor'.
// \ingroup math_type_traits
*/
template< typename MT, AlignmentFlag AF, size_t... CSAs >
struct IsSubtensor< const Subtensor<MT,AF,CSAs...> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the IsSubtensor type trait for 'volatile Subtensor'.
// \ingroup math_type_traits
*/
template< typename MT, AlignmentFlag AF, size_t... CSAs >
struct IsSubtensor< volatile Subtensor<MT,AF,CSAs...> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the IsSubtensor type trait for 'const volatile Subtensor'.
// \ingroup math_type_traits
*/
template< typename MT, AlignmentFlag AF, size_t... CSAs >
struct IsSubtensor< const volatile Subtensor<MT,AF,CSAs...> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Auxiliary variable template for the IsSubtensor type trait.
// \ingroup type_traits
//
// The IsSubtensor_v variable template provides a convenient shortcut to access the nested
// \a value of the IsSubtensor class template. For instance, given the type \a T the following
// two statements are identical:

   \code
   constexpr bool value1 = blaze::IsSubtensor<T>::value;
   constexpr bool value2 = blaze::IsSubtensor_v<T>;
   \endcode
*/
template< typename T >
constexpr bool IsSubtensor_v = IsSubtensor<T>::value;
//*************************************************************************************************

} // namespace blaze

#endif
