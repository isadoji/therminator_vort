/******************************************************************************
 *                      T H E R M I N A T O R                                 *
 *                   THERMal heavy-IoN generATOR                              *
 *                           version 1.0                                      *
 *                                                                            *
 * Authors of the model: Wojciech Broniowski, Wojciech.Broniowski@ifj.edu.pl, *
 *                       Wojciech Florkowski, Wojciech.Florkowski@ifj.edu.pl  *
 * Authors of the code:  Adam Kisiel, kisiel@if.pw.edu.pl                     *
 *                       Tomasz Taluc, ttaluc@if.pw.edu.pl                    *
 * Code designers: Adam Kisiel, Tomasz Taluc, Wojciech Broniowski,            *
 *                 Wojciech Florkowski                                        *
 *                                                                            *
 * For the detailed description of the program and furhter references         * 
 * to the description of the model plesase refer to: nucl-th/0504047,         *
 * accessibile at: http://www.arxiv.org/nucl-th/0504047                       *
 *                                                                            *
 * Homepage: http://hirg.if.pw.edu.pl/en/therminator/                         *
 *                                                                            *
 * This code can be freely used and redistributed. However if you decide to   *
 * make modifications to the code, please contact the authors, especially     *
 * if you plan to publish the results obtained with such modified code.       *
 * Any publication of results obtained using this code must include the       *
 * reference to nucl-th/0504047 and the published version of it, when         *
 * available.                                                                 *
 *                                                                            *
 *****************************************************************************/
#include <map>
#include "THGlobal.h"
#include "ReadPar.h"
#include "Parser.h"
#include "ParticleDB.h"
#include "ParticleType.h"
#include "DecayTable.h"
#include "Event.h"

ReadPar *sRPInstance;
STR      sRPFileName;
int      sNumEvents;
int      sRunType; 
int      sTables;
int      sModel;
int      sIntegrateSample;
double mOmega;
double zRhoMax;
void ReadParameters()
{
  STR tModel;
  STR tTable;
  
  try {
    sNumEvents = atoi(sRPInstance->getPar("NumberOfEvents").Data());
    tModel = sRPInstance->getPar("FreezeOutModel");
    if (tModel.Contains("SingleFreezeOut"))
      sModel = 0;
    else if (tModel.Contains("BlastWaveVTDelay"))
      sModel = 6;
    else if (tModel.Contains("BlastWaveVT"))
      sModel = 2;
    else if (tModel.Contains("BlastWaveVLinearFormation"))
      sModel = 7;
    else if (tModel.Contains("BlastWaveVLinearDelay"))
      sModel = 8;
    else if (tModel.Contains("BlastWaveVLinear"))
      sModel = 4;
    else if (tModel.Contains("FiniteHydro"))
      sModel = 5;
    else {
      PRINT_MESSAGE("Unknown model type: " << tModel.Data());
      PRINT_MESSAGE("Please provide the proper name");
      exit(0);
    }
    if (atoi(sRPInstance->getPar("Randomize").Data()) == 1)
      sRunType = 4;
    else
      sRunType = 3;
    tTable = sRPInstance->getPar("TableType");
    if (tTable.Contains("Mathematica"))
      sTables = 0;
    else if (tTable.Contains("SHARE"))
      sTables = 1;
    else {
      PRINT_MESSAGE("Unknown table type: " << tTable.Data());
      exit(0);
    }
    sIntegrateSample = atoi(sRPInstance->getPar("NumberOfIntegrateSamples").Data());
  }
  catch (STR tError) {
    PRINT_DEBUG_1("RunBFPW::ReadParameters - Caught exception " << tError);
    PRINT_MESSAGE("Did not find one of the neccessary parameters in the parameters file.");
    exit(0);
   if ((sModel == 0) || (sModel == 2)) {
    try {
      mOmega  = atof(sRPInstance->getPar("Omega").Data());
    }
    catch (STR tError) {
      // Using default value of 0
      mOmega = 0.0;
    }
  }
      if ((sModel == 0) || (sModel == 2)) {
    try {
      zRhoMax  = atof(sRPInstance->getPar("RhoMax").Data());
    }
    catch (STR tError) {
      // Using default value of 0
      zRhoMax = 0.0;
    }
  }
  }
}

