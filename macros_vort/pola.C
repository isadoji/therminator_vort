{
double func( double x){
double sigma = 0.6;
double cte0 = TMath::Power(sigma*TMath::Sqrt(2*TMath::Pi()),-1);
double cte1= TMath::Power(-2*sigma*sigma,-1);
return cte0*TMath::Exp(cte1*TMath::Power(x,2));
}
float PSX,PSY,PSZ,MX,MY,MZ,Sx,Sy,Sz,St,M,EVE,A,Ene,masa;
TFile *hfile = hfile = TFile::Open("pola.root","RECREATE");
TTree * pol = new TTree("pol","polarizacion de lambdas");
auto massa = pol ->Branch("m", &masa, "m/F");
auto equis = pol->Branch("x", &PSX, "x/F");
auto ye = pol->Branch("y", &PSY, "y/F");
auto zeta = pol->Branch("z", &PSZ, "z/F");
auto ener = pol->Branch("e", &Ene, "e/F");
auto emex = pol->Branch("px", &MX, "px/F");
auto emey = pol->Branch("py", &MY, "py/F");
auto emez = pol->Branch("pz", &MZ, "pz/F");
auto Eset = pol->Branch("St", &St, "St/F");
auto Esex = pol->Branch("Sx", &Sx, "Sx/F");
auto Esey = pol->Branch("Sy", &Sy, "Sy/F");
auto Esez = pol->Branch("Sz", &Sz, "Sz/F");
auto Eben = pol->Branch("event", &EVE, "event/F");
auto anti = pol->Branch("anti", &A, "anti/F");

char archivo [50];
char cadena [50];
char cadenaE [50];
for(Int_t nf1=0; nf1<3;nf1++){
sprintf(archivo, "event%d.root", nf1);
sprintf(cadena, "mychain%d", nf1);
sprintf(cadenaE, "mychainE%d", nf1);

TChain cadena("particles");
cadena.Add(archivo);
 //archivo a leer
struct particula_t
{
	Float_t px,py,pz,e,x,y,z,t,pid,mass,fatherpid,rootpid,event;
} PARTICLE;

particula_t particle;
cadena.SetBranchAddress("part",&particle);
Int_t nevent = cadena.GetEntries();

//segunda estructura
TChain cadenaE("particles");
cadenaE.Add(archivo);
struct particula2_t
{
	Float_t px,py,pz,e,x,y,z,t,pid,mass,fatherpid,rootpid,event;
} PARTICLE2;
particula2_t particle2;
cadenaE.SetBranchAddress("part",&particle2);
Int_t nevent2 = cadenaE.GetEntries();

//valores de entrada
double BWV=0.484;
double Tau=8.075;
double KTwoPi3=2*TMath::Pi();
double T=0.1187;
double Mu= -0.0009 + 0.0069 + 0.0285;
float Ka=0.1973269804;
float paso=0.01;
float gama = 1.0/sqrt(1-BWV*BWV);

Int_t i,j,k;
float pose, posx, posy, posz,vx, vy, vz,momx,momy,momz,X,Y,Z,PX,PY,PZ,pt,rho,mas,tMt,eta,rap,Phis,Phip; //propiedades de las partículas
float Vx_0,Vx_y,Vx_z; //velocidades en x del fluido
float Vy_0,Vy_x,Vy_z; //velocidades en y del fluido
float Vz_0,Vz_x,Vz_y; //velocidades en z del fluido
float Wxy,Wyz,Wzx; //entradas del tensor de vorticidad
auto OmmeX =pol->Branch("Wx", &Wyz, "Wx/F");
auto OmmeY =pol->Branch("Wy", &Wzx, "Wy/F");
auto OmmeZ =pol->Branch("Wz", &Wxy, "Wz/F");
float sm,smX,smY,smZ,smearing,smearingX,smearingY,smearingZ; //parametros de smearing

for (Int_t i=0;i<nevent;i++)
{
cadena.GetEvent(i);
if((particle.pid==3122&&particle.fatherpid==3122)||(particle.pid==-3122&&particle.fatherpid==-3122)){
X=particle.x;
Y=particle.y;
Z=particle.z;
PX=particle.px;
PY=particle.py;
PZ=particle.pz;
mas=particle.mass;
masa=particle.mass;
pt=TMath::Hypot(particle.px,particle.py);
rho=TMath::Hypot(particle.x,particle.y);
eta = 0.5*TMath::Log((particle.t+particle.pz)/(particle.e-particle.pz));
rap = 0.5*TMath::Log((particle.e+particle.pz)/(particle.e-particle.pz));
Phis = TMath::ATan(particle.y/particle.x); 
Phip = TMath::ATan(particle.py/particle.px);  
tMt  = TMath::Hypot(mas, pt);
Ene=particle.e;
EVE=particle.event;

//calculo de omegaXZ
      float velsmearingX = 0, velsmearingX_0=0,velsmearingX_Y=0, velsmearingX_Z=0;
      float velsmearingY = 0, velsmearingY_0=0,velsmearingY_X=0, velsmearingY_Z=0;
      float velsmearingZ = 0, velsmearingZ_0=0,velsmearingZ_X=0, velsmearingZ_Y=0;
      float velsmearing = 0;
      
for (j=0;j<nevent;j++)
{
 cadenaE.GetEvent(j);
if(particle.pid==particle.fatherpid){

pose= particle2.e;
posx= particle2.x;
posy= particle2.y;
posz= particle2.z;
momx= particle2.px;
momy= particle2.py;
momz= particle2.pz;
vx = -momx/pose;
vy = -momy/pose;
vz = -momz/pose;

sm = ((X-posx)*(X-posx))+(Y-posy)*(Y-posy)+((Z-posz)*(Z-posz));
smY = ((X-posx)*(X-posx))+(Y-posy+paso)*(Y-posy+paso)+((Z-posz)*(Z-posz));
smX = ((X-posx+paso)*(X-posx+paso))+(Y-posy)*(Y-posy)+((Z-posz)*(Z-posz));
smZ = ((X-posx)*(X-posx))+(Y-posy)*(Y-posy)+((Z+paso-posz)*(Z+paso-posz));

smearing = func(sm);
smearingY = func(smY);
smearingX = func(smX);
smearingZ = func(smZ);

//suma sobre todas las partículas
velsmearing = velsmearing + smearing;
velsmearingX = velsmearingX + smearingX;
velsmearingY = velsmearingY + smearingY;
velsmearingZ = velsmearingZ + smearingZ;

velsmearingX_0 = velsmearingX_0 + vx*smearing;
velsmearingY_0 = velsmearingY_0 + vy*smearing;
velsmearingZ_0 = velsmearingZ_0 + vz*smearing;

velsmearingX_Y = velsmearingX_Y + vx*smearingY;
velsmearingX_Z = velsmearingX_Z + vx*smearingZ;
velsmearingY_X = velsmearingY_X + vy*smearingX;
velsmearingY_Z = velsmearingY_Z + vy*smearingZ;
velsmearingZ_X = velsmearingZ_X + vz*smearingX;
velsmearingZ_Y = velsmearingZ_Y + vz*smearingY;

}}
cout << i << "/" << nevent  << " | Evento: "<< EVE << endl;

//calculo de las derivadas
   Vx_z=-((velsmearingX_0/velsmearing)-(velsmearingX_Z/velsmearingZ))/paso;
   Vx_y=-((velsmearingX_0/velsmearing)-(velsmearingX_Y/velsmearingY))/paso;
   Vy_x=-((velsmearingY_0/velsmearing)-(velsmearingY_X/velsmearingX))/paso;
   Vy_z=-((velsmearingY_0/velsmearing)-(velsmearingY_Z/velsmearingZ))/paso;
   Vz_x=-((velsmearingZ_0/velsmearing)-(velsmearingZ_X/velsmearingX))/paso;
   Vz_y=-((velsmearingZ_0/velsmearing)-(velsmearingZ_Y/velsmearingY))/paso;
   //componentes de la vorticidad
   Wxy=Ka*gama*(Vy_x - Vx_y)/(2*T);
   Wyz=Ka*gama*(Vz_y - Vy_z)/(2*T);
   Wzx=Ka*gama*(Vz_x - Vx_z)/(2*T);

   //vector de espin
 
   
if(particle.pid==3122&&particle.fatherpid==3122){
  PSX=X;
  PSY=Y;
  PSZ=Z;
  MX=PX;
  MY=PY;
  MZ=PZ;
  St=-(1/(4*mas))*(PX*Wyz + PY*Wzx + PZ*Wxy);
  Sx=(1/(4*mas))*Ene*Wyz;
  Sy=(1/(4*mas))*Ene*Wzx;
  Sz=(1/(4*mas))*Ene*Wxy;
  A=1;
  
  }
  else if(particle.pid==-3122&&particle.fatherpid==-3122){
  PSX=X;
  PSY=Y;
  PSZ=Z;
  MX=PX;
  MY=PY;
  MZ=PZ;
  St=-(1/(4*mas))*(PX*Wyz + PY*Wzx + PZ*Wxy);
  Sx=(1/(4*mas))*Ene*Wyz;
  Sy=(1/(4*mas))*Ene*Wzx;
  Sz=(1/(4*mas))*Ene*Wxy;
  A=-1;
  }
  
  else{ 
  PSX=0;
  PSY=0;
  PSZ=0;
  MX=0;
  MY=0;
  MZ=0;
  St=0;	 
  Sy=0;
  Sx=0;
  Sz=0;
  A=0;}
  //cout  << "Sy=" << Sy << endl;
pol->Fill();
} 
}
}
pol->Write();
hfile->Close(); 

}
