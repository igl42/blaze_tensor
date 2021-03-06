//=================================================================================================
/*!
//  \file blaze_tensor/math/traits/SubtensorTrait.h
//  \brief Header file for the subtensor trait
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

#ifndef _BLAZE_TENSOR_MATH_TRAITS_SUBTENSORTRAIT_H_
#define _BLAZE_TENSOR_MATH_TRAITS_SUBTENSORTRAIT_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <utility>
#include <blaze/math/Infinity.h>
#include <blaze/util/InvalidType.h>
#include <blaze/util/Types.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename, size_t... > struct SubtensorTrait;
template< typename, size_t, size_t, size_t, size_t, size_t, size_t, typename = void > struct SubtensorTraitEval1;
template< typename, size_t, size_t, size_t, size_t, size_t, size_t, typename = void > struct SubtensorTraitEval2;
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< size_t K, size_t I, size_t J, size_t O, size_t M, size_t N, typename T >
auto evalSubtensorTrait( T& )
   -> typename SubtensorTraitEval1<T,K,I,J,O,M,N>::Type;

template< typename T >
auto evalSubtensorTrait( T& )
   -> typename SubtensorTraitEval2<T,inf,inf,inf,inf,inf,inf>::Type;

template< size_t K, size_t I, size_t J, size_t O, size_t M, size_t N, typename T >
auto evalSubtensorTrait( const T& )
   -> typename SubtensorTrait<T,K,I,J,O,M,N>::Type;

template< typename T >
auto evalSubtensorTrait( const T& )
   -> typename SubtensorTrait<T>::Type;

template< size_t K, size_t I, size_t J, size_t O, size_t M, size_t N, typename T >
auto evalSubtensorTrait( const volatile T& )
   -> typename SubtensorTrait<T,K,I,J,O,M,N>::Type;

template< typename T >
auto evalSubtensorTrait( const volatile T& )
   -> typename SubtensorTrait<T>::Type;
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Base template for the SubtensorTrait class.
// \ingroup math_traits
//
// \section subtensortrait_general General
//
// The SubtensorTrait class template offers the possibility to select the resulting data type
// when creating a subtensor of a dense or sparse tensor. SubtensorTrait defines the nested
// type \a Type, which represents the resulting data type of the subtensor operation. In case
// the given data type is not a dense or sparse tensor type, the resulting data type \a Type
// is set to \a INVALID_TYPE. Note that \a const and \a volatile qualifiers and reference
// modifiers are generally ignored.
//
//
// \section subtensortrait_specializations Creating custom specializations
//
// Per default, SubtensorTrait supports all tensor types of the Blaze library (including views
// and adaptors). For all other data types it is possible to specialize the SubtensorTrait
// template. The following example shows the according specialization for the DynamicMatrix
// class template:

   \code
   template< typename T1, bool SO >
   struct SubtensorTrait< DynamicMatrix<T1,SO> >
   {
      using Type = DynamicMatrix<T1,SO>;
   };
   \endcode

// \n \section subtensortrait_examples Examples
//
// The following example demonstrates the use of the SubtensorTrait template, where depending
// on the given tensor type the according result type is selected:

   \code
   using blaze::rowMajor;
   using blaze::columnMajor;

   // Definition of the result type of a row-major dynamic tensor
   using MatrixType1 = blaze::DynamicMatrix<int,rowMajor>;
   using ResultType1 = typename blaze::SubtensorTrait<MatrixType1>::Type;

   // Definition of the result type for the inner four elements of a 4x4 column-major static tensor
   using MatrixType2 = blaze::StaticMatrix<int,4UL,4UL,columnMajor>;
   using ResultType2 = typename blaze::SubtensorTrait<MatrixType2,1UL,1UL,2UL,2UL>::Type;
   \endcode
*/
template< typename MT       // Type of the tensor
        , size_t... CSAs >  // Compile time subtensor arguments
struct SubtensorTrait
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   using Type = decltype( evalSubtensorTrait<CSAs...>( std::declval<MT&>() ) );
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Auxiliary alias declaration for the SubtensorTrait type trait.
// \ingroup math_traits
//
// The SubtensorTrait_t alias declaration provides a convenient shortcut to access the nested
// \a Type of the SubtensorTrait class template. For instance, given the tensor type \a MT the
// following two type definitions are identical:

   \code
   using Type1 = typename blaze::SubtensorTrait<MT>::Type;
   using Type2 = blaze::SubtensorTrait_t<MT>;
   \endcode
*/
template< typename MT       // Type of the tensor
        , size_t... CSAs >  // Compile time subtensor arguments
using SubtensorTrait_t = typename SubtensorTrait<MT,CSAs...>::Type;
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief First auxiliary helper struct for the SubtensorTrait type trait.
// \ingroup math_traits
*/
template< typename MT  // Type of the tensor
        , size_t K     // Index of the first page
        , size_t I     // Index of the first row
        , size_t J     // Index of the first column
        , size_t O     // Number of pages
        , size_t M     // Number of rows
        , size_t N     // Number of columns
        , typename >   // Restricting condition
struct SubtensorTraitEval1
{
 public:
   //**********************************************************************************************
   using Type = typename SubtensorTraitEval2<MT,K,I,J,O,M,N>::Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Second auxiliary helper struct for the SubtensorTrait type trait.
// \ingroup math_traits
*/
template< typename MT  // Type of the tensor
        , size_t K     // Index of the first page
        , size_t I     // Index of the first row
        , size_t J     // Index of the first column
        , size_t O     // Number of pages
        , size_t M     // Number of rows
        , size_t N     // Number of columns
        , typename >   // Restricting condition
struct SubtensorTraitEval2
{
 public:
   //**********************************************************************************************
   using Type = INVALID_TYPE;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
