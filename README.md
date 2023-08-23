# therminator_vort
https://www.ifj.edu.pl/dept/no4/nz41/programs/therminator/

Para realizar la modificación necesaria para que Therminator incluya efectos de vorticidad, hay que agregar ciertas lineas de código en los siguientes archivos.
   
    thermitator.in
    therm_events.cxx
    Integrator.cxx
    Particles.cxx


# Modificación de therminator.in
El archivo therminator.in contiene todos los parámetros de entrada necesarios para la generación de los eventos. La modificación a realizar será agregar un parámetro extra, que corresponderá a omega. Esto se puede hacer con las siguientes lineas de código en cualquier lugar del archivo:
#parametro de vorticidad
Omega = 0

# Modificación de therm_events.cxx
El archivo therm_events.cxx lee los parámetros de entrada del archivo therminator.in. La modificación a realizar será para que el parámetro extra Omega recién agregado sea leído. Esto se puede realizar agregando lo siguiente en la linea 43 del código:

double mOmega;
double zRhoMax;

Luego se debe agregar lo siguiente en la linea 91:

if ((sModel == 0) || (sModel == 2)){
try {
mOmega = atof(sRPInstance->getPar"Omega").Data());
}
catch (STR tError) {
mOmega = 0.0;
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

# Modificación de Integrator.cxx
El archivo Integrator.cxx realiza todo el cálculo Monte Carlo de la integral de Cooper-Frye. Aquí solo se agregará lo siguiente en la linea 37:

extern double mOmega;

# Modificación de Particle.cxx
El archivo Particle.cxx guarda las partículas de caa evento en el archivo de salida. La modificación a realizar será para que el parámetro Omega genere una vorticidad final neta. Esto se realiza agregando lo siguiente en la linea 31:

#include <fstream>
#include "Integrator.h"
#include <TFile.h>
extern ReadPar *sRPInstance;
extern int sTables;
extern int sModel;
extern double mOmega;
extern double zRhoMax;

Luego, hay que modificar la expresión para pz en la linea 54, reemplazandola por:

pz=tMt*TMath::Sinh(aRapidity) + mOmega*GetMass()*aRho*TMath::Cos(aPhis)*TMath::Exp(-aRho*aRho/(2*zRhoMax*zRhoMax))/zRhoMax;

# Compilación
Una vez realizadas las modificaciones descritas anteriormente, hay que compilar therminator. Para eso se necesita un compilador de C++ y las librerías de ROOT. Se ejecuta el comando make en el directorio donde se tenga therminator modificado.

# Verificación de prueba
Ya modificado y compilado, uno puede controlar la intensidad de la vorticidad final del sistema cambiando el valor del parámetro de entrada Omega. Un valor Omega=0 corresponde al modelo original sin modificación. Pero si se quiere verificar que valores distintos de cero crean una entrada y salida de partículas a lo largo del eje z se puede realizar una corrida sencilla con Omega=1. Esto se hace cambiando el valor de Omega en el archivo therminator.in y ejecutando therm\_events y therm\_tree.
