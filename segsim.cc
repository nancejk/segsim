#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIsession.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4TransportationManager.hh"

#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4GDMLParser.hh"

int main(int argc,char **argv)
{
	G4GDMLParser parser;
   parser.Read("geo.gdml");

   G4RunManager* runManager = new G4RunManager;

   runManager->SetUserInitialization(new DetectorConstruction(
                                     parser.GetWorldVolume()));
   runManager->SetUserInitialization(new PhysicsList);
   runManager->SetUserAction(new PrimaryGeneratorAction);

   runManager->Initialize();

   G4UImanager* UI = G4UImanager::GetUIpointer();
 
   ///////////////////////////////////////////////////////////////////////
   //
   // Example how to retrieve Auxiliary Information
   //
   const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
   std::vector<G4LogicalVolume*>::const_iterator lvciter;
   for( lvciter = lvs->begin(); lvciter != lvs->end(); lvciter++ )
   {
     G4GDMLAuxListType auxInfo = parser.GetVolumeAuxiliaryInformation(*lvciter);
     std::vector<G4GDMLAuxPairType>::const_iterator ipair = auxInfo.begin();
     for( ipair = auxInfo.begin(); ipair != auxInfo.end(); ipair++ )
     {
       G4String str=ipair->type;
       G4cout << " Auxiliary Information is found for Logical Volume :  "
              << (*lvciter)->GetName() << G4endl;
       G4cout << " Name of Auxiliary type is  =  " << str << G4endl;
     }
   }
   //
   // End of Auxiliary Information block
   //
   ////////////////////////////////////////////////////////////////////////

#ifdef G4VIS_USE
   G4VisManager* visManager = new G4VisExecutive;
   visManager->Initialize();
#endif

   G4UIsession * session = 0;
#ifdef G4UI_USE_TCSH
   session = new G4UIterminal(new G4UItcsh);
#else
   session = new G4UIterminal();
#endif
#ifdef G4VIS_USE
   UI->ApplyCommand("/control/execute vis.mac"); 
#endif
   session->SessionStart();
   delete session;

#ifdef G4VIS_USE
   delete visManager;
#endif

   delete runManager;

   return 0;
}
