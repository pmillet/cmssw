/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Authors:
 *	Jan Kašpar (jan.kaspar@gmail.com)
 *
 ****************************************************************************/

#include "DataFormats/CTPPSAlignment/interface/RPAlignmentCorrectionsData.h"

#include "FWCore/Utilities/interface/typelookup.h"

#include "DataFormats/CTPPSAlignment/interface/RPAlignmentCorrectionsData.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"

#include <set>

//----------------------------------------------------------------------------------------------------

RPAlignmentCorrectionData&
RPAlignmentCorrectionsData::GetRPCorrection( unsigned int id )
{
  return rps[id];
}

//----------------------------------------------------------------------------------------------------

RPAlignmentCorrectionData
RPAlignmentCorrectionsData::GetRPCorrection( unsigned int id ) const
{
  RPAlignmentCorrectionData align_corr;
  auto it = rps.find( id );
  if ( it != rps.end() )
    align_corr = it->second;
  return align_corr;
} 

//----------------------------------------------------------------------------------------------------

RPAlignmentCorrectionData&
RPAlignmentCorrectionsData::GetSensorCorrection( unsigned int id )
{
  return sensors[id];
}

//----------------------------------------------------------------------------------------------------

RPAlignmentCorrectionData
RPAlignmentCorrectionsData::GetSensorCorrection( unsigned int id ) const
{
  RPAlignmentCorrectionData align_corr;
  auto it = sensors.find( id );
  if ( it != sensors.end() )
    align_corr = it->second;
  return align_corr;
}

//----------------------------------------------------------------------------------------------------

RPAlignmentCorrectionData
RPAlignmentCorrectionsData::GetFullSensorCorrection( unsigned int id, bool useRPErrors ) const
{
  RPAlignmentCorrectionData align_corr;

  // try to get alignment correction of the full RP
  auto rpIt = rps.find( CTPPSDetId( id ).getRPId() );
  if ( rpIt != rps.end() )
    align_corr = rpIt->second;

  // try to get sensor alignment correction
  auto sIt = sensors.find( id );

  // merge the corrections
  if ( sIt != sensors.end() )
    align_corr.add(sIt->second, useRPErrors);

  return align_corr;
}

//----------------------------------------------------------------------------------------------------

void
RPAlignmentCorrectionsData::SetRPCorrection( unsigned int id, const RPAlignmentCorrectionData& ac )
{
  rps[id] = ac;
}

//----------------------------------------------------------------------------------------------------

void
RPAlignmentCorrectionsData::SetSensorCorrection( unsigned int id, const RPAlignmentCorrectionData& ac )
{
  sensors[id] = ac;
}

//----------------------------------------------------------------------------------------------------

void
RPAlignmentCorrectionsData::AddRPCorrection( unsigned int id, const RPAlignmentCorrectionData &a, bool sumErrors, bool addShR, bool addShZ, bool addRotZ )
{
  auto it = rps.find( id );
  if ( it == rps.end() )
    rps.insert( mapType::value_type( id, a ) );
  else
    it->second.add( a, sumErrors, addShR, addShZ, addRotZ );
}

//----------------------------------------------------------------------------------------------------

void
RPAlignmentCorrectionsData::AddSensorCorrection( unsigned int id, const RPAlignmentCorrectionData &a, bool sumErrors, bool addShR, bool addShZ, bool addRotZ )
{
  auto it = sensors.find( id );
  if ( it == sensors.end() )
    sensors.insert( mapType::value_type( id, a ) );
  else
    it->second.add( a, sumErrors, addShR, addShZ, addRotZ );
}

//----------------------------------------------------------------------------------------------------

void
RPAlignmentCorrectionsData::AddCorrections( const RPAlignmentCorrectionsData &nac, bool sumErrors, bool addShR, bool addShZ, bool addRotZ )
{
  for ( const auto& it : nac.rps )
    AddRPCorrection( it.first, it.second, sumErrors, addShR, addShZ, addRotZ );

  for ( const auto& it : nac.sensors )
    AddSensorCorrection( it.first, it.second, sumErrors, addShR, addShZ, addRotZ );
}

//----------------------------------------------------------------------------------------------------

void
RPAlignmentCorrectionsData::Clear()
{
  rps.clear();
  sensors.clear();
}

//----------------------------------------------------------------------------------------------------

TYPELOOKUP_DATA_REG( RPAlignmentCorrectionsData );
