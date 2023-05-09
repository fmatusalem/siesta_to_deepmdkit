/*BY FILIPE MATUSALEM, APR 2023     filipematus@gmail.com */
/*Program to extract raw data from SIESTA output*/
/*Compilation: g++ -o program.x program.c*/

//units to use in deepMD-kit

//Property	Unit
//Time	ps
//Length	Å
//Energy	eV
//Force	eV/Å
//Virial	eV
//Pressure	Bar





#include <stdio.h>
#include <getopt.h> // *GNU* Para o getopt_long()
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int main(int argc, char *argv[])
{
FILE *siestaout,*box,*coord,*type,*type_map,*energy,*forces,*virial;
int i,j,k,nspecies,natoms,nsteps;
char str1[150],ch,lixo[150];
double a,volume,tensor[3][3];


if( argc < 2 ){
printf("\n\n");
printf("EEEEEEE   RRRRRRRR   RRRRRRRR   OOOOOOO   RRRRRRRR\n");
printf("EE        RR    RR   RR    RR   OO   OO   RR    RR\n");
printf("EE        RR    RR   RR    RR   OO   OO   RR    RR\n");
printf("EEEE      RRRRRRRR   RRRRRRRR   OO   OO   RRRRRRRR\n");
printf("EE        RRRR       RRRR       OO   OO   RRRR\n");
printf("EE        RR  RR     RR  RR     OO   OO   RR  RR\n");
printf("EEEEEEE   RR    RR   RR    RR   OOOOOOO   RR    RR\n\n");

printf("Enter the name of siesta output file. \n\n");

 exit(0);}

strcpy(str1,argv[1]);

siestaout = fopen(str1,"r"); /* Arquivo ASCII, para leitura */
if(!siestaout)
{
printf( "Error opening argument 1 file\n");

exit(0);
}

printf("\n\nProgram to extract DeepMd-kit .raw files from SIESTA - version 1.0\n\n");
printf("\nREMEMBER TO INCLUDE ""LongOutput T"" FLAG ON SIESTA INPUT!!\n");



//----------------------------------------------
coord = fopen("coord.raw","w"); /* Arquivo ASCII, para escrita */
if(!coord)
{
printf( "Error creating coord.raw file\n");
exit(0);
}

box = fopen("box.raw","w"); /* Arquivo ASCII, para escrita */
if(!box)
{
printf( "Error creating box.raw file\n");
exit(0);
}

type = fopen("type.raw","w"); /* Arquivo ASCII, para escrita */
if(!type)
{
printf( "Error creating type.raw file\n");
exit(0);
}

type_map = fopen("type_map.raw","w"); /* Arquivo ASCII, para escrita */
if(!type_map)
{
printf( "Error creating type.raw file\n");
exit(0);
}

energy = fopen("energy.raw","w"); /* Arquivo ASCII, para escrita */
if(!energy)
{
printf( "Error creating energy.raw file\n");
exit(0);
}

forces = fopen("force.raw","w"); /* Arquivo ASCII, para escrita */
if(!forces)
{
printf( "Error creating forces.raw file\n");
exit(0);
}

virial = fopen("virial.raw","w"); /* Arquivo ASCII, para escrita */
if(!virial)
{
printf( "Error creating virial.raw file\n");
exit(0);
}
//-----------------------------------------------------------

nsteps=0;
while (fscanf(siestaout,"%s",str1) != EOF){            /*conta steps*/
if(strcmp(str1,"Voigt[x,y,z,yz,xz,xy]")==0)nsteps++;                      
}

printf("\nTotal number of MD steps = %d",nsteps);
rewind(siestaout);

do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"NumberOfAtoms")!=0);
fscanf(siestaout,"%d",&natoms);
printf("\nNumber of atoms = %d",natoms);
rewind(siestaout);

do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"NumberOfSpecies")!=0);
fscanf(siestaout,"%d",&nspecies);
printf("\nNumber of species = %d\n",nspecies);
rewind(siestaout);



do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"ChemicalSpeciesLabel")!=0);

do
ch = getc(siestaout);              /*chega ao fim da linha*/
while(ch!='\n');

char species[nspecies][10];

for(i=0;i<nspecies;i++){
fscanf(siestaout,"%s",str1);
fscanf(siestaout,"%s",str1);
fscanf(siestaout,"%s",species[i]);
printf("%s\n",species[i]);
fprintf(type_map,"%s\n",species[i]);
}

