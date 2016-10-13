////////////////////////////////////////////////////////////////////////////////////////////////
// Compares the content of two mzXML files.						      //
// 											      //
// WARNING! The program assumes that scans are consecutively numbered starting from 1.	      //
// If this is not the case use indexMzXML to fix it.					      //
// 											      //
// Copyright:	(C) 2016 Patrick Pedrioli <pedrioli@imsb.biol.ethz.ch>			      //
// Email:	pedrioli@imsb.biol.ethz.ch						      //
////////////////////////////////////////////////////////////////////////////////////////////////


/***************************************************************************
*                                                                          *
*  This program is free software; you can redistribute it and/or modify    *
*  it under the terms of the GNU Library or "Lesser" General Public        *
*  License (LGPL) as published by the Free Software Foundation;            *
*  either version 2 of the License, or (at your option) any later          *
*  version.                                                                *
***************************************************************************/


#include <iostream>
#include "mzxml.hpp"


int compareFiles( char *fileName1 , char *fileName2 , bool doHeaders = true , bool doPeaks = true , bool doZeroInt = true );
int detailledCompareHeaders( MyScanHeaderStruct h1 , MyScanHeaderStruct h2 );
void comparePeaks( int n , MyScanHeaderStruct &h1 , MyScanHeaderStruct &h2 , MzxmlReader &parser1 , MzxmlReader &parser2 );


#define MZ_TOL  0.0001

void help( string prgName ) {
  cout << "Compares the content of two mzXML files\n\n";
  cout << "Usage: " << prgName << " file1.mzXML file2.mzXML" << endl;
  exit(1);
}


int main(int argc, char *argv[])
{
  int n;
  bool doHeaders = true;
  bool doPeaks = true;
  bool doZeroInt = true;

  if( argc < 3 ) {
    cerr << "Incorrect number of parameters." << endl;
    help( argv[0] );
    exit( 1 );
  }

  for( n = 1 ; n < argc ; ++n ) {
    if( !strcmp( "--noHeaders" , argv[n] )) {
      doHeaders = false;
    }
    else if( !strcmp( "--noPeaks" , argv[n] )) {
      doPeaks = false;
    }
    else if( !strcmp( "--noZeroInt" , argv[n] )) {
      doZeroInt = false;
    }
  }

  // The last two parameters are the files to compare.
  n = n - 2;
  
  compareFiles( argv[n] , argv[n+1] , doHeaders , doPeaks , doZeroInt );
  
  return 0;
}


int compareFiles( char *fileName1 , char *fileName2 , bool doHeaders  , bool doPeaks , bool doZeroInt ) {
  int n;
  MyScanHeaderStruct h1, h2;
  
  cout << "> " << fileName1 << endl;
  cout << "< " << fileName2 << endl;
  
  // Prepare files for parsing
  MzxmlReader parser1( fileName1 );
  MzxmlReader parser2( fileName2 );

  // Do they have the same number of scans?
  if( parser1.getTotScans() != parser2.getTotScans() ) {
    cout << "Total number of scans in both files differ!" << endl;
    cout << parser1.getFileName() << " has " << parser1.getTotScans() << endl;
    cout << parser2.getFileName() << " has " << parser2.getTotScans() << endl;
    cout << "Note that this assumes that scans are sequentially numbered." << endl;
    cout << "If this is not the case use indexMzXML to fix files." << endl;
    cout << endl;

    exit(1);
  }
    
  // Compare scan by scan
  for( n = 1 ; n <= parser1.getTotScans() ; ++n ) {
    // Compare scanHeaders
    h1 = parser1.getHeader(n);
    h2 = parser2.getHeader(n);

    if( doHeaders ) {
      if( !(h1 == h2) )
	{
	  cout << "\nHeaders for scan " << n << " differ." << endl;
	  detailledCompareHeaders( h1, h2 );
	}
    }
    
    // Compare peaks
    if( doPeaks ) {
      if( doZeroInt ) {
	comparePeaks( n , h1 , h2 , parser1 , parser2 );
      }
      else {
	// Do not consider m/z values with zero intensity in comparison 
	parser1.removeZeroIntPeaks();
	parser2.removeZeroIntPeaks();

	// Reload scanHeader since we have modified the peaksCount
	h1 = parser1.getHeader(n);
	h2 = parser2.getHeader(n);

	comparePeaks( n , h1 , h2 , parser1 , parser2 );
      }
    }
  }
  
  return 0;
}


