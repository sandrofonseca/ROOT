// @(#)root/tmva $Id$
// Author: Attila Krasznahorkay

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : MsgLogger                                                             *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Implementation (see header for description)                               *
 *                                                                                *
 * Author:                                                                        *
 *      Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch> - CERN, Switzerland   *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         * 
 *      MPI-K Heidelberg, Germany                                                 * 
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

// Local include(s):
#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"

// STL include(s):
#include <iomanip>

#include <cstdlib>

// ROOT include(s):

ClassImp(TMVA::MsgLogger)

// this is the hard-coded maximum length of the source names
UInt_t TMVA::MsgLogger::fgMaxSourceSize = 25;
Bool_t TMVA::MsgLogger::fgInhibitOutput = kFALSE;

const std::string TMVA::MsgLogger::fgPrefix="--- ";     
const std::string TMVA::MsgLogger::fgSuffix=": ";
std::map<TMVA::EMsgType, std::string> TMVA::MsgLogger::fgTypeMap=std::map<TMVA::EMsgType, std::string>();
std::map<TMVA::EMsgType, std::string> TMVA::MsgLogger::fgColorMap=std::map<TMVA::EMsgType, std::string>();

void   TMVA::MsgLogger::InhibitOutput() { fgInhibitOutput = kTRUE;  }
void   TMVA::MsgLogger::EnableOutput()  { fgInhibitOutput = kFALSE; }
//_______________________________________________________________________
TMVA::MsgLogger::MsgLogger( const TObject* source, EMsgType minType )
   : fObjSource ( source ), 
     fStrSource ( "" ), 
     fActiveType( kINFO ), 
     fMinType   ( minType )
{
   // constructor
}

//_______________________________________________________________________
TMVA::MsgLogger::MsgLogger( const std::string& source, EMsgType minType )
   : fObjSource ( 0 ),
     fStrSource ( source ), 
     fActiveType( kINFO ), 
     fMinType   ( minType )
{
   // constructor
}

//_______________________________________________________________________
TMVA::MsgLogger::MsgLogger( EMsgType minType )
   : fObjSource ( 0 ), 
     fStrSource ( "Unknown" ), 
     fActiveType( kINFO ), 
     fMinType   ( minType )
{
   // constructor
}

//_______________________________________________________________________
TMVA::MsgLogger::MsgLogger( const MsgLogger& parent )
   : std::basic_ios<MsgLogger::char_type, MsgLogger::traits_type>(),
     std::ostringstream(),
     TObject()
{
   // copy constructor
   *this = parent;
}

//_______________________________________________________________________
TMVA::MsgLogger::~MsgLogger() 
{
   // destructor
}

//_______________________________________________________________________
TMVA::MsgLogger& TMVA::MsgLogger::operator= ( const MsgLogger& parent ) 
{
   // assingment operator
   if (&parent != this) {
      fObjSource  = parent.fObjSource;
      fStrSource  = parent.fStrSource;
      fActiveType = parent.fActiveType;
      fMinType    = parent.fMinType;
   }

   return *this;
}

//_______________________________________________________________________
std::string TMVA::MsgLogger::GetFormattedSource() const
{
   // make sure the source name is no longer than fgMaxSourceSize:
   std::string source_name;
   if (fObjSource) source_name = fObjSource->GetName();
   else            source_name = fStrSource;

   if (source_name.size() > fgMaxSourceSize) {
      source_name = source_name.substr( 0, fgMaxSourceSize - 3 );
      source_name += "...";
   }
   
   return source_name;
}

//_______________________________________________________________________
std::string TMVA::MsgLogger::GetPrintedSource() const
{ 
   // the full logger prefix
   std::string source_name = GetFormattedSource();
   if (source_name.size() < fgMaxSourceSize) 
      for (std::string::size_type i=source_name.size(); i<fgMaxSourceSize; i++) source_name.push_back( ' ' );

   return fgPrefix + source_name + fgSuffix; 
}

//_______________________________________________________________________
void TMVA::MsgLogger::Send() 
{
   // activates the logger writer

   // make sure the source name is no longer than fgMaxSourceSize:
   std::string source_name = GetFormattedSource();

   std::string message = this->str();
   std::string::size_type previous_pos = 0, current_pos = 0;

   // slice the message into lines:
   while (kTRUE) {
      current_pos = message.find( '\n', previous_pos );
      std::string line = message.substr( previous_pos, current_pos - previous_pos );

      std::ostringstream message_to_send;
      // must call the modifiers like this, otherwise g++ get's confused with the operators...
      message_to_send.setf( std::ios::adjustfield, std::ios::left );
      message_to_send.width( fgMaxSourceSize );
      message_to_send << source_name << fgSuffix << line;
      this->WriteMsg( fActiveType, message_to_send.str() );

      if (current_pos == message.npos) break;
      previous_pos = current_pos + 1;
   }

   // reset the stream buffer:
   this->str( "" );
   fActiveType = kINFO; // To always print messages that have no level specified...
   return;
}

//_______________________________________________________________________
void TMVA::MsgLogger::WriteMsg( EMsgType type, const std::string& line ) const 
{
   // putting the output string, the message type, and the color
   // switcher together into a single string

   if (type < fMinType || (fgInhibitOutput && type!=kFATAL)) return; // no output

   InitMaps();
   std::map<EMsgType, std::string>::const_iterator stype;
   if ((stype = fgTypeMap.find( type )) == fgTypeMap.end()) return;
   if (!gConfig().IsSilent() || type==kFATAL) {
      if (gConfig().UseColor()) {
         // no text for INFO or VERBOSE
         if (type == kINFO || type == kVERBOSE) 
            std::cout << fgPrefix << line << std::endl; // no color for info
         else
            std::cout << fgColorMap.find( type )->second << fgPrefix << "<"
                      << stype->second << "> " << line  << "\033[0m" << std::endl;
      } 
      else {
         if (type == kINFO) std::cout << fgPrefix << line << std::endl;
         else               std::cout << fgPrefix << "<" << stype->second << "> " << line << std::endl;
      }
   }
   // take decision to stop if fatal error
   if (type == kFATAL) { 
      std::cout << "***> abort program execution" << std::endl;
      std::exit(1);
   }
}

//_______________________________________________________________________
TMVA::MsgLogger& TMVA::MsgLogger::Endmsg( MsgLogger& logger ) 
{
   // end line
   logger.Send();
   return logger;
}

//_______________________________________________________________________
void TMVA::MsgLogger::InitMaps()
{
   if (fgTypeMap.size()>0 && fgColorMap.size()>0 ) return;

   // fill maps that assign a string and a color to echo message level
   fgTypeMap[kVERBOSE]  = std::string("VERBOSE");
   fgTypeMap[kDEBUG]    = std::string("DEBUG");
   fgTypeMap[kINFO]     = std::string("INFO");
   fgTypeMap[kWARNING]  = std::string("WARNING");
   fgTypeMap[kERROR]    = std::string("ERROR");
   fgTypeMap[kFATAL]    = std::string("FATAL");
   fgTypeMap[kSILENT]   = std::string("SILENT");

   fgColorMap[kVERBOSE] = std::string("");
   fgColorMap[kDEBUG]   = std::string("\033[34m");
   fgColorMap[kINFO]    = std::string("");
   fgColorMap[kWARNING] = std::string("\033[1;31m");
   fgColorMap[kERROR]   = std::string("\033[31m");
   fgColorMap[kFATAL]   = std::string("\033[37;41;1m");
   fgColorMap[kSILENT]  = std::string("\033[30m");
}