int main(int argc, char **argv)
{
  ParticleDB   *tPartDB = new ParticleDB();
  Parser       *tParser;
  ParticleType *tType;
  int           k;
  
  try {
    if (argc > 1)
      sRPFileName = argv[1];
    else
      sRPFileName = "therminator.in";
    sRPInstance = new ReadPar(sRPFileName.Data());
  }
  catch (STR tError) {
    PRINT_DEBUG_1("Caught: " << tError);
    PRINT_MESSAGE("No Par File. Generating the stub...");
    exit(3);
  }

  ReadParameters();
  
  tParser = new Parser(tPartDB);

   if (sTables)
     {  
       PRINT_MESSAGE("Particle data tables from SHARE");
       tParser->ReadShare();
     }
   else
     {
       PRINT_MESSAGE("Particle data tables from old Mathematica tables");
       tParser->ReadInput();
     }
  
  PRINT_MESSAGE(tPartDB->GetParticleTypeCount()<<" particle types read.");
  
  for(int tPart=0;tPart<tPartDB->GetParticleTypeCount();tPart++)
    {
      tType = tPartDB->GetParticleType(tPart);
      PRINT_DEBUG_2("Particle number " << tType->GetNumber() << " " << tType->GetName() 
		    <<" mass "<<tType->GetMass()
		    <<" gamma "<<tType->GetGamma()
		    <<" spin "<<tType->GetSpin()
		    <<" I "<<tType->GetI()
		    <<" I3 "<<tType->GetI3()
		    <<" barionN "<<tType->GetBarionN()
		    <<" charge "<<tType->GetCharge()
		    <<" mc# "<<tType->GetPDGCode());
      
      double sumaBR=0.;
      
      if (tType->GetTable()) 
	{
	  for (int tChanIndex=0; tChanIndex<((DecayTable *) (tType->GetTable()))->GetChannelCount()+1; tChanIndex++)
	    {
	      PRINT_DEBUG_2("Channel " << tChanIndex << " " 
			    << tType->GetTable()->GetDecayChannel(tChanIndex)->GetParticle1() << " " 
			    << (tPartDB->GetParticleType(tType->GetTable()->GetDecayChannel(tChanIndex)->GetParticle1()))->GetName() << " " 
			    << tType->GetTable()->GetDecayChannel(tChanIndex)->GetParticle2() << " " 
			    << (tPartDB->GetParticleType(tType->GetTable()->GetDecayChannel(tChanIndex)->GetParticle2()))->GetName() << " ");
	      if (tType->GetTable()->GetDecayChannel(tChanIndex)->Is3Particle())
		PRINT_DEBUG_2(tType->GetTable()->GetDecayChannel(tChanIndex)->GetParticle3() << " " 
			      << (tPartDB->GetParticleType(tType->GetTable()->GetDecayChannel(tChanIndex)->GetParticle3()))->GetName() << " ");
	      
	      PRINT_DEBUG_2(tType->GetTable()->GetDecayChannel(tChanIndex)->GetBranchingRatio() << " " 
			    << tType->GetTable()->GetDecayStep(tChanIndex));
	      sumaBR+=tType->GetTable()->GetDecayChannel(tChanIndex)->GetBranchingRatio(); 
	    }
	  PRINT_DEBUG_2("sumaBR "<<sumaBR);
	}
    }
  
  Integrator calka(sIntegrateSample);
  calka.ReadMultInteg(tPartDB);
  
  if ((sRunType == 3) || (sRunType == 4))
    {
      if (sRunType==4)
	calka.Randomize();

      Event *ev1 = new Event(tPartDB, &calka);
      if (sRunType == 4) ev1->Randomize();
      
      for (int tIter=0; tIter<sNumEvents; tIter++)
	{
	  if (sRunType == 4) {
	    ev1->GenerateEvent();
	  }
	  else {
	    ev1->GenerateEvent(43212-tIter*2);
	  }
	  ev1->DecayParticles();
	  ev1->WriteEvent(tIter);
	  //	  PRINT_MESSAGE("Event: "<<tIter+1);
	  cout << "\rEvent: " << tIter+1 << "     ";
	  cout.flush();
	}
    }
  
  cout << endl;
  return 0;
}
