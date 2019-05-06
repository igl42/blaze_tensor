//=================================================================================================
/*!
//  \file blaze_tensor/math/views/DilatedSubmatrix.h
//  \brief Header file for the implementation of the DilatedSubmatrix view
//
//  Copyright (C) 2012-2019 Klaus Iglberger - All Rights Reserved
//  Copyright (C) 2018-2019 Hartmut Kaiser - All Rights Reserved
//  Copyright (C) 2019 Bita Hasheminezhad - All Rights Reserved
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

#ifndef _BLAZE_TENSOR_MATH_VIEWS_DILATEDSUBMATRIX_H_
#define _BLAZE_TENSOR_MATH_VIEWS_DILATEDSUBMATRIX_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/Aliases.h>
#include <blaze/math/constraints/MutableDataAccess.h>
#include <blaze/math/constraints/RequiresEvaluation.h>
#include <blaze/math/expressions/DeclExpr.h>
#include <blaze/math/expressions/MatEvalExpr.h>
#include <blaze/math/expressions/MatMapExpr.h>
#include <blaze/math/expressions/MatMatAddExpr.h>
#include <blaze/math/expressions/MatMatMapExpr.h>
#include <blaze/math/expressions/MatMatMultExpr.h>
#include <blaze/math/expressions/MatMatSubExpr.h>
#include <blaze/math/expressions/Matrix.h>
#include <blaze/math/expressions/MatReduceExpr.h>
#include <blaze/math/expressions/MatScalarDivExpr.h>
#include <blaze/math/expressions/MatScalarMultExpr.h>
#include <blaze/math/expressions/MatSerialExpr.h>
#include <blaze/math/expressions/MatTransExpr.h>
#include <blaze/math/expressions/MatVecMultExpr.h>
#include <blaze/math/expressions/SchurExpr.h>
#include <blaze/math/expressions/TVecMatMultExpr.h>
#include <blaze/math/expressions/VecExpandExpr.h>
#include <blaze/math/expressions/VecTVecMultExpr.h>
#include <blaze/math/InversionFlag.h>
#include <blaze/math/ReductionFlag.h>
#include <blaze/math/shims/IsDefault.h>
#include <blaze/math/shims/Serial.h>
#include <blaze/math/typetraits/HasConstDataAccess.h>
#include <blaze/math/typetraits/HasMutableDataAccess.h>
#include <blaze/math/typetraits/IsHermitian.h>
#include <blaze/math/typetraits/IsLower.h>
#include <blaze/math/typetraits/IsRestricted.h>
#include <blaze/math/typetraits/IsStrictlyLower.h>
#include <blaze/math/typetraits/IsStrictlyUpper.h>
#include <blaze/math/typetraits/IsSymmetric.h>
#include <blaze/math/typetraits/IsUniLower.h>
#include <blaze/math/typetraits/IsUniUpper.h>
#include <blaze/math/typetraits/IsUpper.h>
#include <blaze/math/typetraits/Size.h>
#include <blaze/math/typetraits/TransposeFlag.h>
#include <blaze/math/views/Check.h>
#include <blaze/math/views/column/ColumnData.h>
#include <blaze/math/views/row/RowData.h>
#include <blaze/math/views/Subvector.h>
#include <blaze/util/algorithms/Max.h>
#include <blaze/util/algorithms/Min.h>
#include <blaze/util/Assert.h>
#include <blaze/util/DecltypeAuto.h>
#include <blaze/util/DisableIf.h>
#include <blaze/util/EnableIf.h>
#include <blaze/util/FunctionTrace.h>
#include <blaze/util/IntegralConstant.h>
#include <blaze/util/MaybeUnused.h>
#include <blaze/util/mpl/PtrdiffT.h>
#include <blaze/util/SmallArray.h>
#include <blaze/util/StaticAssert.h>
#include <blaze/util/TrueType.h>
#include <blaze/util/TypeList.h>
#include <blaze/util/Types.h>
#include <blaze/util/typetraits/IsPointer.h>
#include <blaze/util/typetraits/RemoveReference.h>

#include <blaze_tensor/math/views/DilatedSubvector.h>
#include <blaze_tensor/math/IntegerSequence.h>
#include <blaze_tensor/math/views/Forward.h>
#include <blaze_tensor/math/views/dilatedsubmatrix/BaseTemplate.h>
#include <blaze_tensor/math/views/dilatedsubmatrix/Dense.h>
// #include <blaze_tensor/math/views/DilatedSubmatrix/Sparse.h>

namespace blaze {

//=================================================================================================
//
//  GLOBAL FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix.
// \ingroup DilatedSubmatrix
//
// \param matrix The matrix containing the DilatedSubmatrix.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specific dilatedDilatedSubmatrix of the matrix.
// \exception std::invalid_argument Invalid dilatedDilatedSubmatrix specification.
//
// This function returns an expression representing the specified dilatedDilatedSubmatrix of the given matrix.
// The following example demonstrates the creation of a dense and sparse dilatedDilatedSubmatrix:

   \code
   blaze::DynamicMatrix<double,blaze::rowMajor> D;
   blaze::CompressedMatrix<int,blaze::columnMajor> S;
   // ... Resizing and initialization

   // Creating a dense DilatedSubmatrix of size 8x4, starting in row 0 and column 16
   auto dsm = DilatedSubmatrix<0UL,16UL,8UL,4UL>( D );

   // Creating a sparse DilatedSubmatrix of size 7x3, starting in row 2 and column 4
   auto ssm = DilatedSubmatrix<2UL,4UL,7UL,3UL>( S );
   \endcode

// By default, the provided DilatedSubmatrix arguments are checked at runtime. In case the DilatedSubmatrix
// is not properly specified (i.e. if the specified row or column is larger than the total number
// of rows or columns of the given matrix or the DilatedSubmatrix is specified beyond the number of rows
// or columns of the matrix) a \a std::invalid_argument exception is thrown. The checks can be
// skipped by providing the optional \a blaze::unchecked argument.

   \code
   auto dsm = DilatedSubmatrix<0UL,16UL,8UL,4UL>( D, unchecked );
   auto ssm = DilatedSubmatrix<2UL,4UL,7UL,3UL>( S, unchecked );
   \endcode

// Please note that this function creates an unaligned dense or sparse DilatedSubmatrix. For instance,
// the creation of the dense DilatedSubmatrix is equivalent to the following function call:

   \code
   auto dsm = DilatedSubmatrix<unaligned,0UL,16UL,8UL,4UL>( D );
   \endcode

// In contrast to unaligned submatrices, which provide full flexibility, aligned submatrices pose
// additional alignment restrictions. However, especially in case of dense submatrices this may
// result in considerable performance improvements. In order to create an aligned DilatedSubmatrix the
// following function call has to be used:

   \code
   auto dsm = DilatedSubmatrix<aligned,0UL,16UL,8UL,4UL>( D );
   \endcode

// Note however that in this case the given compile time arguments \a I, \a J, \a M, and \a N are
// subject to additional checks to guarantee proper alignment.
*/
template< size_t I               // Index of the first row
        , size_t J               // Index of the first column
        , size_t M               // Number of rows
        , size_t N               // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename MT            // Type of the dense matrix
        , bool SO                // Storage order
        , typename... RSAs >     // Optional DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( Matrix<MT,SO>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<I,J,M,N,RowDilation,ColumnDilation>( ~matrix, args... );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific DilatedSubmatrix of the given constant matrix.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix containing the DilatedSubmatrix.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specific DilatedSubmatrix of the matrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given constant
// matrix. The following example demonstrates the creation of a dense and sparse DilatedSubmatrix:

   \code
   const blaze::DynamicMatrix<double,blaze::rowMajor> D( ... );
   const blaze::CompressedMatrix<int,blaze::columnMajor> S( ... );

   // Creating a dense DilatedSubmatrix of size 8x4, starting in row 0 and column 16
   auto dsm = DilatedSubmatrix<0UL,16UL,8UL,4UL>( D );

   // Creating a sparse DilatedSubmatrix of size 7x3, starting in row 2 and column 4
   auto ssm = DilatedSubmatrix<2UL,4UL,7UL,3UL>( S );
   \endcode

// By default, the provided DilatedSubmatrix arguments are checked at runtime. In case the DilatedSubmatrix
// is not properly specified (i.e. if the specified row or column is larger than the total number
// of rows or columns of the given matrix or the DilatedSubmatrix is specified beyond the number of rows
// or columns of the matrix) a \a std::invalid_argument exception is thrown. The checks can be
// skipped by providing the optional \a blaze::unchecked argument.

   \code
   auto dsm = DilatedSubmatrix<0UL,16UL,8UL,4UL>( D, unchecked );
   auto ssm = DilatedSubmatrix<2UL,4UL,7UL,3UL>( S, unchecked );
   \endcode

// Please note that this function creates an unaligned dense or sparse DilatedSubmatrix. For instance,
// the creation of the dense DilatedSubmatrix is equivalent to the following three function calls:

   \code
   auto dsm = DilatedSubmatrix<unaligned,0UL,16UL,8UL,4UL>( D );
   \endcode

// In contrast to unaligned submatrices, which provide full flexibility, aligned submatrices pose
// additional alignment restrictions. However, especially in case of dense submatrices this may
// result in considerable performance improvements. In order to create an aligned DilatedSubmatrix the
// following function call has to be used:

   \code
   auto dsm = DilatedSubmatrix<aligned,0UL,16UL,8UL,4UL>( D );
   \endcode

// Note however that in this case the given compile time arguments \a I, \a J, \a M, and \a N are
// subject to additional checks to guarantee proper alignment.
*/
template< size_t I               // Index of the first row
        , size_t J               // Index of the first column
        , size_t M               // Number of rows
        , size_t N               // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename MT            // Type of the dense matrix
        , bool SO                // Storage order
        , typename... RSAs >     // Option DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const Matrix<MT,SO>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<I,J,M,N,RowDilation,ColumnDilation>( ~matrix, args... );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific DilatedSubmatrix of the given temporary matrix.
// \ingroup DilatedSubmatrix
//
// \param matrix The temporary matrix containing the DilatedSubmatrix.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specific DilatedSubmatrix of the matrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given
// temporary matrix. In case the DilatedSubmatrix is not properly specified (i.e. if the specified
// row or column is greater than the total number of rows or columns of the given matrix or
// the DilatedSubmatrix is specified beyond the number of rows or columns of the matrix) a
// \a std::invalid_argument exception is thrown.
*/
template< size_t I               // Index of the first row
        , size_t J               // Index of the first column
        , size_t M               // Number of rows
        , size_t N               // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename MT            // Type of the dense matrix
        , bool SO                // Storage order
        , typename... RSAs >     // Option DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( Matrix<MT,SO>&& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<I,J,M,N,RowDilation,ColumnDilation>( ~matrix, args... );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix.
// \ingroup DilatedSubmatrix
//
// \param matrix The matrix containing the DilatedSubmatrix.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specific DilatedSubmatrix of the matrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing an aligned or unaligned DilatedSubmatrix of the
// given dense or sparse matrix, based on the specified alignment flag \a AF. The following
// example demonstrates the creation of both an aligned and unaligned DilatedSubmatrix:

   \code
   blaze::DynamicMatrix<double,blaze::rowMajor> D;
   blaze::CompressedMatrix<int,blaze::columnMajor> S;
   // ... Resizing and initialization

   // Creating an aligned dense DilatedSubmatrix of size 8x4, starting in row 0 and column 16
   auto dsm = DilatedSubmatrix<aligned,0UL,16UL,8UL,4UL>( D );

   // Creating an unaligned sparse DilatedSubmatrix of size 7x3, starting in row 2 and column 4
   auto ssm = DilatedSubmatrix<unaligned,2UL,4UL,7UL,3UL>( S );
   \endcode

// By default, the provided DilatedSubmatrix arguments are checked at runtime. In case the DilatedSubmatrix
// is not properly specified (i.e. if the specified row or column is larger than the total number
// of rows or columns of the given matrix or the DilatedSubmatrix is specified beyond the number of rows
// or columns of the matrix) a \a std::invalid_argument exception is thrown. The checks can be
// skipped by providing the optional \a blaze::unchecked argument.

   \code
   auto dsm = DilatedSubmatrix<aligned,0UL,16UL,8UL,4UL>( D, unchecked );
   auto ssm = DilatedSubmatrix<unaligned,2UL,4UL,7UL,3UL>( S, unchecked );
   \endcode

// Please note that this function creates an unaligned dense or sparse DilatedSubmatrix. For instance,
// the creation of the dense DilatedSubmatrix is equivalent to the following function call:

*************************************************************************************************


*************************************************************************************************
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix.
// \ingroup DilatedSubmatrix
//
// \param matrix The matrix containing the DilatedSubmatrix.
// \param row The index of the first row of the DilatedSubmatrix.
// \param column The index of the first column of the DilatedSubmatrix.
// \param m The number of rows of the DilatedSubmatrix.
// \param n The number of columns of the DilatedSubmatrix.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specific DilatedSubmatrix of the matrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given matrix.
// The following example demonstrates the creation of a dense and sparse DilatedSubmatrix:

   \code
   blaze::DynamicMatrix<double,blaze::rowMajor> D;
   blaze::CompressedMatrix<int,blaze::columnMajor> S;
   // ... Resizing and initialization

   // Creating a dense DilatedSubmatrix of size 8x4, starting in row 0 and column 16
   auto dsm = DilatedSubmatrix( D, 0UL, 16UL, 8UL, 4UL );

   // Creating a sparse DilatedSubmatrix of size 7x3, starting in row 2 and column 4
   auto ssm = DilatedSubmatrix( S, 2UL, 4UL, 7UL, 3UL );
   \endcode

// By default, the provided DilatedSubmatrix arguments are checked at runtime. In case the DilatedSubmatrix
// is not properly specified (i.e. if the specified row or column is larger than the total number
// of rows or columns of the given matrix or the DilatedSubmatrix is specified beyond the number of rows
// or columns of the matrix) a \a std::invalid_argument exception is thrown. The checks can be
// skipped by providing the optional \a blaze::unchecked argument.

   \code
   auto dsm = DilatedSubmatrix( D, 0UL, 16UL, 8UL, 4UL, unchecked );
   auto ssm = DilatedSubmatrix( S, 2UL, 4UL, 7UL, 3UL, unchecked );
   \endcode

// Please note that this function creates an unaligned dense or sparse DilatedSubmatrix. For instance,
// the creation of the dense DilatedSubmatrix is equivalent to the following function call:

   \code
   unaligned dsm = DilatedSubmatrix<unaligned>( D, 0UL, 16UL, 8UL, 4UL );
   \endcode

// In contrast to unaligned submatrices, which provide full flexibility, aligned submatrices pose
// additional alignment restrictions. However, especially in case of dense submatrices this may
// result in considerable performance improvements. In order to create an aligned DilatedSubmatrix the
// following function call has to be used:

   \code
   auto dsm = DilatedSubmatrix<aligned>( D, 0UL, 16UL, 8UL, 4UL );
   \endcode

