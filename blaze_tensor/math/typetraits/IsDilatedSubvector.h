//=================================================================================================
/*!
//  \file blaze_tensor/math/typetraits/IsDilatedSubvector.h
//  \brief Header file for the IsDilatedSubvector type trait
//
//  Copyright (C) 2012-2019 Klaus Iglberger - All Rights Reserved
//  Copyright (C) 2018-2019 Hartmut Kaiser - All Rights Reserved
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

#ifndef _BLAZE_TENSOR_MATH_TYPETRAITS_ISDILATEDSUBVECTOR_H_
#define _BLAZE_TENSOR_MATH_TYPETRAITS_ISDILATEDSUBVECTOR_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/views/Forward.h>
#include <blaze/util/FalseType.h>
#include <blaze/util/TrueType.h>

#include <blaze_tensor/math/views/Forward.h>

namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Compile time check for dilatedsubvectors.
// \ingroup math_type_traits
//
// This type trait tests whether or not the given template parameter is a dilatedsubvector (i.e. a view
// on the part of a dense or sparse vector). In case the type is a dilatedsubvector, the \a value member
// constant is set to \a true, the nested type definition \a Type is \a TrueType, and the class
// derives from \a TrueType. Otherwise \a value is set to \a false, \a Type is \a FalseType, and
// the class derives from \a FalseType.

   \code
   using VectorType1 = blaze::StaticVector<int,10UL>;
   using VectorType2 = blaze::DynamicVector<double>;
   using VectorType3 = blaze::CompressedVector<float>;

   VectorType1 a;
   VectorType2 b( 100UL );
   VectorType3 c( 200UL );

   using DilatedSubvectorType1 = decltype( blaze::dilatedsubvector<2UL,4UL,2UL>( a ) );
   using DilatedSubvectorType2 = decltype( blaze::dilatedsubvector( b, 8UL, 24UL, 2UL ) );
   using DilatedSubvectorType3 = decltype( blaze::dilatedsubvector( c, 5UL, 13UL, 3UL ) );

   blaze::IsDilatedSubvector< DilatedSubvectorType1 >::value       // Evaluates to 1
   blaze::IsDilatedSubvector< const DilatedSubvectorType2 >::Type  // Results in TrueType
   blaze::IsDilatedSubvector< volatile DilatedSubvectorType3 >     // Is derived from TrueType
   blaze::IsDilatedSubvector< VectorType1 >::value          // Evaluates to 0
   blaze::IsDilatedSubvector< const VectorType2 >::Type     // Results in FalseType
   blaze::IsDilatedSubvector< volatile VectorType3 >        // Is derived from FalseType
   \endcode
*/
template< typename T >
struct IsDilatedSubvector
   : public FalseType
{};
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the IsDilatedSubvector type trait for 'DilatedSubvector'.
// \ingroup math_type_traits
*/
template< typename VT, bool TF, bool DF, size_t... CSAs >
struct IsDilatedSubvector< DilatedSubvector<VT,TF,DF,CSAs...> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the IsDilatedSubvector type trait for 'const DilatedSubvector'.
// \ingroup math_type_traits
*/
template< typename VT, bool TF, bool DF, size_t... CSAs >
struct IsDilatedSubvector< const DilatedSubvector<VT,TF,DF,CSAs...> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the IsDilatedSubvector type trait for 'volatile DilatedSubvector'.
// \ingroup math_type_traits
*/
template< typename VT, bool TF, bool DF, size_t... CSAs >
struct IsDilatedSubvector< volatile DilatedSubvector<VT,TF,DF,CSAs...> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the IsDilatedSubvector type trait for 'const volatile DilatedSubvector'.
// \ingroup math_type_traits
*/
template< typename VT, bool TF, bool DF, size_t... CSAs >
struct IsDilatedSubvector< const volatile DilatedSubvector<VT,TF,DF,CSAs...> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Auxiliary variable template for the IsDilatedSubvector type trait.
// \ingroup type_traits
//
// The IsDilatedSubvector_v variable template provides a convenient shortcut to access the nested
// \a value of the IsDilatedSubvector class template. For instance, given the type \a T the following
// two statements are identical:

   \code
   constexpr bool value1 = blaze::IsDilatedSubvector<T>::value;
   constexpr bool value2 = blaze::IsDilatedSubvector_v<T>;
   \endcode
*/
template< typename T >
constexpr bool IsDilatedSubvector_v = IsDilatedSubvector<T>::value;
//*************************************************************************************************

} // namespace blaze

#endif