int detailledCompareHeaders( MyScanHeaderStruct h1 , MyScanHeaderStruct h2 ) {
  
  if( h1.seqNum != h2.seqNum ) {
    cout << "seqNum\t" << h1.seqNum << "\t" << h2.seqNum << endl;
  }
  if( h1.acquisitionNum != h2.acquisitionNum ) {
    cout << "acquisitionNum\t" << h1.acquisitionNum << "\t" << h2.acquisitionNum << endl;
  }
  if( h1.msLevel != h2.msLevel ) {
    cout << "msLevel\t" << h1.msLevel << "\t" << h2.msLevel << endl;
  }
  if( h1.peaksCount != h2.peaksCount ) {
    cout << "peaksCount\t" << h1.peaksCount << "\t" << h2.peaksCount << endl;
  }
  if( h1.totIonCurrent != h2.totIonCurrent ) {
    cout << "totIonCurrent\t" << h1.totIonCurrent << "\t" << h2.totIonCurrent << endl;
  }
  if( h1.retentionTime != h2.retentionTime ) {
    cout << "retentionTime\t" << h1.retentionTime << "\t" << h2.retentionTime << endl;
  }
  if( h1.basePeakMZ != h2.basePeakMZ ) {
    cout << "basePeakMZ\t" << h1.basePeakMZ << "\t" << h2.basePeakMZ << endl;
  }
  if( h1.basePeakIntensity != h2.basePeakIntensity ) {
    cout << "basePeakIntensity\t" << h1.basePeakIntensity << "\t" << h2.basePeakIntensity << endl;
  }
  if( h1.collisionEnergy != h2.collisionEnergy ) {
    cout << "collisionEnergy\t" << h1.collisionEnergy << "\t" << h2.collisionEnergy << endl;
  }
  if( h1.compensationVoltage != h2.compensationVoltage ) {
    cout << "compensationVoltage\t" << h1.compensationVoltage << "\t" << h2.compensationVoltage << endl;
  }
  if( h1.ionisationEnergy != h2.ionisationEnergy ) {
    cout << "ionisationEnergy\t" << h1.ionisationEnergy << "\t" << h2.ionisationEnergy << endl;
  }
  if( h1.lowMZ != h2.lowMZ ) {
    cout << "lowMZ\t" << h1.lowMZ << "\t" << h2.lowMZ << endl;
  }
  if( h1.highMZ != h2.highMZ ) {
    cout << "highMZ\t" << h1.highMZ << "\t" << h2.highMZ << endl;
  }
  if( h1.precursorScanNum != h2.precursorScanNum ) {
    cout << "precursorScanNum\t" << h1.precursorScanNum << "\t" << h2.precursorScanNum << endl;
  }
  if( h1.precursorMZ != h2.precursorMZ ) {
    cout << "precursorMZ\t" << h1.precursorMZ << "\t" << h2.precursorMZ << endl;
  }
  if( h1.precursorCharge != h2.precursorCharge ) {
    cout << "precursorCharge\t" << h1.precursorCharge << "\t" << h2.precursorCharge << endl;
  }
  if( h1.precursorIntensity != h2.precursorIntensity ) {
    cout << "precursorIntensity\t" << h1.precursorIntensity << "\t" << h2.precursorIntensity << endl;
  }
  if( h1.scanType != h2.scanType ) {
    cout << "scanType\t" << h1.scanType << "\t" << h2.scanType << endl;
  }
  if( h1.activationMethod != h2.activationMethod ) {
    cout << "activationMethod\t" << h1.activationMethod << "\t" << h2.activationMethod << endl;
  }
  if( h1.possibleCharges != h2.possibleCharges ) {
    cout << "possibleCharges\t" << h1.possibleCharges << "\t" << h2.possibleCharges << endl;
  }
  if( h1.numPossibleCharges != h2.numPossibleCharges ) {
    cout << "numPossibleCharges\t" << h1.numPossibleCharges << "\t" << h2.numPossibleCharges << endl;
  }
  if( h1.possibleChargesArray != h2.possibleChargesArray ) {
    cout << "possibleChargesArray\t";
    for( int n=0; n <= h1.numPossibleCharges; ++n ) {
      cout << h1.possibleChargesArray[n];
    }
    cout << "\t";
    for( int n=0; n <= h2.numPossibleCharges; ++n ) {
      cout << h2.possibleChargesArray[n];
    }
    cout << endl;
  }
  if( h1.mergedScan != h2.mergedScan ) {
    cout << "mergedScan\t" << h1.mergedScan << "\t" << h2.mergedScan << endl;
  }
  if( h1.mergedResultScanNum != h2.mergedResultScanNum ) {
    cout << "mergedResultScanNum\t" << h1.mergedResultScanNum << "\t" << h2.mergedResultScanNum << endl;
  }
  if( h1.mergedResultStartScanNum != h2.mergedResultStartScanNum ) {
    cout << "mergedResultStartScanNum\t" << h1.mergedResultStartScanNum << "\t" << h2.mergedResultStartScanNum << endl;
  }
  if( h1.mergedResultEndScanNum != h2.mergedResultEndScanNum ) {
    cout << "mergedResultEndScanNum\t" << h1.mergedResultEndScanNum << "\t" << h2.mergedResultEndScanNum << endl;
  }
  if( h1.filterLine != h2.filterLine ) {
    cout << "filterLine\t" << h1.filterLine << "\t" << h2.filterLine << endl;
  }
}


void comparePeaks( int n , MyScanHeaderStruct &h1 , MyScanHeaderStruct &h2 , MzxmlReader &parser1 , MzxmlReader &parser2 ) {
  if( h1.peaksCount != h2.peaksCount ) {
    cout << "\nPeaks count for scan " << n << " differ" << endl;

    parser1.printPeaks( n );
    cout << "---" << endl;
    parser2.printPeaks( n );
  }
  else {
    RAMPREAL *pPeaks1, *pPeaks2;
    pPeaks1 = parser1.getPeaks( n );
    pPeaks2 = parser2.getPeaks( n );
    bool isFirst = true;
    for( int peakPos = 0 ; peakPos < h1.peaksCount * 2 ; peakPos+=2 ){
      if( (pPeaks1[peakPos] - pPeaks2[peakPos]) > MZ_TOL || (pPeaks1[peakPos+1] != pPeaks2[peakPos+1]) ) {
	if( isFirst ) {
	  cout << "\nPeaks for scan " << n << " differ" << endl;		  
	  isFirst = false;
	}
	cout << (double) pPeaks1[peakPos] << " - " << pPeaks1[peakPos+1]
	     << "\t" << (double) pPeaks2[peakPos] << " - " << pPeaks2[peakPos+1]
	     << "\t(" << ( pPeaks1[peakPos] - pPeaks2[peakPos] ) << " - " << (pPeaks1[peakPos+1] - pPeaks2[peakPos+1]) << ")" <<endl;
      }
    }
  }
}