// Note however that in this case the given arguments \a row, \a column, \a m, and \a n are
// subject to additional checks to guarantee proper alignment.
*/
template< typename MT         // Type of the dense matrix
        , bool SO             // Storage order
        , typename... RSAs >  // Option DilatedSubmatrix arguments
inline decltype(auto)
   dilatedsubmatrix( Matrix<MT,SO>& matrix, size_t row, size_t column, size_t m, size_t n,
      size_t rowdilation, size_t columndilation, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   using ReturnType = DilatedSubmatrix_<MT>;
   return ReturnType( ~matrix, row, column, m, n, rowdilation, columndilation, args... );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific DilatedSubmatrix of the given constant matrix.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix containing the DilatedSubmatrix.
// \param row The index of the first row of the DilatedSubmatrix.
// \param column The index of the first column of the DilatedSubmatrix.
// \param m The number of rows of the DilatedSubmatrix.
// \param n The number of columns of the DilatedSubmatrix.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specific DilatedSubmatrix of the matrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given constant
// matrix. The following example demonstrates the creation of a dense and sparse DilatedSubmatrix:

   \code
   const blaze::DynamicMatrix<double,blaze::rowMajor> D( ... );
   const blaze::CompressedMatrix<int,blaze::columnMajor> S( ... );

   // Creating a dense DilatedSubmatrix of size 8x4, starting in row 0 and column 16
   auto dsm = DilatedSubmatrix( D, 0UL, 16UL, 8UL, 4UL );

   // Creating a sparse DilatedSubmatrix of size 7x3, starting in row 2 and column 4
   auto ssm = DilatedSubmatrix( S, 2UL, 4UL, 7UL, 3UL );
   \endcode

// By default, the provided DilatedSubmatrix arguments are checked at runtime. In case the DilatedSubmatrix
// is not properly specified (i.e. if the specified row or column is larger than the total number
// of rows or columns of the given matrix or the DilatedSubmatrix is specified beyond the number of rows
// or columns of the matrix) a \a std::invalid_argument exception is thrown. The checks can be
// skipped by providing the optional \a blaze::unchecked argument.

   \code
   auto dsm = DilatedSubmatrix( D, 0UL, 16UL, 8UL, 4UL, unchecked );
   auto ssm = DilatedSubmatrix( S, 2UL, 4UL, 7UL, 3UL, unchecked );
   \endcode

// Please note that this function creates an unaligned dense or sparse DilatedSubmatrix. For instance,
// the creation of the dense DilatedSubmatrix is equivalent to the following three function calls:

   \code
   auto dsm = DilatedSubmatrix<unaligned>( D, 0UL, 16UL, 8UL, 4UL );
   \endcode

// In contrast to unaligned submatrices, which provide full flexibility, aligned submatrices pose
// additional alignment restrictions. However, especially in case of dense submatrices this may
// result in considerable performance improvements. In order to create an aligned DilatedSubmatrix the
// following function call has to be used:

   \code
   auto dsm = DilatedSubmatrix<aligned>( D, 0UL, 16UL, 8UL, 4UL );
   \endcode

// Note however that in this case the given arguments \a row, \a column, \a m, and \a n are
// subject to additional checks to guarantee proper alignment.
*/
template< typename MT         // Type of the dense matrix
        , bool SO             // Storage order
        , typename... RSAs >  // Option DilatedSubmatrix arguments
inline decltype(auto)
   dilatedsubmatrix( const Matrix<MT,SO>& matrix, size_t row, size_t column, size_t m, size_t n,
      size_t rowdilation, size_t columndilation, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   using ReturnType = const DilatedSubmatrix_<const MT>;
   return ReturnType( ~matrix, row, column, m, n, rowdilation, columndilation, args... );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific DilatedSubmatrix of the given temporary matrix.
// \ingroup DilatedSubmatrix
//
// \param matrix The temporary matrix containing the DilatedSubmatrix.
// \param row The index of the first row of the DilatedSubmatrix.
// \param column The index of the first column of the DilatedSubmatrix.
// \param m The number of rows of the DilatedSubmatrix.
// \param n The number of columns of the DilatedSubmatrix.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specific DilatedSubmatrix of the matrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given
// temporary matrix. In case the DilatedSubmatrix is not properly specified (i.e. if the specified
// row or column is greater than the total number of rows or columns of the given matrix or
// the DilatedSubmatrix is specified beyond the number of rows or columns of the matrix) a
// \a std::invalid_argument exception is thrown.
*/
template< typename MT         // Type of the dense matrix
        , bool SO             // Storage order
        , typename... RSAs >  // Option DilatedSubmatrix arguments
inline decltype(auto)
   dilatedsubmatrix( Matrix<MT,SO>&& matrix, size_t row, size_t column, size_t m, size_t n,
      size_t rowdilation, size_t columndilation, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   using ReturnType = const DilatedSubmatrix_<const MT>;
   return ReturnType( ~matrix, row, column, m, n, rowdilation, columndilation, args... );
}

//=================================================================================================
//
//  GLOBAL RESTRUCTURING FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix/matrix addition.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix/matrix addition.
// \param args The runtime DilatedSubmatrix arguments
// \return View on the specified DilatedSubmatrix of the addition.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given
// matrix/matrix addition.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const MatMatAddExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<CSAs...>( (~matrix).leftOperand(), args... ) +
          dilatedsubmatrix<CSAs...>( (~matrix).rightOperand(), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix/matrix subtraction.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix/matrix subtraction.
// \param args The runtime DilatedSubmatrix arguments
// \return View on the specified DilatedSubmatrix of the subtraction.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given
// matrix/matrix subtraction.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const MatMatSubExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<CSAs...>( (~matrix).leftOperand(), args... ) -
          dilatedsubmatrix<CSAs...>( (~matrix).rightOperand(), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given Schur product.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant Schur product.
// \param args The runtime DilatedSubmatrix arguments
// \return View on the specified DilatedSubmatrix of the Schur product.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given Schur
// product.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const SchurExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<CSAs...>( (~matrix).leftOperand(), args... ) %
          dilatedsubmatrix<CSAs...>( (~matrix).rightOperand(), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix/matrix multiplication.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix/matrix multiplication.
// \param args The runtime DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the multiplication.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given
// matrix/matrix multiplication.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const MatMatMultExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   using MT1 = RemoveReference_t< LeftOperand_t< MatrixType_t<MT> > >;
   using MT2 = RemoveReference_t< RightOperand_t< MatrixType_t<MT> > >;

   const DilatedSubmatrixData<CSAs...> sd( args... );

   BLAZE_DECLTYPE_AUTO( left , (~matrix).leftOperand()  );
   BLAZE_DECLTYPE_AUTO( right, (~matrix).rightOperand() );

   const size_t begin( max( ( IsUpper_v<MT1> )
                            ?( ( !IsStrictlyUpper_v<MT1> )
                               ?( sd.row() + 1UL )
                               :( sd.row() ) )
                            :( 0UL )
                          , ( IsLower_v<MT2> )
                            ?( ( !IsStrictlyLower_v<MT2> )
                               ?( sd.column() + 1UL )
                               :( sd.column() ) )
                            :( 0UL ) ) );
   const size_t end( min( ( IsLower_v<MT1> )
                          ?( ( IsStrictlyLower_v<MT1> && sd.rows() > 0UL )
                             ?( sd.row() + sd.rows() - 1UL )
                             :( sd.row() + sd.rows() ) )
                          :( left.columns() )
                        , ( IsUpper_v<MT2> )
                          ?( ( IsStrictlyUpper_v<MT2> && sd.columns() > 0UL )
                             ?( sd.column() + sd.columns() - 1UL )
                             :( sd.column() + sd.columns() ) )
                          :( left.columns() ) ) );

   const size_t diff( ( begin < end )?( end - begin ):( 0UL ) );

   return dilatedsubmatrix( left, sd.row(), begin, sd.rows(), diff ) *
          dilatedsubmatrix( right, begin, sd.column(), diff, sd.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given outer product.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant outer product.
// \return View on the specified DilatedSubmatrix of the outer product.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given
// outer product.
*/
template< size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const VecTVecMultExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubvector<I,M,RowDilation>( (~matrix).leftOperand(), args... ) *
          dilatedsubvector<J,N,ColumnDilation>( (~matrix).rightOperand(), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given outer product.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant outer product.
// \param row The index of the first row of the DilatedSubmatrix.
// \param column The index of the first column of the DilatedSubmatrix.
// \param m The number of rows of the DilatedSubmatrix.
// \param n The number of columns of the DilatedSubmatrix.
// \return View on the specified DilatedSubmatrix of the outer product.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given
// outer product.
*/
template< typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto)
   dilatedsubmatrix( const VecTVecMultExpr<MT>& matrix, size_t row, size_t column, size_t m, size_t n,
      size_t rowdilation, size_t columndilation, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return diltedsubvector( (~matrix).leftOperand(), row, m, rowdilation, args... ) *
          diltedsubvector( (~matrix).rightOperand(), column, n, columndilation, args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix/scalar multiplication.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix/scalar multiplication.
// \param args The runtime DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the multiplication.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given
// matrix/scalar multiplication.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const MatScalarMultExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<CSAs...>( (~matrix).leftOperand(), args... ) * (~matrix).rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix/scalar division.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix/scalar division.
// \param args The runtime DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the division.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given
// matrix/scalar division.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const MatScalarDivExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<CSAs...>( (~matrix).leftOperand(), args... ) / (~matrix).rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given unary matrix map operation.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant unary matrix map operation.
// \param args The runtime DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the unary map operation.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given unary
// matrix map operation.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const MatMapExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return map( dilatedsubmatrix<CSAs...>( (~matrix).operand(), args... ), (~matrix).operation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given binary matrix map operation.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant binary matrix map operation.
// \param args The runtime DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the binary map operation.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given binary
// matrix map operation.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const MatMatMapExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return map( dilatedsubmatrix<CSAs...>( (~matrix).leftOperand(), args... ),
               dilatedsubmatrix<CSAs...>( (~matrix).rightOperand(), args... ),
               (~matrix).operation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix evaluation operation.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix evaluation operation.
// \param args The runtime DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the evaluation operation.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given matrix
// evaluation operation.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const MatEvalExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return eval( dilatedsubmatrix<CSAs...>( (~matrix).operand(), args... ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix serialization operation.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix serialization operation.
// \param args The runtime DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the serialization operation.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given matrix
// serialization operation.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const MatSerialExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return serial( dilatedsubmatrix<CSAs...>( (~matrix).operand(), args... ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix declaration operation.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix declaration operation.
// \param args The runtime DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the declaration operation.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given matrix
// declaration operation.
*/
template< size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Runtime DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const DeclExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<CSAs...>( (~matrix).operand(), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix transpose operation.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix transpose operation.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the transpose operation.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given matrix
// transpose operation.
*/
template< size_t I              // Index of the first row
        , size_t J              // Index of the first column
        , size_t M              // Number of rows
        , size_t N              // Number of columns
        , size_t RowDilation    // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation // Step between columns of the DilatedSubmatrix
        , typename MT           // Matrix base type of the expression
        , typename... RSAs >    // Optional DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const MatTransExpr<MT>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return trans( dilatedsubmatrix<J,I,N,M,ColumnDilation,RowDilation>( (~matrix).operand(), args... ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given matrix transpose operation.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant matrix transpose operation.
// \param row The index of the first row of the DilatedSubmatrix.
// \param column The index of the first column of the DilatedSubmatrix.
// \param m The number of rows of the DilatedSubmatrix.
// \param n The number of columns of the DilatedSubmatrix.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the transpose operation.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given matrix
// transpose operation.
*/
template< typename MT         // Matrix base type of the expression
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto)
   dilatedsubmatrix( const MatTransExpr<MT>& matrix, size_t row, size_t column, size_t m, size_t n,
      size_t rowdilation, size_t columndilation, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return trans( dilatedsubmatrix( (~matrix).operand(), column, row, n, m, columndilation, rowdilation, args... ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given vector expansion operation.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant vector expansion operation.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the expansion operation.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given vector
// expansion operation.
*/
template< size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename MT         // Matrix base type of the expression
        , size_t... CEAs      // Compile time expansion arguments
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const VecExpandExpr<MT,CEAs...>& matrix, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   using VT = VectorType_t< RemoveReference_t< decltype( (~matrix).operand() ) > >;

   constexpr bool TF( TransposeFlag_v<VT> );

   constexpr size_t index    ( TF ? J : I );
   constexpr size_t size     ( TF ? N : M );
   constexpr size_t expansion( TF ? M : N );
   constexpr size_t dilation ( TF ? ColumnDilation : RowDilation );

   return expand<expansion>( dilatedsubvector<index,size,dilation>( (~matrix).operand(), args... ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of the given vector expansion operation.
// \ingroup DilatedSubmatrix
//
// \param matrix The constant vector expansion operation.
// \param row The index of the first row of the DilatedSubmatrix.
// \param column The index of the first column of the DilatedSubmatrix.
// \param m The number of rows of the DilatedSubmatrix.
// \param n The number of columns of the DilatedSubmatrix.
// \param args Optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the expansion operation.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given vector
// expansion operation.
*/
template< typename MT         // Matrix base type of the expression
        , size_t... CEAs      // Compile time expansion arguments
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto)
   dilatedsubmatrix( const VecExpandExpr<MT,CEAs...>& matrix,
              size_t row, size_t column, size_t m, size_t n, size_t rowdilation, size_t columndilation, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   using VT = VectorType_t< RemoveReference_t< decltype( (~matrix).operand() ) > >;

   constexpr bool TF( TransposeFlag_v<VT> );

   const size_t index    ( TF ? column : row );
   const size_t size     ( TF ? n : m );
   const size_t expansion( TF ? m : n );
   const size_t dilation ( TF ? columndilation : rowdilation );

   return expand( dilatedsubvector( (~matrix).operand(), index, size, dilation, args... ), expansion );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of another DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The given DilatedSubmatrix
// \param args The optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the other DilatedSubmatrix.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given DilatedSubmatrix.
*/
template< size_t I1               // Required index of the first row
        , size_t J1               // Required index of the first column
        , size_t M1               // Required number of rows
        , size_t N1               // Required number of columns
        , size_t RowDilation1     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation1  // Step between columns of the DilatedSubmatrix
        , typename MT             // Type of the sparse DilatedSubmatrix
        , bool SO                 // Storage order
        , bool DF                 // Density flag
        , size_t I2               // Present index of the first row
        , size_t J2               // Present index of the first column
        , size_t M2               // Present number of rows
        , size_t N2               // Present number of columns
        , size_t RowDilation2     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation2  // Step between columns of the DilatedSubmatrix
        , typename... RSAs >      // Optional DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( DilatedSubmatrix<MT,SO,DF,I2,J2,M2,N2,RowDilation2,ColumnDilation2>& sm, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   BLAZE_STATIC_ASSERT_MSG( I1 + M1 * RowDilation1 <= M2 * RowDilation2, "Invalid DilatedSubmatrix specification" );
   BLAZE_STATIC_ASSERT_MSG( J1 + N1 * ColumnDilation1 <= N2 * ColumnDilation2, "Invalid DilatedSubmatrix specification" );

   return dilatedsubmatrix<I1*RowDilation2+I2,J1*ColumnDilation2+J2,M1,N1,RowDilation1*RowDilation2,
      ColumnDilation1*ColumnDilation2>( sm.operand(), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of another constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The given constant DilatedSubmatrix
// \param args The optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the other DilatedSubmatrix.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given constant
// DilatedSubmatrix.
*/
template< size_t I1           // Required index of the first row
        , size_t J1           // Required index of the first column
        , size_t M1           // Required number of rows
        , size_t N1           // Required number of columns
        , size_t RowDilation1     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation1  // Step between columns of the DilatedSubmatrix
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Present index of the first row
        , size_t J2           // Present index of the first column
        , size_t M2           // Present number of rows
        , size_t N2           // Present number of columns
        , size_t RowDilation2     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation2  // Step between columns of the DilatedSubmatrix
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const DilatedSubmatrix<MT,SO,DF,I2,J2,M2,N2,RowDilation2,ColumnDilation2>& sm, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   BLAZE_STATIC_ASSERT_MSG( I1 + M1 * RowDilation1 <= M2 * RowDilation2, "Invalid DilatedSubmatrix specification" );
   BLAZE_STATIC_ASSERT_MSG( J1 + N1 * ColumnDilation1 <= N2 * ColumnDilation2, "Invalid DilatedSubmatrix specification" );

   return dilatedsubmatrix<I1*RowDilation2+I2,J1*ColumnDilation2+J2,M1,N1,RowDilation1*RowDilation2,
      ColumnDilation1*ColumnDilation2>( sm.operand(), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of another temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The given temporary DilatedSubmatrix
// \param args The optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the other DilatedSubmatrix.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given temporary
// DilatedSubmatrix.
*/
template< size_t I1           // Required index of the first row
        , size_t J1           // Required index of the first column
        , size_t M1           // Required number of rows
        , size_t N1           // Required number of columns
        , size_t RowDilation1     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation1  // Step between columns of the DilatedSubmatrix
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Present index of the first row
        , size_t J2           // Present index of the first column
        , size_t M2           // Present number of rows
        , size_t N2           // Present number of columns
        , size_t RowDilation2     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation2  // Step between columns of the DilatedSubmatrix
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( DilatedSubmatrix<MT,SO,DF,I2,J2,M2,N2,RowDilation2,ColumnDilation2>&& sm, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   BLAZE_STATIC_ASSERT_MSG( I1 + M1 * RowDilation1 <= M2 * RowDilation2, "Invalid DilatedSubmatrix specification" );
   BLAZE_STATIC_ASSERT_MSG( J1 + N1 * ColumnDilation1 <= N2 * ColumnDilation2, "Invalid DilatedSubmatrix specification" );

   return dilatedsubmatrix<I1*RowDilation2+I2,J1*ColumnDilation2+J2,M1,N1,RowDilation1*RowDilation2,
      ColumnDilation1*ColumnDilation2>( sm.operand(), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of another DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The given DilatedSubmatrix
// \param args The optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the other DilatedSubmatrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given DilatedSubmatrix.
*/
template< size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( DilatedSubmatrix<MT,SO,DF>& sm, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RSAs...>, Unchecked > );

   if( isChecked ) {
      if( ( I + M * RowDilation > sm.rows( ) * sm.rowdilation( ) ) ||
         ( J + N * ColumnDilation > sm.columns( ) * sm.columndilation( ) ) )
      {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid DilatedSubmatrix specification" );
      }
   }
   else {
      BLAZE_USER_ASSERT( I + M * RowDilation <= sm.rows( ) * sm.rowdilation( ),
         "Invalid DilatedSubmatrix specification" );
      BLAZE_USER_ASSERT(
         J + N * ColumnDilation <= sm.columns( ) * sm.columndilation( ),
         "Invalid DilatedSubmatrix specification" );
   }

   return dilatedsubmatrix( sm.operand( ), sm.row( ) + I * sm.rowdilation( ),
      sm.column( ) + J * sm.columndilation( ), M * sm.rowdilation( ),
      N * sm.columndilation( ), RowDilation * sm.rowdilation( ),
      ColumnDilation * sm.columndilation( ), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of another constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix
// \param args The optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the other DilatedSubmatrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given constant
// DilatedSubmatrix.
*/
template< size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( const DilatedSubmatrix<MT,SO,DF>& sm, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RSAs...>, Unchecked > );

   if( isChecked ) {
      if( ( I + M * RowDilation > sm.rows( ) * sm.rowdilation( ) ) ||
         ( J + N * ColumnDilation > sm.columns( ) * sm.columndilation( ) ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid DilatedSubmatrix specification" );
      }
   }
   else {
      BLAZE_USER_ASSERT( I + M * RowDilation <= sm.rows( ) * sm.rowdilation( ) , "Invalid DilatedSubmatrix specification" );
      BLAZE_USER_ASSERT( J + N * ColumnDilation <= sm.columns( ) * sm.columndilation( ) , "Invalid DilatedSubmatrix specification" );
   }

   return dilatedsubmatrix( sm.operand( ), sm.row( ) + I * sm.rowdilation( ),
      sm.column( ) + J * sm.columndilation( ), M * sm.rowdilation( ),
      N * sm.columndilation( ), RowDilation * sm.rowdilation( ),
      ColumnDilation * sm.columndilation( ), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of another temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix
// \param args The optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the other DilatedSubmatrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given temporary
// DilatedSubmatrix.
*/
template< size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto) dilatedsubmatrix( DilatedSubmatrix<MT,SO,DF>&& sm, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RSAs...>, Unchecked > );

   if( isChecked ) {
      if( ( I + M * RowDilation > sm.rows( ) * sm.rowdilation( ) ) ||
         ( J + N * ColumnDilation > sm.columns( ) * sm.columndilation( ) ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid DilatedSubmatrix specification" );
      }
   }
   else {
      BLAZE_USER_ASSERT( I + M * RowDilation <= sm.rows( ) * sm.rowdilation( ) , "Invalid DilatedSubmatrix specification" );
      BLAZE_USER_ASSERT( J + N * ColumnDilation <= sm.columns( ) * sm.columndilation( ), "Invalid DilatedSubmatrix specification" );
   }

   return dilatedsubmatrix( sm.operand( ), sm.row( ) + I * sm.rowdilation( ),
      sm.column( ) + J * sm.columndilation( ), M * sm.rowdilation( ),
      N * sm.columndilation( ), RowDilation * sm.rowdilation( ),
      ColumnDilation * sm.columndilation( ), args...  );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of another DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The given DilatedSubmatrix
// \param row The index of the first row of the DilatedSubmatrix.
// \param column The index of the first column of the DilatedSubmatrix.
// \param m The number of rows of the DilatedSubmatrix.
// \param n The number of columns of the DilatedSubmatrix.
// \param args The optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the other DilatedSubmatrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto)
   dilatedsubmatrix( DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, size_t row, size_t column,
              size_t m, size_t n, size_t rowdilation, size_t columndilation, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RSAs...>, Unchecked > );

   if( isChecked ) {
      if( ( row + m * rowdilation > sm.rows( ) * sm.rowdilation( ) ) ||
         ( column + n * columndilation > sm.columns( ) * sm.columndilation( ) ) )
      {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid DilatedSubmatrix specification" );
      }
   }
   else {
      BLAZE_USER_ASSERT( row    + m * rowdilation <= sm.rows() * sm.rowdilation()   , "Invalid DilatedSubmatrix specification" );
      BLAZE_USER_ASSERT( column + n * columndilation <= sm.columns() * sm.columndilation(), "Invalid DilatedSubmatrix specification" );
   }

   return dilatedsubmatrix( sm.operand( ), sm.row( ) + row * sm.rowdilation( ), sm.column( ) + column * sm.columndilation( ), m, n,
      rowdilation * sm.rowdilation( ), columndilation * sm.columndilation( ), args... );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of another constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The given constant DilatedSubmatrix
// \param row The index of the first row of the DilatedSubmatrix.
// \param column The index of the first column of the DilatedSubmatrix.
// \param m The number of rows of the DilatedSubmatrix.
// \param n The number of columns of the DilatedSubmatrix.
// \param args The optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the other DilatedSubmatrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given constant
// DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto)
   dilatedsubmatrix( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, size_t row, size_t column,
              size_t m, size_t n, size_t rowdilation, size_t columndilation, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RSAs...>, Unchecked > );

   if( isChecked ) {
      if( ( row + m * rowdilation > sm.rows( ) * sm.rowdilation( ) ) ||
         ( column + n * columndilation > sm.columns( ) * sm.columndilation( ) ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid DilatedSubmatrix specification" );
      }
   }
   else {
      BLAZE_USER_ASSERT( row    + m * rowdilation <= sm.rows() * sm.rowdilation() , "Invalid DilatedSubmatrix specification" );
      BLAZE_USER_ASSERT( column + n * columndilation <= sm.columns() * sm.columndilation(), "Invalid DilatedSubmatrix specification" );
   }

   return dilatedsubmatrix( sm.operand(), sm.row( ) + row * sm.rowdilation( ), sm.column( ) + column * sm.columndilation( ), m, n,
      rowdilation * sm.rowdilation( ), columndilation * sm.columndilation( ), args...  );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific DilatedSubmatrix of another temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The given temporary DilatedSubmatrix
// \param row The index of the first row of the DilatedSubmatrix.
// \param column The index of the first column of the DilatedSubmatrix.
// \param m The number of rows of the DilatedSubmatrix.
// \param n The number of columns of the DilatedSubmatrix.
// \param args The optional DilatedSubmatrix arguments.
// \return View on the specified DilatedSubmatrix of the other DilatedSubmatrix.
// \exception std::invalid_argument Invalid DilatedSubmatrix specification.
//
// This function returns an expression representing the specified DilatedSubmatrix of the given temporary
// DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename... RSAs >  // Optional DilatedSubmatrix arguments
inline decltype(auto)
   dilatedsubmatrix( DilatedSubmatrix<MT,SO,DF,CSAs...>&& sm, size_t row, size_t column,
              size_t m, size_t n, size_t rowdilation, size_t columndilation, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RSAs...>, Unchecked > );

   if( isChecked ) {
      if( ( row + m * rowdilation > sm.rows( ) * sm.rowdilation( ) ) ||
         ( column + n * columndilation > sm.columns( ) * sm.columndilation( ) ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid DilatedSubmatrix specification" );
      }
   }
   else {
      BLAZE_USER_ASSERT( row    + m * rowdilation <= sm.rows() * sm.rowdilation()   , "Invalid DilatedSubmatrix specification" );
      BLAZE_USER_ASSERT( column + n * columndilation <= sm.columns() * sm.columndilation(), "Invalid DilatedSubmatrix specification" );
   }

   return dilatedsubmatrix( sm.operand(), sm.row( ) + row * sm.rowdilation( ), sm.column( ) + column * sm.columndilation( ), m, n,
      rowdilation * sm.rowdilation( ), columndilation * sm.columndilation( ), args... );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL RESTRUCTURING FUNCTIONS (DILATEDSUBVECTOR)
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific dilatedsubvector of the given matrix/vector multiplication.
// \ingroup DilatedSubmatrix
//
// \param vector The constant matrix/vector multiplication.
// \param args The runtime dilatedsubvector arguments.
// \return View on the specified dilatedsubvector of the multiplication.
//
// This function returns an expression representing the specified dilatedsubvector of the given
// matrix/vector multiplication.
*/
template< size_t... CSAs      // Compile time dilatedsubvector arguments
        , typename VT         // Vector base type of the expression
        , typename... RSAs >  // Runtime dilatedsubvector arguments
inline decltype(auto) dilatedsubvector( const MatVecMultExpr<VT>& vector, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   using MT = RemoveReference_t< LeftOperand_t< VectorType_t<VT> > >;

   const DilatedSubvectorData<CSAs...> sd( args... );

   BLAZE_DECLTYPE_AUTO( left , (~vector).leftOperand()  );
   BLAZE_DECLTYPE_AUTO( right, (~vector).rightOperand() );

   const size_t column( ( IsUpper_v<MT> )
                        ?( ( !IsStrictlyUpper_v<MT> )?( sd.offset() + 1UL ):( sd.offset() ) )
                        :( 0UL ) );
   const size_t n( ( IsLower_v<MT> )
                   ?( ( IsUpper_v<MT> )?( sd.size() )
                                       :( ( IsStrictlyLower_v<MT> && sd.size() > 0UL )
                                          ?( sd.offset() + sd.size() - 1UL )
                                          :( sd.offset() + sd.size() ) ) )
                   :( ( IsUpper_v<MT> )?( left.columns() - column )
                                       :( left.columns() ) ) );

   return dilatedsubmatrix( left, sd.offset(), column, sd.size(), n, sd.dilation(), 1UL ) * dilatedsubvector( right, column, n, sd.dilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific dilatedsubvector of the given vector/matrix multiplication.
// \ingroup DilatedSubmatrix
//
// \param vector The constant vector/matrix multiplication.
// \param args The runtime dilatedsubvector arguments.
// \return View on the specified dilatedsubvector of the multiplication.
//
// This function returns an expression representing the specified dilatedsubvector of the given
// vector/matrix multiplication.
*/
template< size_t... CSAs      // Compile time dilatedsubvector arguments
        , typename VT         // Vector base type of the expression
        , typename... RSAs >  // Runtime dilatedsubvector arguments
inline decltype(auto) dilatedsubvector( const TVecMatMultExpr<VT>& vector, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   using MT = RemoveReference_t< RightOperand_t< VectorType_t<VT> > >;

   const DilatedSubvectorData<CSAs...> sd( args... );

   BLAZE_DECLTYPE_AUTO( left , (~vector).leftOperand()  );
   BLAZE_DECLTYPE_AUTO( right, (~vector).rightOperand() );

   const size_t row( ( IsLower_v<MT> )
                     ?( ( !IsStrictlyLower_v<MT> )?( sd.offset() + 1UL ):( sd.offset() ) )
                     :( 0UL ) );
   const size_t m( ( IsUpper_v<MT> )
                   ?( ( IsLower_v<MT> )?( sd.size() )
                                       :( ( IsStrictlyUpper_v<MT> && sd.size() > 0UL )
                                          ?( sd.offset() + sd.size() - 1UL )
                                          :( sd.offset() + sd.size() ) ) )
                   :( ( IsLower_v<MT> )?( right.rows() - row )
                                       :( right.rows() ) ) );

   return dilatedsubvector( left, row, m, sd.dilation() ) * dilatedsubmatrix( right, row, sd.offset(), m, sd.size(), 1UL, sd.dilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific dilatedsubvector of the given column-wise matrix reduction operation.
// \ingroup DilatedSubmatrix
//
// \param vector The constant column-wise matrix reduction operation.
// \param args The runtime dilatedsubvector arguments.
// \return View on the specified dilatedsubvector of the reduction operation.
//
// This function returns an expression representing the specified dilatedsubvector of the given
// column-wise matrix reduction operation.
*/
template< size_t... CSAs      // Compile time dilatedsubvector arguments
        , typename VT         // Vector base type of the expression
        , typename... RSAs >  // Runtime dilatedsubvector arguments
inline decltype(auto) dilatedsubvector( const MatReduceExpr<VT,columnwise>& vector, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   const DilatedSubvectorData<CSAs...> sd( args... );
   const size_t M( (~vector).operand().rows() );

   decltype(auto) sm( dilatedsubmatrix( (~vector).operand(), 0UL, sd.offset(), M, sd.size(), 1UL, sd.dilation() ) );
   return reduce<columnwise>( sm, (~vector).operation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific dilatedsubvector of the given row-wise matrix reduction operation.
// \ingroup DilatedSubmatrix
//
// \param vector The constant row-wise matrix reduction operation.
// \param args The runtime dilatedsubvector arguments.
// \return View on the specified dilatedsubvector of the reduction operation.
//
// This function returns an expression representing the specified dilatedsubvector of the given row-wise
// matrix reduction operation.
*/
template< size_t... CSAs      // Compile time dilatedsubvector arguments
        , typename VT         // Vector base type of the expression
        , typename... RSAs >  // Runtime dilatedsubvector arguments
inline decltype(auto) dilatedsubvector( const MatReduceExpr<VT,rowwise>& vector, RSAs... args )
{
   BLAZE_FUNCTION_TRACE;

   const DilatedSubvectorData<CSAs...> sd( args... );
   const size_t N( (~vector).operand().columns() );

   decltype(auto) sm( dilatedsubmatrix( (~vector).operand(), sd.offset(), 0UL, sd.size(), N, sd.dilation(), 1UL ) );
   return reduce<rowwise>( sm, (~vector).operation() );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL RESTRUCTURING FUNCTIONS (ROW)
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the row.
// \param args The optional row arguments.
// \return View on the specified row of the DilatedSubmatrix.
//
// This function returns an expression representing the specified row of the given DilatedSubmatrix.
*/
template< size_t I1           // Row index
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) row( DilatedSubmatrix<MT,SO,DF,I2,J,M,N,RowDilation,ColumnDilation>& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   BLAZE_STATIC_ASSERT_MSG( I1 < M*RowDilation, "Invalid row access index" );

   return dilatedsubvector<J,N,ColumnDilation>( row<I1*RowDilation+I2>( sm.operand(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the row.
// \param args The optional row arguments.
// \return View on the specified row of the DilatedSubmatrix.
//
// This function returns an expression representing the specified row of the given constant
// DilatedSubmatrix.
*/
template< size_t I1           // Row index
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) row( const DilatedSubmatrix<MT,SO,DF,I2,J,M,N>& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   BLAZE_STATIC_ASSERT_MSG( I1 < M*RowDilation, "Invalid row access index" );

   return dilatedsubvector<J,N,ColumnDilation>( row<I1*RowDilation+I2>( sm.operand(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the row.
// \param args The optional row arguments.
// \return View on the specified row of the DilatedSubmatrix.
//
// This function returns an expression representing the specified row of the given temporary
// DilatedSubmatrix.
*/
template< size_t I1           // Row index
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) row( DilatedSubmatrix<MT,SO,DF,I2,J,M,N>&& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   BLAZE_STATIC_ASSERT_MSG( I1 < M *RowDilation, "Invalid row access index" );

   return dilatedsubvector<J,N,ColumnDilation>( row<I1*RowDilation+I2>( sm.operand(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the row.
// \param index The index of the row.
// \param args The optional row arguments.
// \return View on the specified row of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) row( DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>& sm, size_t index, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      if( ( index >= M * RowDilation ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid row access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( index < M * RowDilation, "Invalid row access index" );
   }

   return dilatedsubvector<J,N,ColumnDilation>( row( sm.operand(), I+index*RowDilation, args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the row.
// \param index The index of the row.
// \param args The optional row arguments.
// \return View on the specified row of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given constant
// DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) row( const DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>& sm, size_t index, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      if( ( index >= M * RowDilation) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid row access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( index < M * RowDilation, "Invalid row access index" );
   }

   return dilatedsubvector<J,N,ColumnDilation>( row( sm.operand(), I+index*RowDilation, args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the row.
// \param index The index of the row.
// \param args The optional row arguments.
// \return View on the specified row of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given temporary
// DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) row( DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>&& sm, size_t index, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      if( ( index >= M * RowDilation ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid row access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( index < M * RowDilation, "Invalid row access index" );
   }

   return dilatedsubvector<J,N,ColumnDilation>( row( sm.operand(), I+index*RowDilation, args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the row.
// \param args The optional row arguments.
// \return View on the specified row of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given DilatedSubmatrix.
*/
template< size_t... CRAs      // Compile time row arguments
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RRAs >  // Runtime row arguments
inline decltype(auto) row( DilatedSubmatrix<MT,SO,DF>& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   const RowData<CRAs...> rd( args... );

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      if( ( rd.row() >= sm.rows() * sm.rowdilation() ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid row access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( rd.row() < sm.rows() * sm.rowdilation(), "Invalid row access index" );
   }

   const size_t index( rd.row() * sm.rowdilation() + sm.row() );

   return dilatedsubvector( row( sm.operand(), index, args... ), sm.column(), sm.columns(), sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the row.
// \param args The optional row arguments.
// \return View on the specified row of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given constant
// DilatedSubmatrix.
*/
template< size_t... CRAs      // Compile time row arguments
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RRAs >  // Runtime row arguments
inline decltype(auto) row( const DilatedSubmatrix<MT,SO,DF>& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   const RowData<CRAs...> rd( args... );

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      if( ( rd.row() >= sm.rows() * sm.rowdilation() ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid row access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( rd.row() < sm.rows() * sm.rowdilation(), "Invalid row access index" );
   }

   const size_t index( rd.row() * sm.rowdilation() + sm.row() );

   return dilatedsubvector( row( sm.operand(), index, args... ), sm.column(), sm.columns(), sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the row.
// \param args The optional row arguments.
// \return View on the specified row of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given temporary
// DilatedSubmatrix.
*/
template< size_t... CRAs      // Compile time row arguments
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RRAs >  // Runtime row arguments
inline decltype(auto) row( DilatedSubmatrix<MT,SO,DF>&& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   const RowData<CRAs...> rd( args... );

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      if( ( rd.row() >= sm.rows() * sm.rowdilation() ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid row access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( rd.row() < sm.rows() * sm.rowdilation(), "Invalid row access index" );
   }

   const size_t index( rd.row() * sm.rowdilation() + sm.row() );

   return dilatedsubvector( row( sm.operand(), index, args... ), sm.column(), sm.columns(), sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL RESTRUCTURING FUNCTIONS (ROWS)
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the rows.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
//
// This function returns an expression representing the specified rows of the given DilatedSubmatrix.
*/
template< size_t I1           // First row index
        , size_t... Is        // Remaining row indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) rows( DilatedSubmatrix<MT,SO,DF,I2,J,M,N,RowDilation,ColumnDilation>& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<0UL,J,sizeof...(Is)+1UL,N,1UL,ColumnDilation>(
      rows( sm.operand(), make_dilated_index_subsequence<I2,M,RowDilation,I1,Is...>(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the rows.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
//
// This function returns an expression representing the specified rows of the given constant
// DilatedSubmatrix.
*/
template< size_t I1           // First row index
        , size_t... Is        // Remaining row indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) rows( const DilatedSubmatrix<MT,SO,DF,I2,J,M,N,RowDilation,ColumnDilation>& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<0UL,J,sizeof...(Is)+1UL,N,1UL,ColumnDilation>(
      rows( sm.operand(), make_dilated_index_subsequence<I2,M,RowDilation,I1,Is...>(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the rows.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
//
// This function returns an expression representing the specified rows of the given temporary
// DilatedSubmatrix.
*/
template< size_t I1           // First row index
        , size_t... Is        // Remaining row indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) rows( DilatedSubmatrix<MT,SO,DF,I2,J,M,N,RowDilation,ColumnDilation>&& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<0UL,J,sizeof...(Is)+1UL,N,1UL,ColumnDilation>(
      rows( sm.operand(), make_dilated_index_subsequence<I2,M,RowDilation,I1,Is...>(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the rows.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified rows of the given DilatedSubmatrix.
*/
template< size_t I1           // First row index
        , size_t... Is        // Remaining row indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) rows( DilatedSubmatrix<MT,SO,DF>& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      static constexpr size_t indices[] = { I1, Is... };
      for( size_t i=0UL; i<sizeof...(Is)+1UL; ++i ) {
         if( sm.rows() <= indices[i]*sm.rowdilation() ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid row access index" );
         }
      }
   }

   return dilatedsubmatrix( rows( sm.operand(), { I1*sm.rowdilation()+sm.row(), Is*sm.rowdilation()+sm.row()... }, args... ),
                     0UL, sm.column(), sizeof...(Is)+1UL, sm.columns(), 1UL, sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the rows.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified rows of the given constant
// DilatedSubmatrix.
*/
template< size_t I1           // First row index
        , size_t... Is        // Remaining row indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) rows( const DilatedSubmatrix<MT,SO,DF>& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      static constexpr size_t indices[] = { I1, Is... };
      for( size_t i=0UL; i<sizeof...(Is)+1UL; ++i ) {
         if( sm.rows() <= indices[i]*sm.rowdilation() ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid row access index" );
         }
      }
   }

   return dilatedsubmatrix( rows( sm.operand(), { I1*sm.rowdilation()+sm.row(), Is*sm.rowdilation()+sm.row()... }, args... ),
                     0UL, sm.column(), sizeof...(Is)+1UL, sm.columns(), 1UL, sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the rows.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified rows of the given temporary
// DilatedSubmatrix.
*/
template< size_t I1           // First row index
        , size_t... Is        // Remaining row indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RRAs >  // Optional row arguments
inline decltype(auto) rows( DilatedSubmatrix<MT,SO,DF>&& sm, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      static constexpr size_t indices[] = { I1, Is... };
      for( size_t i=0UL; i<sizeof...(Is)+1UL; ++i ) {
         if( sm.rows() <= indices[i]*sm.rowdilation() ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid row access index" );
         }
      }
   }

   return dilatedsubmatrix( rows( sm.operand(), { I1*sm.rowdilation()+sm.row(), Is*sm.rowdilation()+sm.row()... }, args... ),
                     0UL, sm.column(), sizeof...(Is)+1UL, sm.columns(), 1UL, sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the rows.
// \param indices Pointer to the first index of the selected rows.
// \param n The total number of indices.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified rows of the given DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename T          // Type of the row indices
        , typename... RRAs >  // Optional row arguments
inline decltype(auto)
   rows( DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, const T* indices, size_t n, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t i=0UL; i<n; ++i ) {
         if( sm.rows() <= size_t( indices[i]*sm.rowdilation() ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid row specification" );
         }
      }
   }

   SmallArray<size_t,128UL> newIndices( indices, indices+n );
   std::for_each( newIndices.begin(), newIndices.end(),
                  [row=sm.row(),rowdilation =sm.rowdilation()]( size_t& index ){ index = index * rowdilation + row; } );

   return dilatedsubmatrix( rows( sm.operand(), newIndices.data(), n, args... ),
                     0UL, sm.column(), n, sm.columns(), 1UL, sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the rows.
// \param indices Pointer to the first index of the selected rows.
// \param n The total number of indices.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified rows of the given constant
// DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename T          // Type of the row indices
        , typename... RRAs >  // Optional row arguments
inline decltype(auto)
   rows( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, const T* indices, size_t n, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t i=0UL; i<n; ++i ) {
         if( sm.rows() <= size_t( indices[i] ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid row specification" );
         }
      }
   }

   SmallArray<size_t,128UL> newIndices( indices, indices+n );
   std::for_each( newIndices.begin(), newIndices.end(),
                  [row=sm.row(),rowdilation=sm.rowdilation()]( size_t& index ){ index = index * rowdilation + row; } );

   return dilatedsubmatrix( rows( sm.operand(), newIndices.data(), n, args... ),
                     0UL, sm.column(), n, sm.columns(), 1UL, sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the rows.
// \param indices Pointer to the first index of the selected rows.
// \param n The total number of indices.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified rows of the given temporary
// DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename T          // Type of the row indices
        , typename... RRAs >  // Optional row arguments
inline decltype(auto)
   rows( DilatedSubmatrix<MT,SO,DF,CSAs...>&& sm, const T* indices, size_t n, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t i=0UL; i<n; ++i ) {
         if( sm.rows() <= size_t( indices[i] ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid row specification" );
         }
      }
   }

   SmallArray<size_t,128UL> newIndices( indices, indices+n );
   std::for_each( newIndices.begin(), newIndices.end(),
                  [row=sm.row(),rowdilation=sm.rowdilation()]( size_t& index ){ index = index * rowdilation + row; } );

   return dilatedsubmatrix( rows( sm.operand(), newIndices.data(), n, args... ),
                     0UL, sm.column(), n, sm.columns(), 1UL, sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the rows.
// \param p Callable producing the indices.
// \param n The total number of indices.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified rows of the given DilatedSubmatrix.
*/
template< typename MT       // Type of the sparse DilatedSubmatrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename P        // Type of the index producer
        , typename... RRAs  // Optional row arguments
        , EnableIf_t< !IsPointer_v<P> >* = nullptr >
inline decltype(auto)
   rows( DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, P p, size_t n, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t i=0UL; i<n; ++i ) {
         if( sm.rows() <= size_t( p(i) ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid row specification" );
         }
      }
   }

   return dilatedsubmatrix( rows( sm.operand(), [p,offset=sm.row(),rowdilation=sm.rowdilation()]( size_t i )
                     { return p(i)*rowdilation+offset; }, n, args... ),
                     0UL, sm.column(), n, sm.columns(), 1UL, sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the rows.
// \param p Callable producing the indices.
// \param n The total number of indices.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified rows of the given constant
// DilatedSubmatrix.
*/
template< typename MT       // Type of the sparse DilatedSubmatrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename P        // Type of the index producer
        , typename... RRAs  // Optional row arguments
        , EnableIf_t< !IsPointer_v<P> >* = nullptr >
inline decltype(auto)
   rows( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, P p, size_t n, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t i=0UL; i<n; ++i ) {
         if( sm.rows() <= size_t( p(i) ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid row specification" );
         }
      }
   }

   return dilatedsubmatrix( rows( sm.operand(), [p,offset=sm.row(),rowdilation=sm.rowdilation()]( size_t i )
                     { return p(i)*rowdilation+offset; }, n, args... ),
                     0UL, sm.column(), n, sm.columns(), 1UL, sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific rows of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the rows.
// \param p Callable producing the indices.
// \param n The total number of indices.
// \param args The optional row arguments.
// \return View on the specified rows of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified rows of the given temporary
// DilatedSubmatrix.
*/
template< typename MT       // Type of the sparse DilatedSubmatrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename P        // Type of the index producer
        , typename... RRAs  // Optional row arguments
        , EnableIf_t< !IsPointer_v<P> >* = nullptr >
inline decltype(auto)
   rows( DilatedSubmatrix<MT,SO,DF,CSAs...>&& sm, P p, size_t n, RRAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RRAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t i=0UL; i<n; ++i ) {
         if( sm.rows() <= size_t( p(i) ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid row specification" );
         }
      }
   }

   return dilatedsubmatrix( rows( sm.operand(), [p,offset=sm.row(),rowdilation=sm.rowdilation()]( size_t i )
                     { return p(i)*rowdilation+offset; }, n, args... ),
                     0UL, sm.column(), n, sm.columns(), 1UL, sm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL RESTRUCTURING FUNCTIONS (COLUMN)
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific column of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the column.
// \param args The optional column arguments.
// \return View on the specified column of the DilatedSubmatrix.
//
// This function returns an expression representing the specified column of the given DilatedSubmatrix.
*/
template< size_t I1           // Column index
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) column( DilatedSubmatrix<MT,SO,DF,I2,J,M,N,RowDilation,ColumnDilation>& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   BLAZE_STATIC_ASSERT_MSG( I1 < N*ColumnDilation, "Invalid column access index" );

   return dilatedsubvector<I2,M,RowDilation>( column<I1*ColumnDilation+J>( sm.operand(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific column of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the column.
// \param args The optional column arguments.
// \return View on the specified column of the DilatedSubmatrix.
//
// This function returns an expression representing the specified column of the given constant
// DilatedSubmatrix.
*/
template< size_t I1           // Column index
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) column( const DilatedSubmatrix<MT,SO,DF,I2,J,M,N,RowDilation,ColumnDilation>& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   BLAZE_STATIC_ASSERT_MSG( I1 < N*ColumnDilation, "Invalid column access index" );

   return dilatedsubvector<I2,M,RowDilation>( column<I1*ColumnDilation+J>( sm.operand(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific column of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the column.
// \param args The optional column arguments.
// \return View on the specified column of the DilatedSubmatrix.
//
// This function returns an expression representing the specified column of the given temporary
// DilatedSubmatrix.
*/
template< size_t I1           // Column index
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) column( DilatedSubmatrix<MT,SO,DF,I2,J,M,N,RowDilation,ColumnDilation>&& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   BLAZE_STATIC_ASSERT_MSG( I1 < N*ColumnDilation, "Invalid column access index" );

   return dilatedsubvector<I2,M,RowDilation>( column<I1*ColumnDilation+J>( sm.operand(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific column of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the column.
// \param index The index of the column.
// \param args The optional column arguments.
// \return View on the specified column of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified column of the given DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) column( DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>& sm, size_t index, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      if( ( index >= N*ColumnDilation ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid column access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( index < N*ColumnDilation, "Invalid column access index" );
   }

   return dilatedsubvector<I,M,RowDilation>( column( sm.operand(), J+index*ColumnDilation, args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific column of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the column.
// \param index The index of the column.
// \param args The optional column arguments.
// \return View on the specified column of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified column of the given constant
// DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) column( const DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>& sm, size_t index, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      if( ( index >= N*ColumnDilation ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid column access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( index < N*ColumnDilation, "Invalid column access index" );
   }

   return dilatedsubvector<I,M,RowDilation>( column( sm.operand(), J+index*ColumnDilation, args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific column of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the column.
// \param index The index of the column.
// \param args The optional column arguments.
// \return View on the specified column of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified column of the given temporary
// DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I            // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) column( DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>&& sm, size_t index, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      if( ( index >= N * ColumnDilation ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid column access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( index < N * ColumnDilation, "Invalid column access index" );
   }

   return dilatedsubvector<I,M,RowDilation>( column( sm.operand(), J+index*ColumnDilation, args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific column of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the column.
// \param args The optional column arguments.
// \return View on the specified column of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified column of the given DilatedSubmatrix.
*/
template< size_t... CCAs      // Compile time column arguments
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RCAs >  // Runtime column arguments
inline decltype(auto) column( DilatedSubmatrix<MT,SO,DF>& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   const ColumnData<CCAs...> cd( args... );

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      if( ( cd.column() >= sm.columns() * sm.columndilation() ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid column access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( cd.column() < sm.columns() * sm.columndilation(), "Invalid column access index" );
   }

   const size_t index( cd.column() * sm.columndilation() + sm.column() );

   return dilatedsubvector( column( sm.operand(), index, args... ), sm.row(), sm.rows(), sm.rowdilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific column of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the column.
// \param args The optional column arguments.
// \return View on the specified column of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified column of the given constant
// DilatedSubmatrix.
*/
template< size_t... CCAs      // Compile time column arguments
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RCAs >  // Runtime column arguments
inline decltype(auto) column( const DilatedSubmatrix<MT,SO,DF>& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   const ColumnData<CCAs...> cd( args... );

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      if( ( cd.column() >= sm.columns() * sm.columndilation() ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid column access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( cd.column() < sm.columns() * sm.columndilation(), "Invalid column access index" );
   }

   const size_t index( cd.column() * sm.columndilation() + sm.column() );

   return dilatedsubvector( column( sm.operand(), index, args... ), sm.row(), sm.rows(), sm.rowdilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific column of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the column.
// \param args The optional column arguments.
// \return View on the specified column of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified column of the given temporary
// DilatedSubmatrix.
*/
template< size_t... CCAs      // Compile time column arguments
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RCAs >  // Runtime column arguments
inline decltype(auto) column( DilatedSubmatrix<MT,SO,DF>&& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   const ColumnData<CCAs...> cd( args... );

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      if( ( cd.column() >= sm.columns() * sm.columndilation() ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid column access index" );
      }
   }
   else {
      BLAZE_USER_ASSERT( cd.column() < sm.columns() * sm.columndilation(), "Invalid column access index" );
   }

   const size_t index( cd.column() * sm.columndilation() + sm.column() );

   return dilatedsubvector( column( sm.operand(), index, args... ), sm.row(), sm.rows(), sm.rowdilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL RESTRUCTURING FUNCTIONS (COLUMNS)
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the columns.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
//
// This function returns an expression representing the specified columns of the given DilatedSubmatrix.
*/
template< size_t I1           // First column index
        , size_t... Is        // Remaining column indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) columns( DilatedSubmatrix<MT,SO,DF,I2,J,M,N,RowDilation,ColumnDilation>& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<I2,0UL,M,sizeof...(Is)+1UL,RowDilation,1UL>(
      columns( sm.operand(), make_dilated_index_subsequence<J,N,ColumnDilation,I1,Is...>(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the columns.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
//
// This function returns an expression representing the specified columns of the given constant
// DilatedSubmatrix.
*/
template< size_t I1           // First column index
        , size_t... Is        // Remaining column indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) columns( const DilatedSubmatrix<MT,SO,DF,I2,J,M,N,RowDilation,ColumnDilation>& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<I2,0UL,M,sizeof...(Is)+1UL,RowDilation>(
      columns( sm.operand(), make_dilated_index_subsequence<J,N,ColumnDilation,I1,Is...>(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the columns.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
//
// This function returns an expression representing the specified columns of the given temporary
// DilatedSubmatrix.
*/
template< size_t I1           // First column index
        , size_t... Is        // Remaining column indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t I2           // Index of the first row
        , size_t J            // Index of the first column
        , size_t M            // Number of rows
        , size_t N            // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation  // Step between columns of the DilatedSubmatrix
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) columns( DilatedSubmatrix<MT,SO,DF,I2,J,M,N,RowDilation,ColumnDilation>&& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   return dilatedsubmatrix<I2,0UL,M,sizeof...(Is)+1UL,RowDilation>(
      columns( sm.operand(), make_dilated_index_subsequence<J,N,ColumnDilation,I1,Is...>(), args... ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the columns.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified columns of the given DilatedSubmatrix.
*/
template< size_t I1           // First column index
        , size_t... Is        // Remaining column indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) columns( DilatedSubmatrix<MT,SO,DF>& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      static constexpr size_t indices[] = { I1, Is... };
      for( size_t j=0UL; j<sizeof...(Is)+1UL; ++j ) {
         if( sm.columns() <= indices[j]*sm.columndilation() ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid column access index" );
         }
      }
   }

   return dilatedsubmatrix( columns( sm.operand(), { I1*sm.columndilation()+sm.column(), Is*sm.columndilation()+sm.column()... }, args... ),
                     sm.row(), 0UL, sm.rows(), sizeof...(Is)+1UL, sm.columndilation(), 1UL, unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the columns.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified columns of the given constant
// DilatedSubmatrix.
*/
template< size_t I1           // First column index
        , size_t... Is        // Remaining column indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) columns( const DilatedSubmatrix<MT,SO,DF>& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      static constexpr size_t indices[] = { I1, Is... };
      for( size_t j=0UL; j<sizeof...(Is)+1UL; ++j ) {
         if( sm.columns() <= indices[j]*sm.columndilation() ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid column access index" );
         }
      }
   }

   return dilatedsubmatrix( columns( sm.operand(), { I1*sm.columndilation()+sm.column(), Is*sm.columndilation()+sm.column()... }, args... ),
                     sm.row(), 0UL, sm.rows(), sizeof...(Is)+1UL, sm.rowdilation(), 1UL, unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the columns.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified columns of the given temporary
// DilatedSubmatrix.
*/
template< size_t I1           // First column index
        , size_t... Is        // Remaining column indices
        , typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , typename... RCAs >  // Optional column arguments
inline decltype(auto) columns( DilatedSubmatrix<MT,SO,DF>&& sm, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      static constexpr size_t indices[] = { I1, Is... };
      for( size_t j=0UL; j<sizeof...(Is)+1UL; ++j ) {
         if( sm.columns() <= indices[j] *sm.columndilation()) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid column access index" );
         }
      }
   }

   return dilatedsubmatrix( columns( sm.operand(), { I1*sm.columndilation()+sm.column(), Is*sm.columndilation()+sm.column()... }, args... ),
                     sm.row(), 0UL, sm.rows(), sizeof...(Is)+1UL, sm.rowdilation(), 1UL, unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the columns.
// \param indices Pointer to the first index of the selected columns.
// \param n The total number of indices.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified columns of the given DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename T          // Type of the column indices
        , typename... RCAs >  // Optional column arguments
inline decltype(auto)
   columns( DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, const T* indices, size_t n, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t j=0UL; j<n; ++j ) {
         if( sm.columns() <= size_t( indices[j]*sm.columndilation() ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid column specification" );
         }
      }
   }

   SmallArray<size_t,128UL> newIndices( indices, indices+n );
   std::for_each( newIndices.begin(), newIndices.end(),
                  [column=sm.column(),columndilation=sm.columndilation()]( size_t& index ){ index = index*columndilation + column; } );

   return dilatedsubmatrix( columns( sm.operand(), newIndices.data(), n, args... ),
                     sm.row(), 0UL, sm.rows(), n, sm.rowdilation(), 1UL, unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the columns.
// \param indices Pointer to the first index of the selected columns.
// \param n The total number of indices.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified columns of the given constant
// DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename T          // Type of the column indices
        , typename... RCAs >  // Optional column arguments
inline decltype(auto)
   columns( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, const T* indices, size_t n, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t j=0UL; j<n; ++j ) {
         if( sm.columns() <= size_t( indices[j] ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid column specification" );
         }
      }
   }

   SmallArray<size_t,128UL> newIndices( indices, indices+n );
   std::for_each( newIndices.begin(), newIndices.end(),
                  [column=sm.column(),columndilation=sm.columndilation()]( size_t& index ){ index = index*columndilation + column; } );

   return dilatedsubmatrix( columns( sm.operand(), newIndices.data(), n, args... ),
                     sm.row(), 0UL, sm.rows(), n, sm.rowdilation(), 1UL, unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the columns.
// \param indices Pointer to the first index of the selected columns.
// \param n The total number of indices.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified columns of the given temporary
// DilatedSubmatrix.
*/
template< typename MT         // Type of the sparse DilatedSubmatrix
        , bool SO             // Storage order
        , bool DF             // Density flag
        , size_t... CSAs      // Compile time DilatedSubmatrix arguments
        , typename T          // Type of the column indices
        , typename... RCAs >  // Optional column arguments
inline decltype(auto)
   columns( DilatedSubmatrix<MT,SO,DF,CSAs...>&& sm, const T* indices, size_t n, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t j=0UL; j<n; ++j ) {
         if( sm.columns() <= size_t( indices[j] ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid column specification" );
         }
      }
   }

   SmallArray<size_t,128UL> newIndices( indices, indices+n );
   std::for_each( newIndices.begin(), newIndices.end(),
                  [column=sm.column(),columndilation=sm.columndilation()]( size_t& index ){ index = index*columndilation + column; } );

   return dilatedsubmatrix( columns( sm.operand(), newIndices.data(), n, args... ),
                     sm.row(), 0UL, sm.rows(), n, sm.rowdilation(), 1UL, unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix containing the columns.
// \param p Callable producing the indices.
// \param n The total number of indices.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified columns of the given DilatedSubmatrix.
*/
template< typename MT       // Type of the sparse DilatedSubmatrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename P        // Type of the index producer
        , typename... RCAs  // Optional column arguments
        , EnableIf_t< !IsPointer_v<P> >* = nullptr >
inline decltype(auto)
   columns( DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, P p, size_t n, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t j=0UL; j<n; ++j ) {
         if( sm.columns() <= size_t( p(j) ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid column specification" );
         }
      }
   }

   return dilatedsubmatrix( columns( sm.operand(), [p,offset=sm.column(),columndilation=sm.columndilation()]( size_t i )
                     { return p(i)*columndilation+offset; }, n, args... ),
                     sm.row(), 0UL, sm.rows(), n, sm.rowdilation(), 1UL, unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given constant DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The constant DilatedSubmatrix containing the columns.
// \param p Callable producing the indices.
// \param n The total number of indices.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified columns of the given constant
// DilatedSubmatrix.
*/
template< typename MT       // Type of the sparse DilatedSubmatrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename P        // Type of the index producer
        , typename... RCAs  // Optional column arguments
        , EnableIf_t< !IsPointer_v<P> >* = nullptr >
inline decltype(auto)
   columns( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, P p, size_t n, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t j=0UL; j<n; ++j ) {
         if( sm.columns() <= size_t( p(j) ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid column specification" );
         }
      }
   }

   return dilatedsubmatrix( columns( sm.operand(), [p,offset=sm.column(),columndilation=sm.columndilation()]( size_t i )
                     { return p(i)*columndilation+offset; }, n, args... ),
                     sm.row(), 0UL, sm.rows(), n, sm.rowdilation(), 1UL, unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on specific columns of the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix containing the columns.
// \param p Callable producing the indices.
// \param n The total number of indices.
// \param args The optional column arguments.
// \return View on the specified columns of the DilatedSubmatrix.
// \exception std::invalid_argument Invalid column access index.
//
// This function returns an expression representing the specified columns of the given temporary
// DilatedSubmatrix.
*/
template< typename MT       // Type of the sparse DilatedSubmatrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename P        // Type of the index producer
        , typename... RCAs  // Optional column arguments
        , EnableIf_t< !IsPointer_v<P> >* = nullptr >
inline decltype(auto)
   columns( DilatedSubmatrix<MT,SO,DF,CSAs...>&& sm, P p, size_t n, RCAs... args )
{
   BLAZE_FUNCTION_TRACE;

   constexpr bool isChecked( !Contains_v< TypeList<RCAs...>, Unchecked > );

   if( isChecked ) {
      for( size_t j=0UL; j<n; ++j ) {
         if( sm.columns() <= size_t( p(j) ) ) {
            BLAZE_THROW_INVALID_ARGUMENT( "Invalid column specification" );
         }
      }
   }

   return dilatedsubmatrix( columns( sm.operand(), [p,offset=sm.column(),columndilation=sm.columndilation()]( size_t i )
                     { return p(i)*columndilation+offset; }, n, args... ),
                     sm.row(), 0UL, sm.rows(), n, sm.rowdilation(), 1UL, unchecked );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  DILATEDSUBMATRIX OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Resetting the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be resetted.
// \return void
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline void reset( DilatedSubmatrix<MT,SO,DF,CSAs...>& sm )
{
   sm.reset();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Resetting the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary DilatedSubmatrix to be resetted.
// \return void
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline void reset( DilatedSubmatrix<MT,SO,DF,CSAs...>&& sm )
{
   sm.reset();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Reset the specified row/column of the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be resetted.
// \param i The index of the row/column to be resetted.
// \return void
//
// This function resets the values in the specified row/column of the given DilatedSubmatrix to their
// default value. In case the given DilatedSubmatrix is a \a rowMajor matrix the function resets the
// values in row \a i, if it is a \a columnMajor matrix the function resets the values in column
// \a i. Note that the capacity of the row/column remains unchanged.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline void reset( DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, size_t i )
{
   sm.reset( i );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Clearing the given matrix.
// \ingroup DilatedSubmatrix
//
// \param sm The matrix to be cleared.
// \return void
//
// Clearing a DilatedSubmatrix is equivalent to resetting it via the reset() function.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline void clear( DilatedSubmatrix<MT,SO,DF,CSAs...>& sm )
{
   sm.reset();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Clearing the given temporary matrix.
// \ingroup DilatedSubmatrix
//
// \param sm The temporary matrix to be cleared.
// \return void
//
// Clearing a DilatedSubmatrix is equivalent to resetting it via the reset() function.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline void clear( DilatedSubmatrix<MT,SO,DF,CSAs...>&& sm )
{
   sm.reset();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the given dense DilatedSubmatrix is in default state.
// \ingroup DilatedSubmatrix
//
// \param sm The dense DilatedSubmatrix to be tested for its default state.
// \return \a true in case the given dense DilatedSubmatrix is component-wise zero, \a false otherwise.
//
// This function checks whether the dense DilatedSubmatrix is in default state. For instance, in case
// the DilatedSubmatrix is instantiated for a built-in integral or floating point data type, the function
// returns \a true in case all DilatedSubmatrix elements are 0 and \a false in case any DilatedSubmatrix element
// is not 0. The following example demonstrates the use of the \a isDefault function:

   \code
   blaze::DynamicMatrix<double,rowMajor> A;
   // ... Resizing and initialization
   if( isDefault( DilatedSubmatrix( A, 12UL, 13UL, 22UL, 33UL ) ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isDefault<relaxed>( DilatedSubmatrix( A, 12UL, 13UL, 22UL, 33UL ) ) ) { ... }
   \endcode
*/
template< bool RF           // Relaxation flag
        , typename MT       // Type of the dense matrix
        , bool SO           // Storage order
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isDefault( const DilatedSubmatrix<MT,SO,true,CSAs...>& sm )
{
   using blaze::isDefault;

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<(~sm).rows(); ++i )
         for( size_t j=0UL; j<(~sm).columns(); ++j )
            if( !isDefault<RF>( (~sm)(i,j) ) )
               return false;
   }
   else {
      for( size_t j=0UL; j<(~sm).columns(); ++j )
         for( size_t i=0UL; i<(~sm).rows(); ++i )
            if( !isDefault<RF>( (~sm)(i,j) ) )
               return false;
   }

   return true;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the given sparse DilatedSubmatrix is in default state.
// \ingroup DilatedSubmatrix
//
// \param sm The sparse DilatedSubmatrix to be tested for its default state.
// \return \a true in case the given sparse DilatedSubmatrix is component-wise zero, \a false otherwise.
//
// This function checks whether the sparse DilatedSubmatrix is in default state. For instance, in case
// the DilatedSubmatrix is instantiated for a built-in integral or floating point data type, the function
// returns \a true in case all DilatedSubmatrix elements are 0 and \a false in case any DilatedSubmatrix element
// is not 0. The following example demonstrates the use of the \a isDefault function:

   \code
   blaze::CompressedMatrix<double,rowMajor> A;
   // ... Resizing and initialization
   if( isDefault( DilatedSubmatrix( A, 12UL, 13UL, 22UL, 33UL ) ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isDefault<relaxed>( DilatedSubmatrix( A, 12UL, 13UL, 22UL, 33UL ) ) ) { ... }
   \endcode
*/
template< bool RF           // Relaxation flag
        , typename MT       // Type of the sparse matrix
        , bool SO           // Storage order
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isDefault( const DilatedSubmatrix<MT,SO,false,CSAs...>& sm )
{
   using blaze::isDefault;

   const size_t iend( ( SO == rowMajor)?( sm.rows() ):( sm.columns() ) );

   for( size_t i=0UL; i<iend; ++i ) {
      for( auto element=sm.cbegin(i); element!=sm.cend(i); ++element )
         if( !isDefault<RF>( element->value() ) ) return false;
   }

   return true;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the invariants of the given DilatedSubmatrix are intact.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be tested.
// \return \a true in case the given DilatedSubmatrix's invariants are intact, \a false otherwise.
//
// This function checks whether the invariants of the DilatedSubmatrix are intact, i.e. if its
// state is valid. In case the invariants are intact, the function returns \a true, else it
// will return \a false. The following example demonstrates the use of the \a isIntact()
// function:

   \code
   blaze::DynamicMatrix<double,rowMajor> A;
   // ... Resizing and initialization
   if( isIntact( DilatedSubmatrix( A, 12UL, 13UL, 22UL, 33UL ) ) ) { ... }
   \endcode
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isIntact( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm ) noexcept
{
   return ( sm.row() + sm.rows()*sm.rowdilation() <= sm.operand().rows() &&
            sm.column() + sm.columns()*sm.columndilation() <= sm.operand().columns() &&
            isIntact( sm.operand() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given DilatedSubmatrix is symmetric.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be checked.
// \return \a true if the DilatedSubmatrix is symmetric, \a false if not.
//
// This function checks if the given DilatedSubmatrix is symmetric. The DilatedSubmatrix is considered to
// be symmetric if it is a square matrix whose transpose is equal to itself (\f$ A = A^T \f$). The
// following code example demonstrates the use of the function:

   \code
   blaze::DynamicMatrix<int,blaze::rowMajor> A( 32UL, 16UL );
   // ... Initialization

   auto sm = DilatedSubmatrix( A, 8UL, 8UL, 16UL, 16UL );

   if( isSymmetric( sm ) ) { ... }
   \endcode
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isSymmetric( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm )
{
   using BaseType = BaseType_t< DilatedSubmatrix<MT,SO,DF,CSAs...> >;

   if( IsSymmetric_v< MT > && sm.row( ) == sm.column( ) &&
      sm.rows( ) == sm.columns( ) && sm.rowdilation( ) == sm.columndilation( ) )
      return true;
   else return isSymmetric( static_cast<const BaseType&>( sm ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given DilatedSubmatrix is Hermitian.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be checked.
// \return \a true if the DilatedSubmatrix is Hermitian, \a false if not.
//
// This function checks if the given DilatedSubmatrix is Hermitian. The DilatedSubmatrix is considered to
// be Hermitian if it is a square matrix whose transpose is equal to its conjugate transpose
// (\f$ A = \overline{A^T} \f$). The following code example demonstrates the use of the function:

   \code
   blaze::DynamicMatrix<int,blaze::rowMajor> A( 32UL, 16UL );
   // ... Initialization

   auto sm = DilatedSubmatrix( A, 8UL, 8UL, 16UL, 16UL );

   if( isHermitian( sm ) ) { ... }
   \endcode
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isHermitian( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm )
{
   using BaseType = BaseType_t< DilatedSubmatrix<MT,SO,DF,CSAs...> >;

   if( IsHermitian_v< MT > && sm.row( ) == sm.column( ) &&
      sm.rows( ) == sm.columns( ) && sm.rowdilation( ) == sm.columndilation( ) )
      return true;
   else return isHermitian( static_cast<const BaseType&>( sm ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given DilatedSubmatrix is a lower triangular matrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be checked.
// \return \a true if the DilatedSubmatrix is a lower triangular matrix, \a false if not.
//
// This function checks if the given DilatedSubmatrix is a lower triangular matrix. The matrix is
// considered to be lower triangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        l_{0,0} & 0       & 0       & \cdots & 0       \\
                        l_{1,0} & l_{1,1} & 0       & \cdots & 0       \\
                        l_{2,0} & l_{2,1} & l_{2,2} & \cdots & 0       \\
                        \vdots  & \vdots  & \vdots  & \ddots & \vdots  \\
                        l_{N,0} & l_{N,1} & l_{N,2} & \cdots & l_{N,N} \\
                        \end{array}\right).\f]

// \f$ 0 \times 0 \f$ or \f$ 1 \times 1 \f$ matrices are considered as trivially lower triangular.
// The following code example demonstrates the use of the function:

   \code
   blaze::DynamicMatrix<int,blaze::rowMajor> A( 32UL, 16UL );
   // ... Initialization

   auto sm = DilatedSubmatrix( A, 8UL, 8UL, 16UL, 16UL );

   if( isLower( sm ) ) { ... }
   \endcode
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isLower( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm )
{
   using BaseType = BaseType_t< DilatedSubmatrix<MT,SO,DF,CSAs...> >;

   if( IsLower_v< MT > && sm.row( ) == sm.column( ) &&
      sm.rows( ) == sm.columns( ) && sm.rowdilation( ) == sm.columndilation( ) )
      return true;
   else return isLower( static_cast<const BaseType&>( sm ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given DilatedSubmatrix is a lower unitriangular matrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be checked.
// \return \a true if the DilatedSubmatrix is a lower unitriangular matrix, \a false if not.
//
// This function checks if the given DilatedSubmatrix is a lower unitriangular matrix. The matrix is
// considered to be lower triangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        1       & 0       & 0       & \cdots & 0      \\
                        l_{1,0} & 1       & 0       & \cdots & 0      \\
                        l_{2,0} & l_{2,1} & 1       & \cdots & 0      \\
                        \vdots  & \vdots  & \vdots  & \ddots & \vdots \\
                        l_{N,0} & l_{N,1} & l_{N,2} & \cdots & 1      \\
                        \end{array}\right).\f]

// The following code example demonstrates the use of the function:

   \code
   blaze::DynamicMatrix<int,blaze::rowMajor> A( 32UL, 16UL );
   // ... Initialization

   auto sm = DilatedSubmatrix( A, 8UL, 8UL, 16UL, 16UL );

   if( isUniLower( sm ) ) { ... }
   \endcode
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isUniLower( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm )
{
   using BaseType = BaseType_t< DilatedSubmatrix<MT,SO,DF,CSAs...> >;

   if( IsUniLower_v< MT > && sm.row( ) == sm.column( ) &&
      sm.rows( ) == sm.columns( ) && sm.rowdilation( ) == sm.columndilation( ) )
      return true;
   else return isUniLower( static_cast<const BaseType&>( sm ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given DilatedSubmatrix is a strictly lower triangular matrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be checked.
// \return \a true if the DilatedSubmatrix is a strictly lower triangular matrix, \a false if not.
//
// This function checks if the given DilatedSubmatrix is a strictly lower triangular matrix. The
// matrix is considered to be lower triangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        0       & 0       & 0       & \cdots & 0      \\
                        l_{1,0} & 0       & 0       & \cdots & 0      \\
                        l_{2,0} & l_{2,1} & 0       & \cdots & 0      \\
                        \vdots  & \vdots  & \vdots  & \ddots & \vdots \\
                        l_{N,0} & l_{N,1} & l_{N,2} & \cdots & 0      \\
                        \end{array}\right).\f]

// The following code example demonstrates the use of the function:

   \code
   blaze::DynamicMatrix<int,blaze::rowMajor> A( 32UL, 16UL );
   // ... Initialization

   auto sm = DilatedSubmatrix( A, 8UL, 8UL, 16UL, 16UL );

   if( isStrictlyLower( sm ) ) { ... }
   \endcode
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isStrictlyLower( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm )
{
   using BaseType = BaseType_t< DilatedSubmatrix<MT,SO,DF,CSAs...> >;

   if( IsStrictlyLower_v< MT > && sm.row( ) == sm.column( ) &&
      sm.rows( ) == sm.columns( ) && sm.rowdilation( ) == sm.columndilation( ) )
      return true;
   else return isStrictlyLower( static_cast<const BaseType&>( sm ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given DilatedSubmatrix is an upper triangular matrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be checked.
// \return \a true if the DilatedSubmatrix is an upper triangular matrix, \a false if not.
//
// This function checks if the given sparse DilatedSubmatrix is an upper triangular matrix. The matrix
// is considered to be upper triangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        u_{0,0} & u_{0,1} & u_{0,2} & \cdots & u_{0,N} \\
                        0       & u_{1,1} & u_{1,2} & \cdots & u_{1,N} \\
                        0       & 0       & u_{2,2} & \cdots & u_{2,N} \\
                        \vdots  & \vdots  & \vdots  & \ddots & \vdots  \\
                        0       & 0       & 0       & \cdots & u_{N,N} \\
                        \end{array}\right).\f]

// \f$ 0 \times 0 \f$ or \f$ 1 \times 1 \f$ matrices are considered as trivially upper triangular.
// The following code example demonstrates the use of the function:

   \code
   blaze::DynamicMatrix<int,blaze::rowMajor> A( 32UL, 16UL );
   // ... Initialization

   auto sm = DilatedSubmatrix( A, 8UL, 8UL, 16UL, 16UL );

   if( isUpper( sm ) ) { ... }
   \endcode
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isUpper( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm )
{
   using BaseType = BaseType_t< DilatedSubmatrix<MT,SO,DF,CSAs...> >;

   if( IsUpper_v< MT > && sm.row( ) == sm.column( ) &&
      sm.rows( ) == sm.columns( ) && sm.rowdilation( ) == sm.columndilation( ) )
      return true;
   else return isUpper( static_cast<const BaseType&>( sm ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given DilatedSubmatrix is an upper unitriangular matrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be checked.
// \return \a true if the DilatedSubmatrix is an upper unitriangular matrix, \a false if not.
//
// This function checks if the given sparse DilatedSubmatrix is an upper triangular matrix. The matrix
// is considered to be upper triangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        1      & u_{0,1} & u_{0,2} & \cdots & u_{0,N} \\
                        0      & 1       & u_{1,2} & \cdots & u_{1,N} \\
                        0      & 0       & 1       & \cdots & u_{2,N} \\
                        \vdots & \vdots  & \vdots  & \ddots & \vdots  \\
                        0      & 0       & 0       & \cdots & 1       \\
                        \end{array}\right).\f]

// The following code example demonstrates the use of the function:

   \code
   blaze::DynamicMatrix<int,blaze::rowMajor> A( 32UL, 16UL );
   // ... Initialization

   auto sm = DilatedSubmatrix( A, 8UL, 8UL, 16UL, 16UL );

   if( isUniUpper( sm ) ) { ... }
   \endcode
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isUniUpper( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm )
{
   using BaseType = BaseType_t< DilatedSubmatrix<MT,SO,DF,CSAs...> >;

   if( IsUniUpper_v< MT > && sm.row( ) == sm.column( ) &&
      sm.rows( ) == sm.columns( ) && sm.rowdilation( ) == sm.columndilation( ) )
      return true;
   else return isUniUpper( static_cast<const BaseType&>( sm ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given DilatedSubmatrix is a strictly upper triangular matrix.
// \ingroup DilatedSubmatrix
//
// \param sm The DilatedSubmatrix to be checked.
// \return \a true if the DilatedSubmatrix is a strictly upper triangular matrix, \a false if not.
//
// This function checks if the given sparse DilatedSubmatrix is a strictly upper triangular matrix. The
// matrix is considered to be upper triangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        0      & u_{0,1} & u_{0,2} & \cdots & u_{0,N} \\
                        0      & 0       & u_{1,2} & \cdots & u_{1,N} \\
                        0      & 0       & 0       & \cdots & u_{2,N} \\
                        \vdots & \vdots  & \vdots  & \ddots & \vdots  \\
                        0      & 0       & 0       & \cdots & 0       \\
                        \end{array}\right).\f]

// The following code example demonstrates the use of the function:

   \code
   blaze::DynamicMatrix<int,blaze::rowMajor> A( 32UL, 16UL );
   // ... Initialization

   auto sm = DilatedSubmatrix( A, 8UL, 8UL, 16UL, 16UL );

   if( isStrictlyUpper( sm ) ) { ... }
   \endcode
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isStrictlyUpper( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm )
{
   using BaseType = BaseType_t< DilatedSubmatrix<MT,SO,DF,CSAs...> >;

   if( IsStrictlyUpper_v< MT > && sm.row( ) == sm.column( ) &&
      sm.rows( ) == sm.columns( ) && sm.rowdilation( ) == sm.columndilation( ) )
      return true;
   else return isStrictlyUpper( static_cast<const BaseType&>( sm ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the given matrix and DilatedSubmatrix represent the same observable state.
// \ingroup DilatedSubmatrix
//
// \param a The DilatedSubmatrix to be tested for its state.
// \param b The matrix to be tested for its state.
// \return \a true in case the DilatedSubmatrix and matrix share a state, \a false otherwise.
//
// This overload of the isSame function tests if the given DilatedSubmatrix refers to the full given
// matrix and by that represents the same observable state. In this case, the function returns
// \a true, otherwise it returns \a false.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isSame( const DilatedSubmatrix<MT,SO,DF,CSAs...>& a, const Matrix<MT,SO>& b ) noexcept
{
   return ( isSame( a.operand( ), ~b ) && ( a.rows( ) == ( ~b ).rows( ) ) &&
      ( a.columns( ) == ( ~b ).columns( ) ) && ( a.rowdilation( ) == 1UL ) &&
      ( a.columndilation( ) == 1UL ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the given matrix and DilatedSubmatrix represent the same observable state.
// \ingroup DilatedSubmatrix
//
// \param a The matrix to be tested for its state.
// \param b The DilatedSubmatrix to be tested for its state.
// \return \a true in case the matrix and DilatedSubmatrix share a state, \a false otherwise.
//
// This overload of the isSame function tests if the given DilatedSubmatrix refers to the full given
// matrix and by that represents the same observable state. In this case, the function returns
// \a true, otherwise it returns \a false.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline bool isSame( const Matrix<MT,SO>& a, const DilatedSubmatrix<MT,SO,DF,CSAs...>& b ) noexcept
{
   return ( isSame( ~a, b.operand( ) ) && ( ( ~a ).rows( ) == b.rows( ) ) &&
      ( ( ~a ).columns( ) == b.columns( ) ) && ( a.rowdilation( ) == 1UL ) &&
      ( a.columndilation( ) == 1UL ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the two given submatrices represent the same observable state.
// \ingroup DilatedSubmatrix
//
// \param a The first DilatedSubmatrix to be tested for its state.
// \param b The second DilatedSubmatrix to be tested for its state.
// \return \a true in case the two submatrices share a state, \a false otherwise.
//
// This overload of the isSame function tests if the two given submatrices refer to exactly the
// same part of the same matrix. In case both submatrices represent the same observable state,
// the function returns \a true, otherwise it returns \a false.
*/
template< typename MT1       // Type of the matrix of the left-hand side DilatedSubmatrix
        , bool SO1           // Storage order of the left-hand side DilatedSubmatrix
        , bool DF1           // Density flag of the left-hand side DilatedSubmatrix
        , size_t... CSAs1    // Compile time DilatedSubmatrix arguments of the left-hand side DilatedSubmatrix
        , typename MT2       // Type of the matrix of the right-hand side DilatedSubmatrix
        , bool SO2           // Storage order of the right-hand side DilatedSubmatrix
        , bool DF2           // Density flag of the right-hand side DilatedSubmatrix
        , size_t... CSAs2 >  // Compile time DilatedSubmatrix arguments of the right-hand side DilatedSubmatrix
inline bool isSame( const DilatedSubmatrix<MT1,SO1,DF1,CSAs1...>& a,
                    const DilatedSubmatrix<MT2,SO2,DF2,CSAs2...>& b ) noexcept
{
   return ( isSame( a.operand(), b.operand() ) &&
            ( a.row() == b.row() ) && ( a.column() == b.column() ) &&
            ( a.rows() == b.rows() ) && ( a.columns() == b.columns() ) &&
            ( a.rowdilation() == b.rowdilation() ) && ( a.columndilation() == b.columndilation() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief In-place inversion of the given dense DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The dense DilatedSubmatrix to be inverted.
// \return void
// \exception std::invalid_argument Invalid non-square matrix provided.
// \exception std::runtime_error Inversion of singular matrix failed.
//
// This function inverts the given dense DilatedSubmatrix by means of the specified matrix type or matrix
// inversion algorithm \c IF (see the InversionFlag documentation):

   \code
   invert<asLower>( A );     // Inversion of a lower triangular matrix
   invert<asUniUpper>( A );  // Inversion of an upper unitriangular matrix
   invert<byLU>( A );        // Inversion by means of an LU decomposition
   invert<byLLH>( A );       // Inversion by means of a Cholesky decomposition
   ...
   \endcode

// The matrix inversion fails if ...
//
//  - ... the given DilatedSubmatrix is not a square matrix;
//  - ... the given DilatedSubmatrix is singular and not invertible.
//
// In all failure cases either a compilation error is created if the failure can be predicted at
// compile time or an exception is thrown.
//
// \note The matrix inversion can only be used for dense matrices with \c float, \c double,
// \c complex<float> or \c complex<double> element type. The attempt to call the function with
// matrices of any other element type results in a compile time error!
//
// \note This function can only be used if a fitting LAPACK library is available and linked to
// the executable. Otherwise a linker error will be created.
//
// \note This function does only provide the basic exception safety guarantee, i.e. in case of an
// exception \a sm may already have been modified.
*/
template< InversionFlag IF  // Inversion algorithm
        , typename MT       // Type of the dense matrix
        , bool SO           // Storage order
        , size_t... CSAs >  // Compile time DilatedSubmatrix arguments
inline auto invert( DilatedSubmatrix<MT,SO,true,CSAs...>& sm )
   -> DisableIf_t< HasMutableDataAccess_v<MT> >
{
   using RT = ResultType_t< DilatedSubmatrix<MT,SO,true,CSAs...> >;

   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION  ( RT );
   BLAZE_CONSTRAINT_MUST_HAVE_MUTABLE_DATA_ACCESS( RT );

   RT tmp( sm );
   invert<IF>( tmp );
   sm = tmp;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by setting a single element of a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The target DilatedSubmatrix.
// \param i The row index of the element to be set.
// \param j The column index of the element to be set.
// \param value The value to be set to the element.
// \return \a true in case the operation would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename ET >     // Type of the element
inline bool trySet( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, size_t i, size_t j, const ET& value )
{
   BLAZE_INTERNAL_ASSERT( i < sm.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( j < sm.columns(), "Invalid column access index" );

   return trySet( sm.operand(), sm.row()+i*sm.rowdilation(), sm.column()+j*sm.columndilation(), value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by adding to a single element of a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The target DilatedSubmatrix.
// \param i The row index of the element to be modified.
// \param j The column index of the element to be modified.
// \param value The value to be added to the element.
// \return \a true in case the operation would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename ET >     // Type of the element
inline bool tryAdd( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, size_t i, size_t j, const ET& value )
{
   BLAZE_INTERNAL_ASSERT( i < sm.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( j < sm.columns(), "Invalid column access index" );

   return tryAdd( sm.operand(),sm.row()+i*sm.rowdilation(), sm.column()+j*sm.columndilation(), value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by subtracting from a single element of a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The target DilatedSubmatrix.
// \param i The row index of the element to be modified.
// \param j The column index of the element to be modified.
// \param value The value to be subtracted from the element.
// \return \a true in case the operation would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename ET >     // Type of the element
inline bool trySub( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, size_t i, size_t j, const ET& value )
{
   BLAZE_INTERNAL_ASSERT( i < sm.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( j < sm.columns(), "Invalid column access index" );

   return trySub( sm.operand(), sm.row()+i*sm.rowdilation(), sm.column()+j*sm.columndilation(), value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by scaling a single element of a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The target DilatedSubmatrix.
// \param i The row index of the element to be modified.
// \param j The column index of the element to be modified.
// \param value The factor for the element.
// \return \a true in case the operation would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename ET >     // Type of the element
inline bool tryMult( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, size_t i, size_t j, const ET& value )
{
   BLAZE_INTERNAL_ASSERT( i < sm.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( j < sm.columns(), "Invalid column access index" );

   return tryMult( sm.operand(), sm.row()+i*sm.rowdilation(), sm.column()+j*sm.columndilation(), value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by scaling a range of elements of a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The target DilatedSubmatrix.
// \param row The index of the first row of the range to be modified.
// \param column The index of the first column of the range to be modified.
// \param m The number of rows of the range to be modified.
// \param n The number of columns of the range to be modified.
// \param value The factor for the elements.
// \return \a true in case the operation would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename ET >     // Type of the element
BLAZE_ALWAYS_INLINE bool
   tryMult( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, size_t row, size_t column, size_t m, size_t n, const ET& value )
{
   MAYBE_UNUSED( column );

   BLAZE_INTERNAL_ASSERT( row <= (~sm).rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= (~sm).columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + m <= (~sm).rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + n <= (~sm).columns(), "Invalid number of columns" );

   return tryMult( sm.operand(), sm.row()+row*sm.rowdilation(), sm.column(), m*sm.rowdilation(), n, value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by scaling a single element of a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The target DilatedSubmatrix.
// \param i The row index of the element to be modified.
// \param j The column index of the element to be modified.
// \param value The divisor for the element.
// \return \a true in case the operation would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename ET >     // Type of the element
inline bool tryDiv( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, size_t i, size_t j, const ET& value )
{
   BLAZE_INTERNAL_ASSERT( i < sm.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( j < sm.columns(), "Invalid column access index" );

   return tryDiv( sm.operand(), sm.row()+i*sm.rowdilation(), sm.column()+j*sm.columndilation(), value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by scaling a range of elements of a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param sm The target DilatedSubmatrix.
// \param row The index of the first row of the range to be modified.
// \param column The index of the first column of the range to be modified.
// \param m The number of rows of the range to be modified.
// \param n The number of columns of the range to be modified.
// \param value The divisor for the elements.
// \return \a true in case the operation would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename ET >     // Type of the element
BLAZE_ALWAYS_INLINE bool
   tryDiv( const DilatedSubmatrix<MT,SO,DF,CSAs...>& sm, size_t row, size_t column, size_t m, size_t n, const ET& value )
{
   MAYBE_UNUSED( column );

   BLAZE_INTERNAL_ASSERT( row <= (~sm).rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= (~sm).columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + m <= (~sm).rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + n <= (~sm).columns(), "Invalid number of columns" );

   return tryDiv( sm.operand(), sm.row()+row*sm.rowdilation(), sm.column(), m*sm.rowdilation(), n, value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the assignment of a vector to a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side vector to be assigned.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename VT       // Type of the right-hand side vector
        , bool TF >         // Transpose flag of the right-hand side vector
inline bool tryAssign( const DilatedSubmatrix<MT,SO,DF,CSAs...>& lhs,
                       const Vector<VT,TF>& rhs, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( TF || ( row + (~rhs).size() <= lhs.rows() ), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( !TF || ( column + (~rhs).size() <= lhs.columns() ), "Invalid number of columns" );

   return tryAssign( lhs.operand(), ~rhs, lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the assignment of a vector to the band of a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side vector to be assigned.
// \param band The index of the band the right-hand side vector is assigned to.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename VT       // Type of the right-hand side vector
        , bool TF >         // Transpose flag of the right-hand side vector
inline bool tryAssign( const DilatedSubmatrix<MT,SO,DF,CSAs...>& lhs,
                       const Vector<VT,TF>& rhs, ptrdiff_t band, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + (~rhs).size() <= lhs.rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + (~rhs).size() <= lhs.columns(), "Invalid number of columns" );

   return tryAssign( lhs.operand(), ~rhs, band + ptrdiff_t( lhs.column() - lhs.row() ),
                     lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the assignment of a matrix to a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side matrix to be assigned.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT1      // Type of the matrix
        , bool SO1          // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename MT2      // Type of the right-hand side matrix
        , bool SO2 >        // Storage order of the right-hand side matrix
inline bool tryAssign( const DilatedSubmatrix<MT1,SO1,DF,CSAs...>& lhs,
                       const Matrix<MT2,SO2>& rhs, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + (~rhs).rows() <= lhs.rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + (~rhs).columns() <= lhs.columns(), "Invalid number of columns" );

   return tryAssign( lhs.operand(), ~rhs, lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the addition assignment of a vector to a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side vector to be added.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename VT       // Type of the right-hand side vector
        , bool TF >         // Transpose flag of the right-hand side vector
inline bool tryAddAssign( const DilatedSubmatrix<MT,SO,DF,CSAs...>& lhs,
                          const Vector<VT,TF>& rhs, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( TF || ( row + (~rhs).size() <= lhs.rows() ), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( !TF || ( column + (~rhs).size() <= lhs.columns() ), "Invalid number of columns" );

   return tryAddAssign( lhs.operand(), ~rhs, lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the addition assignment of a vector to the band of
//        a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side vector to be added.
// \param band The index of the band the right-hand side vector is assigned to.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename VT       // Type of the right-hand side vector
        , bool TF >         // Transpose flag of the right-hand side vector
inline bool tryAddAssign( const DilatedSubmatrix<MT,SO,DF,CSAs...>& lhs,
                          const Vector<VT,TF>& rhs, ptrdiff_t band, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + (~rhs).size() <= lhs.rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + (~rhs).size() <= lhs.columns(), "Invalid number of columns" );

   return tryAddAssign( lhs.operand(), ~rhs, band + ptrdiff_t( lhs.column() - lhs.row() ),
                        lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the addition assignment of a matrix to a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side matrix to be added.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT1      // Type of the matrix
        , bool SO1          // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename MT2      // Type of the right-hand side matrix
        , bool SO2 >        // Storage order of the right-hand side matrix
inline bool tryAddAssign( const DilatedSubmatrix<MT1,SO1,DF,CSAs...>& lhs,
                          const Matrix<MT2,SO2>& rhs, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + (~rhs).rows() <= lhs.rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + (~rhs).columns() <= lhs.columns(), "Invalid number of columns" );

   return tryAddAssign( lhs.operand(), ~rhs, lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the subtraction assignment of a vector to a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side vector to be subtracted.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename VT       // Type of the right-hand side vector
        , bool TF >         // Transpose flag of the right-hand side vector
inline bool trySubAssign( const DilatedSubmatrix<MT,SO,DF,CSAs...>& lhs,
                          const Vector<VT,TF>& rhs, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( TF || ( row + (~rhs).size() <= lhs.rows() ), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( !TF || ( column + (~rhs).size() <= lhs.columns() ), "Invalid number of columns" );

   return trySubAssign( lhs.operand(), ~rhs, lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the subtraction assignment of a vector to the band of
//        a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side vector to be subtracted.
// \param band The index of the band the right-hand side vector is assigned to.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename VT       // Type of the right-hand side vector
        , bool TF >         // Transpose flag of the right-hand side vector
inline bool trySubAssign( const DilatedSubmatrix<MT,SO,DF,CSAs...>& lhs,
                          const Vector<VT,TF>& rhs, ptrdiff_t band, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + (~rhs).size() <= lhs.rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + (~rhs).size() <= lhs.columns(), "Invalid number of columns" );

   return trySubAssign( lhs.operand(), ~rhs, band + ptrdiff_t( lhs.column() - lhs.row() ),
                        lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the subtraction assignment of a matrix to a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side matrix to be subtracted.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT1      // Type of the matrix
        , bool SO1          // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename MT2      // Type of the right-hand side matrix
        , bool SO2 >        // Storage order of the right-hand side matrix
inline bool trySubAssign( const DilatedSubmatrix<MT1,SO1,DF,CSAs...>& lhs,
                          const Matrix<MT2,SO2>& rhs, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + (~rhs).rows() <= lhs.rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + (~rhs).columns() <= lhs.columns(), "Invalid number of columns" );

   return trySubAssign( lhs.operand(), ~rhs, lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the multiplication assignment of a vector to a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side vector to be multiplied.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename VT       // Type of the right-hand side vector
        , bool TF >         // Transpose flag of the right-hand side vector
inline bool tryMultAssign( const DilatedSubmatrix<MT,SO,DF,CSAs...>& lhs,
                           const Vector<VT,TF>& rhs, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( TF || ( row + (~rhs).size() <= lhs.rows() ), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( !TF || ( column + (~rhs).size() <= lhs.columns() ), "Invalid number of columns" );

   return tryMultAssign( lhs.operand(), ~rhs, lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the multiplication assignment of a vector to the band
//        of a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side vector to be multiplied.
// \param band The index of the band the right-hand side vector is assigned to.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename VT       // Type of the right-hand side vector
        , bool TF >         // Transpose flag of the right-hand side vector
inline bool tryMultAssign( const DilatedSubmatrix<MT,SO,DF,CSAs...>& lhs,
                           const Vector<VT,TF>& rhs, ptrdiff_t band, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + (~rhs).size() <= lhs.rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + (~rhs).size() <= lhs.columns(), "Invalid number of columns" );

   return tryMultAssign( lhs.operand(), ~rhs, band + ptrdiff_t( lhs.column() - lhs.row() ),
                         lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the Schur product assignment of a matrix to a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side matrix for the Schur product.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT1      // Type of the matrix
        , bool SO1          // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename MT2      // Type of the right-hand side matrix
        , bool SO2 >        // Storage order of the right-hand side matrix
inline bool trySchurAssign( const DilatedSubmatrix<MT1,SO1,DF,CSAs...>& lhs,
                            const Matrix<MT2,SO2>& rhs, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + (~rhs).rows() <= lhs.rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + (~rhs).columns() <= lhs.columns(), "Invalid number of columns" );

   return trySchurAssign( lhs.operand(), ~rhs, lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the division assignment of a vector to a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side vector divisor.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename VT       // Type of the right-hand side vector
        , bool TF >         // Transpose flag of the right-hand side vector
inline bool tryDivAssign( const DilatedSubmatrix<MT,SO,DF,CSAs...>& lhs,
                          const Vector<VT,TF>& rhs, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( TF || ( row + (~rhs).size() <= lhs.rows() ), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( !TF || ( column + (~rhs).size() <= lhs.columns() ), "Invalid number of columns" );

   return tryDivAssign( lhs.operand(), ~rhs, lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the division assignment of a vector to the band of
//        a DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param lhs The target left-hand side DilatedSubmatrix.
// \param rhs The right-hand side vector divisor.
// \param band The index of the band the right-hand side vector is assigned to.
// \param row The row index of the first element to be modified.
// \param column The column index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t... CSAs    // Compile time DilatedSubmatrix arguments
        , typename VT       // Type of the right-hand side vector
        , bool TF >         // Transpose flag of the right-hand side vector
inline bool tryDivAssign( const DilatedSubmatrix<MT,SO,DF,CSAs...>& lhs,
                          const Vector<VT,TF>& rhs, ptrdiff_t band, size_t row, size_t column )
{
   BLAZE_INTERNAL_ASSERT( row <= lhs.rows(), "Invalid row access index" );
   BLAZE_INTERNAL_ASSERT( column <= lhs.columns(), "Invalid column access index" );
   BLAZE_INTERNAL_ASSERT( row + (~rhs).size() <= lhs.rows(), "Invalid number of rows" );
   BLAZE_INTERNAL_ASSERT( column + (~rhs).size() <= lhs.columns(), "Invalid number of columns" );

   return tryDivAssign( lhs.operand(), ~rhs, band + ptrdiff_t( lhs.column() - lhs.row() ),
                        lhs.row() + row * lhs.rowdilation(), lhs.column() + column * lhs.columndilation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Removal of all restrictions on the data access to the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param dm The DilatedSubmatrix to be derestricted.
// \return DilatedSubmatrix without access restrictions.
//
// This function removes all restrictions on the data access to the given DilatedSubmatrix. It returns a
// DilatedSubmatrix that does provide the same interface but does not have any restrictions on the data
// access.\n
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in the violation of invariants, erroneous results and/or in compilation errors.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t I          // Index of the first row
        , size_t J          // Index of the first column
        , size_t M          // Number of rows
        , size_t N          // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation >// Step between columns of the DilatedSubmatrix
inline decltype(auto) derestrict( DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>& dm )
{
   return dilatedsubmatrix<I,J,M,N,RowDilation,ColumnDilation>( derestrict( dm.operand() ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Removal of all restrictions on the data access to the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param dm The temporary DilatedSubmatrix to be derestricted.
// \return DilatedSubmatrix without access restrictions.
//
// This function removes all restrictions on the data access to the given temporary DilatedSubmatrix. It
// returns a DilatedSubmatrix that does provide the same interface but does not have any restrictions on
// the data access.\n
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in the violation of invariants, erroneous results and/or in compilation errors.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF           // Density flag
        , size_t I          // Index of the first row
        , size_t J          // Index of the first column
        , size_t M          // Number of rows
        , size_t N          // Number of columns
        , size_t RowDilation     // Step between rows of the DilatedSubmatrix
        , size_t ColumnDilation >// Step between columns of the DilatedSubmatrix
inline decltype(auto) derestrict( DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>&& dm )
{
   return dilatedsubmatrix<I,J,M,N,RowDilation,ColumnDilation>( derestrict( dm.operand() ), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Removal of all restrictions on the data access to the given DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param dm The DilatedSubmatrix to be derestricted.
// \return DilatedSubmatrix without access restrictions.
//
// This function removes all restrictions on the data access to the given DilatedSubmatrix. It returns a
// DilatedSubmatrix that does provide the same interface but does not have any restrictions on the data
// access.\n
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in the violation of invariants, erroneous results and/or in compilation errors.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF >         // Density flag
inline decltype(auto) derestrict( DilatedSubmatrix<MT,SO,DF>& dm )
{
   return dilatedsubmatrix( derestrict( dm.operand() ), dm.row(), dm.column(), dm.rows(), dm.columns(),
      dm.rowdilation(), dm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Removal of all restrictions on the data access to the given temporary DilatedSubmatrix.
// \ingroup DilatedSubmatrix
//
// \param dm The temporary DilatedSubmatrix to be derestricted.
// \return DilatedSubmatrix without access restrictions.
//
// This function removes all restrictions on the data access to the given temporary DilatedSubmatrix. It
// returns a DilatedSubmatrix that does provide the same interface but does not have any restrictions on
// the data access.\n
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in the violation of invariants, erroneous results and/or in compilation errors.
*/
template< typename MT       // Type of the matrix
        , bool SO           // Storage order
        , bool DF >         // Density flag
inline decltype(auto) derestrict( DilatedSubmatrix<MT,SO,DF>&& dm )
{
   return dilatedsubmatrix( derestrict( dm.operand() ), dm.row(), dm.column(), dm.rows(), dm.columns(),
      dm.rowdilation(), dm.columndilation(), unchecked );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  SIZE SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct Size< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>, 0UL >
   : public PtrdiffT<M>
{};

template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct Size< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>, 1UL >
   : public PtrdiffT<N>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  MAXSIZE SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct MaxSize< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>, 0UL >
   : public PtrdiffT<M>
{};

template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct MaxSize< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation>, 1UL >
   : public PtrdiffT<N>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISRESTRICTED SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t... CSAs >
struct IsRestricted< DilatedSubmatrix<MT,SO,DF,CSAs...> >
   : public IsRestricted<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  HASCONSTDATAACCESS SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, size_t... CSAs >
struct HasConstDataAccess< DilatedSubmatrix<MT,SO,true,CSAs...> >
   : public HasConstDataAccess<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  HASMUTABLEDATAACCESS SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, size_t... CSAs >
struct HasMutableDataAccess< DilatedSubmatrix<MT,SO,true,CSAs...> >
   : public HasMutableDataAccess<MT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISSYMMETRIC SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct IsSymmetric< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation> >
   : public BoolConstant< ( IsSymmetric_v<MT> && I == J && M == N && RowDilation == ColumnDilation) >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISHERMITIAN SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct IsHermitian< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation> >
   : public BoolConstant< ( IsHermitian_v<MT> && I == J && M == N && RowDilation == ColumnDilation) >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISLOWER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct IsLower< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation> >
   : public BoolConstant< ( IsLower_v<MT> && I == J && M == N && RowDilation == ColumnDilation ) ||
                          ( IsStrictlyLower_v<MT> && I == J+1UL && M == N && RowDilation == ColumnDilation) >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISUNILOWER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct IsUniLower< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation> >
   : public BoolConstant< ( IsUniLower_v<MT> && I == J && M == N && RowDilation == ColumnDilation) >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISSTRICTLYLOWER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct IsStrictlyLower< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation> >
   : public BoolConstant< ( IsLower_v<MT> && I < J && M == N && RowDilation == ColumnDilation) ||
                          ( IsStrictlyLower_v<MT> && I == J && M == N && RowDilation == ColumnDilation ) >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISUPPER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct IsUpper< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation> >
   : public BoolConstant< ( IsUpper_v<MT> && I == J && M == N && RowDilation == ColumnDilation) ||
                          ( IsStrictlyUpper_v<MT> && I+1UL == J && M == N && RowDilation == ColumnDilation ) >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISUNIUPPER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation >
struct IsUniUpper< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation> >
   : public BoolConstant< ( IsUniUpper_v<MT> && I == J && M == N && RowDilation == ColumnDilation ) >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISSTRICTLYUPPER SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, size_t I, size_t J, size_t M, size_t N, size_t RowDilation, size_t ColumnDilation>
struct IsStrictlyUpper< DilatedSubmatrix<MT,SO,DF,I,J,M,N,RowDilation,ColumnDilation> >
   : public BoolConstant< ( IsUpper_v<MT> && I > J && M == N && RowDilation == ColumnDilation) ||
                          ( IsStrictlyUpper_v<MT> && I == J && M == N && RowDilation == ColumnDilation) >
{};
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
