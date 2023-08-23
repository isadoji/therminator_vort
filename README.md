# Modified blast-wave model with thermal vorticity

$\textbf{José Jorge Medina Serna, Isabel Domínguez Jiménez  and MariaElena Tejeda-Yeomans}$
  
THERMINATOR [1] is a Monte Carlo event generator designed to study particle production in relativistic heavy-ion collisions. To include the effects of vorticity, it is necessary to modify the following files (therminator_mod)

1. thermitator.in
2. therm_events.cxx
3. Integrator.cxx
4. Particles.cxx


# 1. therminator.in

The therminator.in file contains all the input parameters needed to generate the events. The modification will be to add an extra parameter, which will correspond to $\zeta$. This can be done with the following lines of code anywhere in the file:

L66 
```
#vorticity parameter
Zeta = 0
```

# 2. therm_events.cxx

The therm_events.cxx file reads the input parameters from the therminator.in file. The modification will be made so that the recently added extra parameter Ozeta is read. 

L43 
```
double mZeta;
double zRhoMax;
```
L91:
```
if ((sModel == 0) || (sModel == 2)){
try {
mZeta = atof(sRPInstance->getPar("Zeta").Data());
}
catch (STR tError) {
mZeta = 0.0;
}
}
if ((sModel == 0) || (sModel == 2)){
try {
zRhoMax = atof(sRPInstance->getPar"RhoMax").Data());
}
catch (STR tError) {
zRhoMax = 0.0;
}
}
```
# 3. Integrator.cxx

Integrator.cxx file performs all the Monte Carlo computations of the Cooper-Frye integral, so you have to declare the external variable omega

L37 
```
extern double mZeta;
```

# 4. Particle.cxx

Particle.cxx file saves the particles of each event into an output file. The modification will be made in the momentum of generated particles, where a small "smearing" will be responsible for generating vorticity 


L37 
```
extern double mZeta;
extern double zRhoMax;
```

L54 
```
pz=tMt*TMath::Sinh(aRapidity) + mZeta*GetMass()*aRho*TMath::Cos(aPhis)*TMath::Exp(-aRho*aRho/(2*zRhoMax*zRhoMax))/zRhoMax;
```

# Compilation
Once the modifications described above have been made, therminator must be compiled. You need a C++ compiler and the ROOT [2] libraries for that. The make command is executed in the directory where therminator is modified.

# Test (macros_vort)
Already modified and compiled, one can control the intensity of the final vorticity of the system by changing the value of the input parameter $\zeta. $\zeta$=0 value corresponds to the original model without modification. Still, if you want to verify that non-zero values ​​create an entry and exit of particles along the z-axis, you can perform a simple run with $\zeta$=1. This is done by changing the value of Zeta in the therminator.in file and running therm\_events and therm\_tree.

# Bibliography
[1] THERMINATOR 2: THERMal heavy IoN generATOR 2.

Mikolaj Chojnacki, Adam Kisiel, Wojciech Florkowski and Wojciech Broniowski

Computer Physics Communications.

Elsevier, 183, 3, (746--773) 2012

https://www.ifj.edu.pl/dept/no4/nz41/programs/therminator/

[2] ROOT - An Object Oriented Data Analysis Framework,

Rene Brun and Fons Rademakers, 

Proceedings AIHENP'96 Workshop, Lausanne, Sep. 1996,

Nucl. Inst. & Meth. in Phys. Res. A 389 (1997) 81-86.

https://root.cern/

