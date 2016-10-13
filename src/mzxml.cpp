/***************************************************************************
*                                                                          *
*  This program is free software; you can redistribute it and/or modify    *
*  it under the terms of the GNU Library or "Lesser" General Public        *
*  License (LGPL) as published by the Free Software Foundation;            *
*  either version 2 of the License, or (at your option) any later          *
*  version.                                                                *
***************************************************************************/

#include "mzxml.hpp"

MzxmlReader::MzxmlReader( char* fileName ) {
  m_currentScan = -1;
  m_pPeaks = NULL;
  
  m_fileName = fileName;
    
  // Open file
  if ( (m_pFI = rampOpenFile( fileName )) == NULL)
    {
      cerr << "could not open input file " << fileName;
      exit(1);
    }

  // Get the index
  m_indexOffset = getIndexOffset( m_pFI );
  m_pScanIndex = readIndex( m_pFI , m_indexOffset, &m_iAnalysisLastScan );
  m_iAnalysisFirstScan = 1;
}

MzxmlReader::~MzxmlReader() {
  rampCloseFile(m_pFI);

  free( m_pPeaks );
  free( m_pScanIndex );
}


// Load scan scanNum
// Header information will be in m_scanHeader
// peaks will be in m_pPeaks
int MzxmlReader::loadScan( int scanNum ) {
  // Load header information
  readHeader( m_pFI, m_pScanIndex[scanNum], &m_scanHeader );

  // Cleanup any previous allocation of peaks
  free( m_pPeaks );

  // Load peaks
  m_pPeaks = readPeaks( m_pFI, m_pScanIndex[scanNum]);
  
  m_currentScan = scanNum;
}


struct MyScanHeaderStruct MzxmlReader::getHeader( int scanNum ) {
  if( scanNum != m_currentScan)
    loadScan( scanNum );
  
  return m_scanHeader;
}


RAMPREAL * MzxmlReader::getPeaks( int scanNum ) {
  if( scanNum != m_currentScan)
    loadScan( scanNum );

  return m_pPeaks;
}

void MzxmlReader::removeZeroIntPeaks( ) {
  int n, p;
  RAMPREAL *pTemp = NULL;

  pTemp = (RAMPREAL *) malloc((m_scanHeader.peaksCount+1) * 2 * sizeof(RAMPREAL) + 1);

  p = 0;
  for( n = 0 ; n < 2 * m_scanHeader.peaksCount ; n+=2 ) {
    if( m_pPeaks[n+1] != 0 ) {
      // If the intensity is not zero
      pTemp[p] = m_pPeaks[n];
      p+=1;
      pTemp[p] = m_pPeaks[n+1];
      p+=1;
    }
  }

  free( m_pPeaks );
  m_pPeaks = pTemp;
  m_scanHeader.peaksCount = p/2;
}

int MzxmlReader::printPeaks( int scanNum ) {
  int n;
  int peakNum = 1;

  if( scanNum != m_currentScan)
    loadScan( scanNum ); 

  cout << m_scanHeader.peaksCount << " peaks" << endl;
  for( n = 0 ; n < 2 * m_scanHeader.peaksCount ; n+=2 ) {
    cout << peakNum++ << "\t" << (double) m_pPeaks[n] << "\t" << (double) m_pPeaks[n+1] << endl;
  } 
}


int MzxmlReader::printScanHeader( int scanNum ) {

    cout << "seqNum\t" << m_scanHeader.seqNum << endl;
    cout << "acquisitionNum\t" << m_scanHeader.acquisitionNum << endl;
    cout << "msLevel\t" << m_scanHeader.msLevel << endl;
    cout << "peaksCount\t" << m_scanHeader.peaksCount << endl;
    cout << "totIonCurrent\t" << m_scanHeader.totIonCurrent << endl;
    cout << "retentionTime\t" << m_scanHeader.retentionTime << endl;
    cout << "basePeakMZ\t" << m_scanHeader.basePeakMZ << endl;
    cout << "basePeakIntensity\t" << m_scanHeader.basePeakIntensity << endl;
    cout << "collisionEnergy\t" << m_scanHeader.collisionEnergy << endl;
    cout << "compensationVoltage\t" << m_scanHeader.compensationVoltage << endl;
    cout << "ionisationEnergy\t" << m_scanHeader.ionisationEnergy << endl;
    cout << "lowMZ\t" << m_scanHeader.lowMZ << endl;
    cout << "highMZ\t" << m_scanHeader.highMZ << endl;
    cout << "precursorScanNum\t" << m_scanHeader.precursorScanNum << endl;
    cout << "precursorMZ\t" << m_scanHeader.precursorMZ << endl;
    cout << "precursorCharge\t" << m_scanHeader.precursorCharge << endl;
    cout << "precursorIntensity\t" << m_scanHeader.precursorIntensity << endl;
    cout << "scanType\t" << m_scanHeader.scanType << endl;
    cout << "activationMethod\t" << m_scanHeader.activationMethod << endl;
    cout << "possibleCharges\t" << m_scanHeader.possibleCharges << endl;
    cout << "numPossibleCharges\t" << m_scanHeader.numPossibleCharges << endl;
    cout << "possibleChargesArray\t";
    for( int n = 1 ; n <= m_scanHeader.numPossibleCharges ; ++n ) {
      cout << m_scanHeader.possibleChargesArray[n];
    }
    cout << endl;
    cout << "mergedScan\t" << m_scanHeader.mergedScan << endl;
    cout << "mergedResultScanNum\t" << m_scanHeader.mergedResultScanNum << endl;
    cout << "mergedResultStartScanNum\t" << m_scanHeader.mergedResultStartScanNum << endl;
    cout << "mergedResultEndScanNum\t" << m_scanHeader.mergedResultEndScanNum << endl;
    cout << "filterLine\t" << m_scanHeader.filterLine << endl;

}
