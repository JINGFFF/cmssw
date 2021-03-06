#ifndef CSCALCTStatusDigi_CSCALCTStatusDigi_h
#define CSCALCTStatusDigi_CSCALCTStatusDigi_h

/** \class CSCALCTStatusDigi
 *
 *  Digi for CSC ALCT info available in DDU
 *  
 *
 */

#include <vector>
#include <iosfwd>
#include <cstdint>

class CSCALCTStatusDigi{

public:

  /// Constructor for all variables 
  CSCALCTStatusDigi (const uint16_t * header, const uint16_t * trailer );

  /// Default constructor.
  CSCALCTStatusDigi () {}

  /// Data Accessors
  const uint16_t * header() const {return header_;}
  const uint16_t * trailer() const {return trailer_;}



private:

  uint16_t header_[116]; ///maximum header size is 116 words
  uint16_t trailer_[4];
};

std::ostream & operator<<(std::ostream & o, const CSCALCTStatusDigi& digi);

#endif
