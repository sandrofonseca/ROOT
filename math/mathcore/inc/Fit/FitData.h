// @(#)root/mathcore:$Id: FitData.h 45076 2012-07-16 13:45:18Z mborinsk $
// Author: M. Borinsky 

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2006  LCG ROOT Math Team, CERN/PH-SFT                *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

// Header file for class DataVector

#ifndef ROOT_Fit_FitData
#define ROOT_Fit_FitData

/** 
@defgroup FitData Fit Data Classes 

Classes for describing the input data for fitting

@ingroup Fit
*/


#ifndef ROOT_Fit_DataOptions
#include "Fit/DataOptions.h"
#endif


#ifndef ROOT_Fit_DataRange
#include "Fit/DataRange.h"
#endif


#include <vector>
#include <cassert>
#include <iostream> 


namespace ROOT { 

   namespace Fit { 


/**
 * Base class for all the fit data types:
 * Stores the coordinates and the DataOptions

   @ingroup FitData
 */


/** 
   class holding the fit data points. It is template on the type of point,
   which can be for example a binned or unbinned point. 
   It structures the coordinate data in a vector< vector<double> > construct. 
   This way efficient access to the coordinates and data by SIMD 
   routines is made possible. Though it is not implemented in all 
   fit routines.

   @ingroup FitData

*/ 

class FitData 
{ 
public: 

  /// construct with default option and data range 
  explicit FitData( unsigned int maxpoints = 0, unsigned int dim = 1 );

  /// construct passing options and default data range 
  explicit FitData( const DataOptions & opt, unsigned int maxpoints = 0, unsigned int dim = 1 );


  /// construct passing range and default options 
  explicit FitData( const DataRange & range, unsigned int maxpoints = 0, unsigned int dim = 1 );

  /// construct passing options and data range 
  FitData ( const DataOptions & opt, const DataRange & range, 
    unsigned int maxpoints = 0, unsigned int dim = 1 );
  
  /// constructor from external data for 1D data
  FitData ( unsigned int n, const double * dataX );

  /// constructor from external data for 2D data
  FitData (unsigned int n, const double * dataX, const double * dataY );

  /// constructor from external data for 3D data
  FitData ( unsigned int n, const double * dataX, const double * dataY, 
    const double * dataZ );
  
  /**
    constructor for multi-dim external data and a range (data are copied inside according to the range)
    Uses as argument an iterator of a list (or vector) containing the const double * of the data
    An example could be the std::vector<const double *>::begin
  */
  FitData( const DataRange & range, unsigned int maxpoints, const double* dataX );
  
  /**
    constructor for multi-dim external data and a range (data are copied inside according to the range)
    Uses as argument an iterator of a list (or vector) containing the const double * of the data
    An example could be the std::vector<const double *>::begin
  */
  FitData( const DataRange & range, unsigned int maxpoints, const double* dataX, const double* dataY );
  
  /**
    constructor for multi-dim external data and a range (data are copied inside according to the range)
    Uses as argument an iterator of a list (or vector) containing the const double * of the data
    An example could be the std::vector<const double *>::begin
  */
  FitData( const DataRange & range, unsigned int maxpoints, const double* dataX, const double* dataY, 
    const double* dataZ );
  
  /**
    constructor for multi-dim external data (data are not copied inside)
    Uses as argument an iterator of a list (or vector) containing the const double * of the data
    An example could be the std::vector<const double *>::begin
    In case of weighted data, the external data must have a dim+1 lists of data 
    The apssed dim refers just to the coordinate size
  */
  template<class Iterator> 
  FitData ( unsigned int n, unsigned int dim, Iterator dataItr ) : 
    fWrapped( true ), 
    fMaxPoints( n ), 
    fNPoints( fMaxPoints ), 
    fDim( dim ), 
    fCoordsPtr( fDim ), 
    fpTmpCoordVector( NULL )
  {
    assert( fDim >= 1 );
    for ( unsigned int i=0; i < fDim; i++ )
    {
      fCoordsPtr[i] = *dataItr++;
    }
    
    if ( fpTmpCoordVector )
    {
      delete fpTmpCoordVector;
      fpTmpCoordVector = NULL;
    }
    
    fpTmpCoordVector = new double [fDim]; 
  }
  
  /**
    constructor for multi-dim external data and a range (data are copied inside according to the range)
    Uses as argument an iterator of a list (or vector) containing the const double * of the data
    An example could be the std::vector<const double *>::begin
  */
  template<class Iterator> 
  FitData( const DataRange & range, unsigned int maxpoints, unsigned int dim, Iterator dataItr ) : 
    fWrapped( false ), 
    fRange( range ), 
    fMaxPoints( maxpoints ), 
    fNPoints( 0 ), 
    fDim( dim ), 
    fpTmpCoordVector( NULL )
  {
    assert( fDim >= 1 );
    InitCoordsVector( );
    
    InitFromRange( dataItr );
  }
  
  /// dummy virtual destructor
  virtual ~FitData( );

  FitData ( const FitData & rhs );

  FitData & operator= ( const FitData & rhs );
  
  void Append( unsigned int newPoints, unsigned int dim = 1 );

protected:
  /** 
   * initializer routines to set the corresponding pointers right
   * The vectors must NOT be resized after this initialization 
   * without setting the corresponding pointers in the 
   * same moment ( has to be an atomic operation in case 
   * of multithreading ).
  */ 
  void InitCoordsVector ()
  {
    fCoords.resize( fDim );
    fCoordsPtr.resize( fDim );
    for( unsigned int i=0; i < fDim; i++ )
    {
      fCoords[i].resize( fMaxPoints );
      fCoordsPtr[i] = &fCoords[i].front();
    }
    
    if ( fpTmpCoordVector )
    {
      delete fpTmpCoordVector;
      fpTmpCoordVector = NULL;
    }
    
    fpTmpCoordVector = new double [fDim]; 
  }

