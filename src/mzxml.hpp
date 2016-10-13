/***************************************************************************
*                                                                          *
*  This program is free software; you can redistribute it and/or modify    *
*  it under the terms of the GNU Library or "Lesser" General Public        *
*  License (LGPL) as published by the Free Software Foundation;            *
*  either version 2 of the License, or (at your option) any later          *
*  version.                                                                *
***************************************************************************/

#ifndef _MZXML_H
#define _MZXML_H

#include <tuple>
#include <vector>
#include "ramp/cramp.hpp"

using namespace std;

struct MyScanHeaderStruct : ScanHeaderStruct {

  // Convert these variables to arrays to something else so that we
  // can overload == more easily
  std::string scanType;
  std::string activationMethod;
  std::string possibleCharges;
  std::string filterLine;
  std::vector<bool> possibleChargesArray;
  
  // Compare 2 ScanHeaderStruct
  // We are not using memcmp since filePosition could change, but we wouldn't care about it.
  friend bool operator==(const MyScanHeaderStruct& lhs, const MyScanHeaderStruct& rhs) {
    return std::tie(lhs.seqNum,
		    lhs.acquisitionNum,
		    lhs.msLevel,
		    lhs.peaksCount,
		    lhs.totIonCurrent,
		    lhs.retentionTime,
		    lhs.basePeakMZ,
		    lhs.basePeakIntensity,
		    lhs.collisionEnergy,
		    lhs.compensationVoltage,
		    lhs.ionisationEnergy,
		    lhs.lowMZ,
		    lhs.highMZ,
		    lhs.precursorScanNum,
		    lhs.precursorMZ,
		    lhs.precursorCharge,
		    lhs.precursorIntensity,
		    lhs.scanType,
		    lhs.activationMethod,
		    lhs.possibleCharges,
		    lhs.numPossibleCharges,
		    lhs.possibleChargesArray,
		    lhs.mergedScan,
		    lhs.mergedResultScanNum,
		    lhs.mergedResultStartScanNum,
		    lhs.mergedResultEndScanNum,
		    lhs.filterLine
		    )
      ==
      std::tie(rhs.seqNum,
	       rhs.acquisitionNum,
	       rhs.msLevel,
	       rhs.peaksCount,
	       rhs.totIonCurrent,
	       rhs.retentionTime,
	       rhs.basePeakMZ,
	       rhs.basePeakIntensity,
	       rhs.collisionEnergy,
	       rhs.compensationVoltage,
	       rhs.ionisationEnergy,
	       rhs.lowMZ,
	       rhs.highMZ,
	       rhs.precursorScanNum,
	       rhs.precursorMZ,
	       rhs.precursorCharge,
	       rhs.precursorIntensity,
	       rhs.scanType,
	       rhs.activationMethod,
	       rhs.possibleCharges,
	       rhs.numPossibleCharges,
	       rhs.possibleChargesArray,
	       rhs.mergedScan,
	       rhs.mergedResultScanNum,
	       rhs.mergedResultStartScanNum,
	       rhs.mergedResultEndScanNum,
	       rhs.filterLine
	       );
    
  }
  
};

class MzxmlReader {
public:
  MzxmlReader( char* fileName );
  ~MzxmlReader();

  // Returns the total number of scans in the file
  int getTotScans() {
    return ( m_iAnalysisLastScan - m_iAnalysisFirstScan + 1 );
  }

  // Returns the first scan number
  int getFirstScan() {
    return m_iAnalysisFirstScan;
  }

  // Returns the last scan number
  int getLastScan() {
    return m_iAnalysisLastScan;
  }

  // Returns the name of the file associated with this parser
  string getFileName() {
    return m_fileName;
  }
  
  // Returns the scanHeader for scan number scanNum
  struct MyScanHeaderStruct getHeader( int scanNum );

  // Returns the peaks for scan number scanNum
  RAMPREAL * getPeaks( int scanNum );

  // Removes zero intensity m/z values from the peaklist
  // NOTE this also adjusts the peaksCount value in the scanHeader
  void removeZeroIntPeaks( );
  
  int printPeaks( int scanNum );

  int printScanHeader( int scanNum );
  
private:
  int m_currentScan;	// Scan number for the currently loaded scan
  int m_iAnalysisFirstScan, m_iAnalysisLastScan;
  string m_fileName;
  RAMPFILE *m_pFI;
  ramp_fileoffset_t m_indexOffset;
  ramp_fileoffset_t  *m_pScanIndex;
  struct MyScanHeaderStruct m_scanHeader;
  RAMPREAL *m_pPeaks;

  int loadScan( int scanNum );
};

#endif