fclose(type_map);

//------------------------------------------------------
for(j=0;j<nsteps;j++){

do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"outcoor:")!=0);

do
ch = getc(siestaout);              /*chega ao fim da linha*/
while(ch!='\n');

for(i=0;i<natoms;i++){
fscanf(siestaout,"%lf",&a);fprintf(coord,"%24.18lE ",a);
fscanf(siestaout,"%lf",&a);fprintf(coord,"%24.18lE ",a);
fscanf(siestaout,"%lf",&a);fprintf(coord,"%24.18lE ",a);
if(j==0){fscanf(siestaout,"%d",&k);fprintf(type,"%d\n",k-1);}

do
ch = getc(siestaout);              /*chega ao fim da linha*/
while(ch!='\n');
}
fprintf(coord,"\n");
//*******************************************************

do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"outcell:")!=0);

do
ch = getc(siestaout);              /*chega ao fim da linha*/
while(ch!='\n');

for(i=0;i<9;i++){
fscanf(siestaout,"%lf",&a);fprintf(box,"%24.18lE ",a);
}
fprintf(box,"\n");

do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"volume")!=0);

fscanf(siestaout,"%s",lixo); fscanf(siestaout,"%s",lixo);
fscanf(siestaout,"%lf",&volume);
//printf("%lf\n",volume);
//*********************************************************

do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"E_KS(eV)")!=0);
do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"E_KS(eV)")!=0);

fscanf(siestaout,"%s",lixo); fscanf(siestaout,"%lf",&a);fprintf(energy,"%24.18lE\n",a);
//*********************************************************

do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"(eV/Ang):")!=0);

for(i=0;i<natoms;i++){
fscanf(siestaout,"%s",lixo);
fscanf(siestaout,"%lf",&a);fprintf(forces,"%24.18lE ",a);
fscanf(siestaout,"%lf",&a);fprintf(forces,"%24.18lE ",a);
fscanf(siestaout,"%lf",&a);fprintf(forces,"%24.18lE ",a);
}
fprintf(forces,"\n");
//****************************************************************************************

do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"Voigt[x,y,z,yz,xz,xy]")!=0);

do
fscanf(siestaout,"%s",str1);                                      /*posiciona o  após a palavra xxx*/
while(strcmp(str1,"(eV/Ang**3):")!=0);

fscanf(siestaout,"%lf",&tensor[0][0]);
fscanf(siestaout,"%lf",&tensor[0][1]);
fscanf(siestaout,"%lf",&tensor[0][2]);
fscanf(siestaout,"%lf",&tensor[1][0]);
fscanf(siestaout,"%lf",&tensor[1][1]);
fscanf(siestaout,"%lf",&tensor[1][2]);
fscanf(siestaout,"%lf",&tensor[2][0]);
fscanf(siestaout,"%lf",&tensor[2][1]);
fscanf(siestaout,"%lf",&tensor[2][2]);

// stress to virial conversion, default unit for stress in siesta is kbar
// note the stress is virial (J) = stress (Pa)* volume (m^3) ==> virial (J) = stress (kbar) * 1.0E+8 * volume (Ang^3) * 1.0E-30 ==>  virial (J) = stress (kbar) * volume (Ang^3) * 1.0E-22 ==> virial (eV) = stress (kbar) * volume (Ang^3) * 6.242E-4 

fprintf(virial,"%24.18lE %24.18lE %24.18lE %24.18lE %24.18lE %24.18lE %24.18lE %24.18lE %24.18lE ",tensor[0][0]*volume*6.242E-4,tensor[0][1]*volume*6.242E-4,tensor[0][2]*volume*6.242E-4,tensor[1][0]*volume*6.242E-4,tensor[1][1]*volume*6.242E-4,tensor[1][2]*volume*6.242E-4,tensor[2][0]*volume*6.242E-4,tensor[2][1]*volume*6.242E-4,tensor[2][2]*volume*6.242E-4);
fprintf(virial,"\n");

}

printf("\nSUCCESS!!\n");
printf("\nWrite files for DeepMD-kit, energy.raw, force.raw, coord.raw, box.raw, virial.raw, type.raw and type_map.raw!\n\n");


fclose(siestaout);
fclose(coord);
fclose(box);
fclose(type);
fclose(energy);
fclose(forces);
fclose(virial);
}