  /** template function to initialize the Data from arbitrary iterator.
   **/
  template<class Iterator> 
  void InitFromRange( Iterator dataItr )
  {
    for ( unsigned int i = 0; i < fMaxPoints; i++ ) 
    { 
      bool isInside = true;
      Iterator tmpItr = dataItr;
      
      for ( unsigned int j = 0; j < fDim; j++ )  
         isInside &= fRange.IsInside( (*tmpItr++)[i], j );
      
      if ( isInside )
      {
        tmpItr = dataItr;
        
        for ( unsigned int k = 0; k < fDim; k++ )
          fpTmpCoordVector[k] = (*tmpItr++)[i];
          
        Add( fpTmpCoordVector );
      }
    }
  }
  

public: 
  
  /**
    returns a single coordinate component of a point. 
    This function is threadsafe in contrast to Coords(...)
    and can easily get vectorized by the compiler in loops 
    running over the ipoint-index.
  */  
  double GetCoordComponent( unsigned int ipoint, unsigned int icoord ) const  __attribute__ ((pure))
  {
    assert( ipoint < fMaxPoints );
    assert( icoord < fDim );
    assert( fCoordsPtr.size() == fDim );
    assert( fCoordsPtr[icoord] );
    assert( fCoords.empty() || &fCoords[icoord].front() == fCoordsPtr[icoord] );
    
    return fCoordsPtr[icoord][ipoint];
  }
  
  /**
    return a pointer to the coordinates data for the given fit point 
  */  
  // not threadsafe, to be replaced with never constructs!
  // for example: just return std::array or std::vector, there's 
  // is going to be only minor overhead in c++11.
  const double * Coords( unsigned int ipoint ) const
  { 
    assert( fpTmpCoordVector );
    assert( ipoint < fMaxPoints );
    
    for ( unsigned int i=0; i < fDim; i++ )
    {
      assert( fCoordsPtr[i] );
      assert( fCoords.empty() || &fCoords[i].front() == fCoordsPtr[i] );
      
      fpTmpCoordVector[i] = fCoordsPtr[i][ipoint];
    }
    
    return fpTmpCoordVector;
  }
  
  /**
    add one dim data with only coordinate and values
  */
  void Add( double x )
  {
    assert( !fWrapped );
    assert( !fCoordsPtr.empty() && fCoordsPtr.size() == 1 && fCoordsPtr[0] );
    assert( 1 == fDim );
    assert( fNPoints < fMaxPoints );
    
    fCoords[0][ fNPoints ] = x;
    
    fNPoints++;
  }
  
  /**
    add multi-dim coordinate data with only value 
  */
  void Add( const double *x )
  {
    assert( !fWrapped );
    assert( !fCoordsPtr.empty() && fCoordsPtr.size() == fDim );
    assert( fNPoints < fMaxPoints );
    
    for( unsigned int i=0; i < fDim; i++ )
    {
      fCoords[i][ fNPoints ] = x[i];
    }
    
    fNPoints++;
  }

  /**
    return number of fit points
  */
  unsigned int NPoints() const { return fNPoints; } 

  /**
    return number of fit points 
  */ 
  unsigned int Size() const { return fNPoints; }
  
  /**
    return coordinate data dimension
  */
  unsigned int NDim() const { 
	return fDim; } 

  /**
    access to options
  */
  const DataOptions & Opt() const { return fOptions; }
  DataOptions & Opt() { return fOptions; }

  /**
    access to range
  */
  const DataRange & Range() const { return fRange; }
  
  /**
    direct access to coord data ptrs
  */
  const std::vector< const double* >& GetCoordDataPtrs() const
  {
    return fCoordsPtr;
  }


protected:
  /** 
      copies the data to the local vector< vector < double > > structure 
      this is needed, if the data is going to be manipulated for some reason. 
  */
  void UnWrap( )
  {
    assert( fWrapped );
    assert( fCoords.empty() );
    
    fCoords.resize( fDim );
    for( unsigned int i=0; i < fDim; i++ )
    {
      assert( fCoordsPtr[i] );
      fCoords[i].resize( fNPoints );
      std::copy( fCoordsPtr[i], fCoordsPtr[i] + fNPoints, fCoords[i].begin() );
      fCoordsPtr[i] = &fCoords[i].front();
    }
    
    fWrapped = false;
  }

protected:
  bool          fWrapped; // true if no data is hold by the class and only pointers on the data are set.

private:
  DataOptions   fOptions; 
  DataRange     fRange;
  
protected:
  unsigned int  fMaxPoints; // Maximal number of points
  unsigned int  fNPoints;   // Actual number of points
  unsigned int  fDim;       // Dimension of the data.
  
private:
  /** 
   * This vector stores the vectorizable data:
   * The inner vectors contain the coordinates data
   * fCoords[0] is the vector for the x-coords
   * fCoords[1] is the vector for the y-coords
   * etc.
   * The vector of pointers stores the pointers 
   * to the first elements of the corresponding 
   * elements
   * 
   * If fWrapped is true, fCoords is empty. 
   * the data can only be accessed by using 
   * fCoordsPtr.
  */
  std::vector< std::vector< double > > fCoords;   // array for the coordinates ( if they are stored directly in this class )

  std::vector< const double* > fCoordsPtr; // stores the pointer to the actual data. if fWrapped is false this will be the 
  //  point to the fCoords array.
  
  double* fpTmpCoordVector; // non threadsafe stuff!
  // temporary pointer to array for returning coords by value.
};
      

   } // end namespace Fit

} // end namespace ROOT



#endif /* ROOT_Fit_Data */

