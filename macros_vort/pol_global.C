{
TChain mychain("pol");
mychain.Add("ExE/Nica1.root"); //archivo a leer 500*20
Float_t px,py,pz,x,y,z,St,Sx,Sy,Sz,Wx,Wy,Wz,anti,e,m;
mychain.SetBranchAddress("px",&px);
mychain.SetBranchAddress("py",&py);
mychain.SetBranchAddress("pz",&pz);
mychain.SetBranchAddress("St",&Sx);
mychain.SetBranchAddress("Sx",&Sx);
mychain.SetBranchAddress("Sy",&Sy);
mychain.SetBranchAddress("Sz",&Sz);
mychain.SetBranchAddress("Wx",&Wx);
mychain.SetBranchAddress("Wy",&Wy);
mychain.SetBranchAddress("Wz",&Wz);
mychain.SetBranchAddress("e",&e);
mychain.SetBranchAddress("m",&m);
mychain.SetBranchAddress("anti", &anti);
Int_t nevent = mychain.GetEntries();
Int_t i;
double X=0,Y=0,Z=0,X2=0,Y2=0,Z2=0;
double xx,xx2,yy,yy2,zz,zz2,Norm,pxx,pyy,pzz,Sxx,Syy,Szz,Jx,Jy,Jz,Jxx,Jyy,Jzz;
double hb = 2.506791506E20;
double eventos=1500;
for (i=0;i<nevent;i++)
{
mychain.GetEvent(i);
Norm=e*(e+m);
pxx=((St*e)-(Sx*px+Sy*py+Sz*pz))*px/Norm;
pyy=((St*e)-(Sx*px+Sy*py+Sz*pz))*py/Norm;
pyy=((St*e)-(Sx*px+Sy*py+Sz*pz))*pz/Norm;
Jx=0;
Jy=1;
Jz=0;

if (anti==1){
Sxx=Sx-pxx;
Syy=Sy-pyy;
Szz=Sz-pzz;
xx=Sxx/sqrt(Sxx*Sxx+Syy*Syy+Szz*Szz);
yy=Syy/sqrt(Sxx*Sxx+Syy*Syy+Szz*Szz);
zz=Szz/sqrt(Sxx*Sxx+Syy*Syy+Szz*Szz);
xx=xx*Jx;
yy=yy*Jy;
zz=zz*(Jz);
X=yy+X;
}

else{
Sxx=Sx-pxx;
Syy=Sy-pyy;
Szz=Sz-pzz;
xx2=Sxx/sqrt(Sxx*Sxx+Syy*Syy+Szz*Szz);
yy2=Syy/sqrt(Sxx*Sxx+Syy*Syy+Szz*Szz);
zz2=Szz/sqrt(Sxx*Sxx+Syy*Syy+Szz*Szz);
xx2=xx2*(Jx);
yy2=yy2*(Jy);
zz2=zz2*(Jz);
X2=xx2+yy2+zz2+X2;
}
}

cout << "Polarizacion total para #Lambda" << endl;
cout << "P_H	" << X/eventos  << endl;

cout << "	" << endl;
cout << "Polarizacion total para #bar{#Lambda}" << endl;
cout << "P_H	" << X2/eventos  << endl;
cout << nevent << endl;
   

}
