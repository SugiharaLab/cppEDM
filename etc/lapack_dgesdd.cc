
// g++ lapack_dgesdd.cc -o dgesdd -std=c++11 -g -I../src -L../lib -lstdc++ -llapack -DDEBUG

#include <stdexcept>
#include <iostream>
#include <valarray>

//-------------------------------------------------------------------------
// NOTE: See https://computing.llnl.gov/tutorials/bgq/mixedProgramming2.pdf
// All fortran arguments must be pointers
// DOUBLE PRECISION = REAL*8 = c++ double
//-------------------------------------------------------------------------
// subroutine dgesdd()
//-----------------------------------------------------------------------
// http://www.netlib.org/lapack/explore-html/db/db4/dgesdd_8f.html
//
//  JOBZ is CHARACTER*1
//          Specifies options for computing all or part of the matrix U:
//          = 'A':  all M columns of U and all N rows of V**T are
//                  returned in the arrays U and VT;
//          = 'S':  the first min(M,N) columns of U and the first
//                  min(M,N) rows of V**T are returned in the arrays U and VT;
//  INFO is INTEGER
//           = 0:  successful exit.
//           < 0:  if INFO = -i, the i-th argument had an illegal value.
//           > 0:  DBDSDC did not converge, updating process failed.
//-----------------------------------------------------------------------
extern "C" {
    void dgesdd_( const char *JOBZ,
                  int        *M,
                  int        *N,
                  double     *A,
                  int        *LDA,
                  double     *S,
                  double     *U,
                  int        *LDU,
                  double     *VT,
                  int        *LDVT,
                  double     *WORK,
                  int        *LWORK,
                  int        *IWORK,
                  int        *INFO );
}

//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
void Lapack_SVD(int m,     // number of rows in matrix
                int n,     // number of columns in matrix
                int lda,   // leading dimension of matrix
                double *a) // pointer to top-left corner
{
    // s to hold singular values; u and vt for matrices U and Vt
    int    N_SingularValues = m < n ? m : n;
    double s [ N_SingularValues ];
    double u [ m*m ];
    double vt[ n*n ];

    // Workspace and info variables:
    int    iwork[ 8 * N_SingularValues ];
    double workSize = 0;  // To query optimal work size
    int    lwork    = -1; // To query optimal work size
    int    info     = 0;

#ifdef DEBUG
    std::cout << "m.row=" << m << " n.col=" << n << " lda=" << lda
              << " s.n=" << N_SingularValues << std::endl;
    
    for ( size_t i = 0; i < m*n; i++ ) {
        std::cout << a[i] << " ";
    } std::cout << std::endl;
#endif
    
    // Call dgesdd_ with lwork = -1 to query optimal workspace size:
    dgesdd_( "S", &m, &n, a, &lda, s, u, &m,
             vt, &n, &workSize, &lwork, iwork, &info );
    if ( info ) {
        throw std::runtime_error( "dgesdd_ failed on query.\n" );
    }
    std::cout << "Optimal work size is " << workSize << std::endl;
    
    // Optimal workspace size is returned in workSize.
    double work[ (size_t) workSize ]; 
    lwork = (int) workSize;

    // Call dgesdd_ for SVD using lwork workSize:
    dgesdd_( "S", &m, &n, a, &lda, s, u, &m,
             vt, &n, work, &lwork, iwork, &info );
    if ( info ) {
        throw std::runtime_error( "dgesdd_ failed.\n" );
    }

    std::cout << "Singular values: [ ";
    for ( auto i = 0; i < N_SingularValues; i++ ) {
        std::cout << s[i] << " ";
    } std::cout << "]" << std::endl;
}

//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
int main( int argc, char *argv[] ) {
    //>>> import numpy as np
    //>>> A = np.array( [ [1,2,3], [2,2,1] ] ) # 2x3 array
    //>>> u,s,vh = np.linalg.svd( A )
    //>>> s
    //array([4.56516931, 1.46943158])

    // lapack Fortran is column major format, C is row major
    std::valarray<double> A( {1.,2.,3.,2.,2.,1.} );

    // This gives the wrong answer since row major n,m are specified
    Lapack_SVD( 2,   // number of rows in matrix
                3,   // number of columns in matrix
                3,   // leading dimension of matrix
                &(A[0]) );

    // Right answer by switching m,n to enact column major format
    std::valarray<double> B( {1.,2.,3.,2.,2.,1.} );
    Lapack_SVD( 3,   // number of rows in matrix
                2,   // number of columns in matrix
                3,   // leading dimension of matrix
                &(B[0]) );
    return 0;
}
