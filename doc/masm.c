/******************************************************************************
*                                                                             *
*  Masm is a utility that assemble microprogram source code into the binary   *
*  form used by the procesim tool, copyright (C) 2000 Yann Remond.            *
*                                                                             *
*  This program is free software; you can redistribute it and/or modify       *
*  it under the terms of the GNU General Public License as published by       *
*  the Free Software Foundation; either version 2 of the License, or          *
*  (at your option) any later version.                                        *
*                                                                             *
*  This program is distributed in the hope that it will be useful,            *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of             *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
*  GNU General Public License for more details.                               *
*                                                                             *
*  You should have received a copy of the GNU General Public License          *
*  along with this program; if not, write to the Free Software                *
*  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA *
*                                                                             *
*  Contact: no contact, this program is not maintained anymore.               *
*                                                                             *
******************************************************************************/

#line 6 "microassembleur.nw"
	
#line 15 "microassembleur.nw"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#line 7 "microassembleur.nw"
	
#line 22 "microassembleur.nw"
	
#line 48 "microassembleur.nw"
	
#line 67 "microassembleur.nw"
typedef enum {
	ETIQUETTE_INCONNUE,
	DUPLICATE_IDF,
	PROG_TROP_GROS,
	NON_MUC_FICHIER,
	IMPOSSIBLE_CREER_FICHIER,
	IMPOSSIBLE_OUVRIR_FICHIER,
	CARACTERE_INVALIDE,
	UNEXPECTED_SYM,
	EXPECTED,
	TOO_MUCH_ERROR,
	READ_ET_MB,
	READ_ET_WRITE,
	PC_OU_PO
} type_erreur;
#line 49 "microassembleur.nw"
	
#line 112 "microassembleur.nw"
typedef struct erreur {
	type_erreur 	type;	// le type de l'erreur
	int				ligne;	// numéro de ligne de l'erreur
	char 			*info;	// information supplémentaire
	struct erreur	*suiv;	// pour traitement en liste chainee
} erreur;
#line 23 "microassembleur.nw"
	
#line 183 "microassembleur.nw"
	
#line 202 "microassembleur.nw"
typedef enum {
	PAS_ASSEZ_DE_READ,
	PAS_ASSEZ_DE_WRITE,
	TOO_MUCH
} type_warning;
#line 184 "microassembleur.nw"
	
#line 229 "microassembleur.nw"
typedef struct warning {
	type_warning 	type;	// le type de le warning
	int				ligne;	// numéro de ligne de le warning
	char 			*info;	// information supplémentaire
	struct warning	*suiv;	// pour traitement en liste chainee
} warning;
#line 24 "microassembleur.nw"
	
#line 301 "microassembleur.nw"
	
#line 320 "microassembleur.nw"
typedef struct etiquette {
	char *nom;				// l'identificateur de l'étiquette
	int adresse;			// l'adresse de définition/ou d'utilisation
	int ligne;				// la ligne du source où apparait l'etiquette
	struct etiquette *suiv;	// pour le traitement en liste chaînée
} etiquette;
#line 25 "microassembleur.nw"
	
#line 397 "microassembleur.nw"
	
#line 415 "microassembleur.nw"
typedef struct microinstruction {
	unsigned int condition	:5;	// les conditions de saut
	unsigned int decaleur	:2;	// l'opération du décaleur
	unsigned int mux		:1;	// la commande du Mux 1
	unsigned int emb		:1; // la commande de chargement de MB
	unsigned int read		:1;	// la commande de lecture de la RAM
	unsigned int write		:1; // la commande d'écriture de la RAM
	unsigned int ema		:1; // la commande de chargement de MA
	unsigned int seti		:1;	// mise à 1 du flag I
	unsigned int clri		:1;	// mise à 0 du flag I
	unsigned int flags		:1;	// mise à jour des flags
	unsigned int UAL		:2;	// l'opération de l'UAL
	unsigned int enablebusC	:1;	// activation du chargement des registres
	unsigned int busC		:4;	// numéro du registre sur bus C
	unsigned int busB		:4;	// numéro du registre sur bus B
	unsigned int busA		:4;	// numéro du registre sur bus A
	unsigned int addr		:8;	// addresse où sauter
	// je met aussi les champs, bien qu'ils soient une partie de addr.
	// je ne fais pas d'union, pour d'obscure raisons de gros et de petits
	// boutistes (big/little endian).
	unsigned int CUAL		:1;	// commande du Mux 7
	unsigned int bitC		:1;	// commande du Mux 5
	unsigned int bitB		:1;	// commande du Mux 3
	unsigned int bitA		:1;	// commande du Mux 4
	char *source;
} microinstruction;
#line 26 "microassembleur.nw"
	
#line 540 "microassembleur.nw"
	
#line 620 "microassembleur.nw"
typedef enum etats {
	E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15,E16,E17,E18,E19,E20
} etats;
#line 541 "microassembleur.nw"
	
#line 1051 "microassembleur.nw"
typedef enum lexeme {
	PC,SP,IR,TIR,ZERO,UN,FF,A,B,C,D,E,F,MK1,MK2,Ac,  MA,MB, LABUS,LBBUS,LCBUS,
	JP,JQ,JI,J0,J1,J2,J3,J4,J5,J6,J7,JN,JZ,JC,
	PLUS, AND, XOR, LUAL, SHL, SHR,
	EMA, EMB, MAJFLAG,
	READ,WRITE,
	DEUXPOINTS,
	CLRI,SETI,
	PARO,PARF,
	EGAL,
	IDF,
	FDL,
	INV
} lexeme;
#line 542 "microassembleur.nw"
	
#line 602 "microassembleur.nw"
typedef struct une_ligne {
	lexeme saut;
	lexeme UAL;
	lexeme UALgauche;
	lexeme UALdroite;
	lexeme result;
	lexeme decaleur;
	lexeme seti;
	lexeme clri;
	lexeme majflag;
	lexeme ema;
	lexeme emb;
	lexeme read;
	lexeme write;
} une_ligne;
#line 8 "microassembleur.nw"
	
#line 30 "microassembleur.nw"
	
#line 53 "microassembleur.nw"
	
#line 86 "microassembleur.nw"
struct message_erreur {
	type_erreur type;
	char *message;
} messages_erreurs[]= {
	{ETIQUETTE_INCONNUE,		"Etiquette '%s' inconnue."},
	{DUPLICATE_IDF,				"Nouvelle définition pour l'étiquette '%s'."},
	{PROG_TROP_GROS,			"Le microprogramme est trop gros."},
	{NON_MUC_FICHIER,	"Le fichier '%s' n'a pas l'extension .muc."},
	{IMPOSSIBLE_CREER_FICHIER,	"Impossible de créer le fichier '%s'."},
	{IMPOSSIBLE_OUVRIR_FICHIER,	"Impossible d'ouvrir le fichier '%s'."},
	{CARACTERE_INVALIDE,		"Caractère invalide : '%s'."},
	{UNEXPECTED_SYM,			"Symbole inattendu : '%s'."},
	{EXPECTED,					"On attend : %s."},
	{TOO_MUCH_ERROR,			"%s en surnombre."},
	{READ_ET_MB,				"Read et affectation de MB en même temps."},
	{READ_ET_WRITE,				"Read et write en même temps."},
	{PC_OU_PO,					"Un saut doit être tout seul."},
};
#line 54 "microassembleur.nw"
	
#line 106 "microassembleur.nw"
int nombre_messages_erreurs=sizeof(messages_erreurs)/sizeof(struct message_erreur);
#line 55 "microassembleur.nw"
	
#line 126 "microassembleur.nw"
erreur *liste_erreurs=NULL;

int nombre_erreurs=0;
#line 31 "microassembleur.nw"
	
#line 188 "microassembleur.nw"
	
#line 212 "microassembleur.nw"
struct message_warning {
	type_warning type;
	char *message;
} messages_warnings[]= {
	{PAS_ASSEZ_DE_READ,		"Nombre de 'read' insuffisant"},
	{PAS_ASSEZ_DE_WRITE,	"Nombre de 'write' insuffisant"},
	{TOO_MUCH,				"Trop de %s."},
};
#line 189 "microassembleur.nw"
	
#line 223 "microassembleur.nw"
int nombre_messages_warnings=sizeof(messages_warnings)/sizeof(struct message_warning);
#line 190 "microassembleur.nw"
	
#line 243 "microassembleur.nw"
warning *liste_warnings=NULL;

int nombre_warnings=0;
#line 32 "microassembleur.nw"
	
#line 305 "microassembleur.nw"
	
#line 331 "microassembleur.nw"
etiquette *liste_etiq_def=NULL;	// liste des définitions d'étiquettes

etiquette *liste_etiq_util=NULL; // liste d'utilisation des étiquettes
#line 33 "microassembleur.nw"
	
#line 401 "microassembleur.nw"
	
#line 446 "microassembleur.nw"
#define TAILLE_ROM	256

microinstruction microcode[TAILLE_ROM];
int nombre_microinstruction=0;
#line 34 "microassembleur.nw"
	
#line 546 "microassembleur.nw"
	
#line 560 "microassembleur.nw"
int adresse_courante=0;

int tabsize=8;		// la taille d'une tabulation, en caractere
#line 547 "microassembleur.nw"
	
#line 1131 "microassembleur.nw"
struct symbolesetmotscles {
	char *nom;
	lexeme representation;
} keywordsandsymbols[] = {
	{	"PC",		PC			},
	{	"SP",		SP			},
	{	"IR",		IR			},
	{	"TIR",		TIR			},
	{	"0",		ZERO		},
	{	"1",		UN			},
	{	"FF",		FF			},
	{	"A",		A			},
	{	"B",		B			},
	{	"C",		C			},
	{	"D",		D			},
	{	"E",		E			},
	{	"F",		F			},
	{	"MK1",		MK1			},
	{	"MK2",		MK2			},
	{	"Ac",		Ac			},
	{	"MA",		MA			},
	{	"MB",		MB			},
	{	"LABUS",	LABUS		},
	{	"LBBUS",	LBBUS		},
	{	"LCBUS",	LCBUS		},
	{	"+",		PLUS		},
	{	"AND",		AND			},
	{	"XOR",		XOR			},
	{	"LUAL",		LUAL		},
	{	"SHL",		SHL			},
	{	"SHR",		SHR			},
	{	"EMA",		EMA			},
	{	"EMB",		EMB			},
	{	"MAJFLAG",	MAJFLAG		},
	{	"READ",		READ		},
	{	"WRITE",	WRITE		},
	{	":",		DEUXPOINTS	},
	{	"CLRI",		CLRI		},
	{	"SETI",		SETI		},
	{	"(",		PARO		},
	{	")",		PARF		},
	{	"=",		EGAL		},
	{	"JP",		JP			},
	{	"JQ",		JQ			},
	{	"JI",		JI			},
	{	"J0",		J0			},
	{	"J1",		J1			},
	{	"J2",		J2			},
	{	"J3",		J3			},
	{	"J4",		J4			},
	{	"J5",		J5			},
	{	"J6",		J6			},
	{	"J7",		J7			},
	{	"JN",		JN			},
	{	"JZ",		JZ			},
	{	"JC",		JC			},
};

int nombresymbolesetmotscles=sizeof(keywordsandsymbols)/sizeof(struct symbolesetmotscles);
#line 9 "microassembleur.nw"
	
#line 38 "microassembleur.nw"
	
#line 59 "microassembleur.nw"
	
#line 138 "microassembleur.nw"
void ajout_erreur(type_erreur type,int ligne,char *info) {
	erreur *p=(erreur *)malloc(sizeof(erreur));
	p->type=type;
	p->ligne=ligne;
	p->info=strdup(info);
	p->suiv=liste_erreurs;
	liste_erreurs=p;
	nombre_erreurs++;
}
#line 60 "microassembleur.nw"
	
#line 154 "microassembleur.nw"
void recurs_affichage_erreurs(erreur *e) {
	if (e==NULL) return;
	recurs_affichage_erreurs(e->suiv);
	{ 
		int i;	// ici pour ne pas surcharger la pile au moment du parcrours
				// récursif. Ca marche ?
		for(i=0;i<nombre_messages_erreurs;i++) {
			if (messages_erreurs[i].type==e->type) break;
		}
		if (e->ligne==-1) fprintf(stderr,"Erreur        : ");
		else fprintf(stderr,"Erreur  (%3d) : ",e->ligne);
		fprintf(stderr,messages_erreurs[i].message,e->info);
		fprintf(stderr,"\n");
	}
}
#line 61 "microassembleur.nw"
	
#line 174 "microassembleur.nw"
void affichage_erreurs() {
	fprintf(stderr,"Liste des erreurs\n");
	recurs_affichage_erreurs(liste_erreurs);
}
#line 39 "microassembleur.nw"
	
#line 194 "microassembleur.nw"
	
#line 255 "microassembleur.nw"
void ajout_warning(type_warning type,int ligne,char *info) {
	warning *p=(warning *)malloc(sizeof(warning));
	p->type=type;
	p->ligne=ligne;
	p->info=strdup(info);
	p->suiv=liste_warnings;
	liste_warnings=p;
	nombre_warnings++;
}
#line 195 "microassembleur.nw"
	
#line 271 "microassembleur.nw"
void recurs_affichage_warnings(warning *e) {
	if (e==NULL) return;
	recurs_affichage_warnings(e->suiv);
	{ 
		int i;	// ici pour ne pas surcharger la pile au moment du parcours
				// récursif. Ca marche ?
		for(i=0;i<nombre_warnings;i++) {
			if (messages_warnings[i].type==e->type) break;
		}
		fprintf(stderr,"Warning (%3d) : ",e->ligne);
		fprintf(stderr,messages_warnings[i].message,e->info);
		fprintf(stderr,"\n");
	}
}
#line 196 "microassembleur.nw"
	
#line 290 "microassembleur.nw"
void affichage_warnings() {
	fprintf(stderr,"Liste des warnings\n");
	recurs_affichage_warnings(liste_warnings);
}
#line 40 "microassembleur.nw"
	
#line 309 "microassembleur.nw"
	
#line 344 "microassembleur.nw"
int existe_etiquette(char *nom, etiquette *liste, etiquette **retour) {
	etiquette *l;
	for(l=liste;l!=NULL;l=l->suiv) {
		if (strcmp(nom,l->nom)==0) {
			*retour=l;
			return 1;
		}
	}
	*retour=NULL;
	return 0;
}
#line 310 "microassembleur.nw"
	
#line 364 "microassembleur.nw"
void ajout_etiquette_definition(char *nom, int adresse,int ligne) {
	etiquette *dummy;
	if (existe_etiquette(nom,liste_etiq_def,&dummy)) {
		ajout_erreur(DUPLICATE_IDF,ligne,nom);
	} else {
		etiquette *eti=(etiquette *)malloc(sizeof(etiquette));
		eti->nom=strdup(nom);
		eti->adresse=adresse;
		eti->ligne=ligne;
		eti->suiv=liste_etiq_def;
		liste_etiq_def=eti;
	}
}
#line 311 "microassembleur.nw"
	
#line 384 "microassembleur.nw"
void ajout_etiquette_utilisation(char *nom, int adresse,int ligne) {
	etiquette *eti=(etiquette *)malloc(sizeof(etiquette));
	eti->nom=strdup(nom);
	eti->adresse=adresse;
	eti->ligne=ligne;
	eti->suiv=liste_etiq_util;
	liste_etiq_util=eti;
}
#line 41 "microassembleur.nw"
	
#line 405 "microassembleur.nw"
	
#line 455 "microassembleur.nw"
void ajout_microinstruction(microinstruction MI,int ligne) {
	if (nombre_microinstruction>=TAILLE_ROM) {
		// on a forcé sur le nombre de micro-instructions
		if (nombre_microinstruction==TAILLE_ROM) {
			ajout_erreur(PROG_TROP_GROS,ligne,"");
			nombre_microinstruction++;
		}
	} else {	// c'est tout bon !
		// on assure la cohérence addr<-->CUAL/bitC/bitB/bitA
		MI.addr|=MI.CUAL<<3;		MI.addr|=MI.bitC<<2;
		MI.addr|=MI.bitB<<1;		MI.addr|=MI.bitA;
		MI.CUAL|=(MI.addr>>3)&1;	MI.bitC|=(MI.addr>>2)&1;
		MI.bitB|=(MI.addr>>1)&1;	MI.bitA|=MI.addr&1;
		microcode[nombre_microinstruction]=MI;
		microcode[nombre_microinstruction].source=strdup(MI.source);
		nombre_microinstruction++;
	}
}
#line 406 "microassembleur.nw"
	
#line 478 "microassembleur.nw"
void resolution_etiquettes() {
	etiquette *p,*eti;
	for(p=liste_etiq_util;p!=NULL;p=p->suiv) {
		if (existe_etiquette(p->nom,liste_etiq_def,&eti)) {
			microcode[p->adresse].addr=eti->adresse;
		} else {
			ajout_erreur(ETIQUETTE_INCONNUE,p->ligne,p->nom);
		}
	}
}
#line 407 "microassembleur.nw"
	
#line 505 "microassembleur.nw"
void generation_fichier_sortie(char *fichier) {
	FILE *f;
	if ((f=fopen(fichier,"wb"))==NULL) {
		ajout_erreur(IMPOSSIBLE_CREER_FICHIER,-1,fichier);
	} else {
		int i,val,n;
		for(i=0;i<nombre_microinstruction;i++) {
			val=microcode[i].condition;		n=5;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 513 "microassembleur.nw"
			val=microcode[i].decaleur;		n=2;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 514 "microassembleur.nw"
			val=microcode[i].mux;			n=1;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 515 "microassembleur.nw"
			val=microcode[i].emb;			n=1;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 516 "microassembleur.nw"
			val=microcode[i].read;			n=1;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 517 "microassembleur.nw"
			val=microcode[i].write;			n=1;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 518 "microassembleur.nw"
			val=microcode[i].ema;			n=1;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 519 "microassembleur.nw"
			val=microcode[i].seti;			n=1;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 520 "microassembleur.nw"
			val=microcode[i].clri;			n=1;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 521 "microassembleur.nw"
			val=microcode[i].flags;			n=1;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 522 "microassembleur.nw"
			val=microcode[i].UAL;			n=2;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 523 "microassembleur.nw"
			val=microcode[i].enablebusC;	n=1;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 524 "microassembleur.nw"
			val=microcode[i].busC;			n=4;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 525 "microassembleur.nw"
			val=microcode[i].busB;			n=4;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 526 "microassembleur.nw"
			val=microcode[i].busA;			n=4;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 527 "microassembleur.nw"
			val=microcode[i].addr;			n=8;	
#line 493 "microassembleur.nw"
{
	int j;
	for(j=0;j<n;j++) {
		int k=((val&(1<<(n-j-1)))
					>>(n-j-1))&1;	// noweb !
		fprintf(f,"%c",'0'+k);
	}
	fprintf(f," ");
}
#line 528 "microassembleur.nw"
			if (microcode[i].source!=NULL) 
				fprintf(f,"%s",microcode[i].source);
			fprintf(f,"\n");
		}
		fclose(f);
	}
}
#line 42 "microassembleur.nw"
	
#line 551 "microassembleur.nw"
	
#line 1193 "microassembleur.nw"
int ispeut_etre_id(char c) {
	return (isalpha((int)c) || isxdigit((int)c) || (c=='_'));
}
#line 552 "microassembleur.nw"
	
#line 1199 "microassembleur.nw"
int ispeut_etre_deb_id(char c) {
	return (isalpha((int)c) || (c=='_'));
}
#line 553 "microassembleur.nw"
	
#line 1205 "microassembleur.nw"
lexeme lecture_lexeme(char **tab,char **idf,int ligne) {
	char txt[10000]; // ca suffit !!!
	int ptr=0; int i;
	while (isspace((int)((*tab)[0]))) (*tab)++;
	txt[ptr]=(*tab)[0]; (*tab)++; ptr++;
	if (ispeut_etre_deb_id(txt[ptr-1])) {
		while (ispeut_etre_id((int)((*tab)[0]))) {
			txt[ptr]=(*tab)[0]; (*tab)++; ptr++;
		};
	}
	txt[ptr]='\0';
	// trouver le lexeme
	for(i=0;i<nombresymbolesetmotscles;i++) {
		if (strcasecmp(keywordsandsymbols[i].nom,txt)==0)
			return keywordsandsymbols[i].representation;
	}
	if ((txt[0]=='\0') || (txt[0]=='!')) return FDL;
	if (ispeut_etre_id(txt[0])) { (*idf)=strdup(txt); return IDF; }
	ajout_erreur(CARACTERE_INVALIDE,ligne,txt); return INV;
}
#line 554 "microassembleur.nw"
	
#line 1068 "microassembleur.nw"
char *trad(lexeme l,char *idf) {
	switch (l) {
		case PC:		return "PC"; break;
		case SP:		return "SP"; break;
		case IR:		return "IR"; break;
		case TIR:		return "TIR"; break;
		case ZERO:		return "0"; break;
		case UN:		return "1"; break;
		case FF:		return "ff"; break;
		case A:			return "A"; break;
		case B:			return "B"; break;
		case C:			return "C"; break;
		case D:			return "D"; break;
		case E:			return "E"; break;
		case F:			return "F"; break;
		case MK1:		return "MK1"; break;
		case MK2:		return "MK2"; break;
		case Ac:		return "Ac"; break;
		case MA:		return "MA"; break;
		case MB:		return "MB"; break;
		case LABUS:		return "LABUS"; break;
		case LBBUS:		return "LBBUS"; break;
		case LCBUS:		return "LCBUS"; break;
		case JP:		return "JP"; break;
		case JQ:		return "JQ"; break;
		case JI:		return "JI"; break;
		case J0:		return "J0"; break;
		case J1:		return "J1"; break;
		case J2:		return "J2"; break;
		case J3:		return "J3"; break;
		case J4:		return "J4"; break;
		case J5:		return "J5"; break;
		case J6:		return "J6"; break;
		case J7:		return "J7"; break;
		case JN:		return "JN"; break;
		case JZ:		return "JZ"; break;
		case JC:		return "JC"; break;
		case PLUS:		return "+"; break;
		case AND:		return "and"; break;
		case XOR:		return "xor"; break;
		case LUAL:		return "lual"; break;
		case SHL:		return "shl"; break;
		case SHR:		return "shr"; break;
		case EMA:		return "ema"; break;
		case EMB:		return "emb"; break;
		case MAJFLAG:	return "majflag"; break;
		case READ:		return "read"; break;
		case WRITE:		return "write"; break;
		case DEUXPOINTS:return ":"; break;
		case CLRI:		return "clri"; break;
		case SETI:		return "seti"; break;
		case PARO:		return "("; break;
		case PARF:		return ")"; break;
		case EGAL:		return "="; break;
		case IDF:		return idf; break;
		case FDL:		return "<fdl>"; break;
		case INV:		return "<inv>"; break;
	}
	return "Ceci est un bug !!!!";
}
#line 555 "microassembleur.nw"
	
#line 580 "microassembleur.nw"
#define TAILLE_MAX_LIGNE	10000

void analyse_fichier(FILE *f) {
	char tab[TAILLE_MAX_LIGNE]; // ca devrait suffire.
	int i;
	int numero_ligne=1;
	int verif_read=0; 
	int verif_write=0;
	if (f==NULL) {
		return;
	} else {
		while (!feof(f)) {
			for(i=0;i<TAILLE_MAX_LIGNE;i++) tab[i]='\0';
			fgets(tab,TAILLE_MAX_LIGNE-1,f);
			
#line 626 "microassembleur.nw"
{
	char *p=tab;
	char *idf;
	char *idf2;
	lexeme lex;
	int saut=0,po=0;
	une_ligne l={INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV};
	int vide=1;
	int dpi=0;	// decal ? (Decal Point d'Interrogation) (pour les neu-neu !)
	etats etat=E1;
	etats etat_suivant;
	do {
		switch (etat) {
			case E1:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				if (lex==IDF) {
					idf2=idf;
					etat_suivant=E2; 
				} else etat_suivant=E4;
				break;
			case E2:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				if (lex==DEUXPOINTS) {
					ajout_etiquette_definition(idf2,adresse_courante,numero_ligne);
					free(idf2);
				} else {
					ajout_erreur(EXPECTED,numero_ligne,"':'");
				}
				etat_suivant=E3; // de toute facon, on continue
				break;
			case E3:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				etat_suivant=E4;
				break;
			case E4: // lex a ete lu avant.
				switch (lex) {
					case PC:	case SP:	case IR:	case TIR:	case ZERO:
					case UN:	case FF:	case A:		case B:		case C:
					case D:		case E:		case F:		case MK1:	case MK2:
					case Ac:	case MB:	case LCBUS:
						l.result=lex;
						vide=0;
						po=1;
						if (saut) {
							ajout_erreur(PC_OU_PO,numero_ligne,"");
							etat_suivant=E3;
						} else {
							etat_suivant=E12;
						}
						break;
					case SETI:		etat_suivant=E5;	vide=0;	break;
					case CLRI:		etat_suivant=E6;	vide=0;	break;
					case MAJFLAG:	etat_suivant=E7;	vide=0;	break;
					case READ:		etat_suivant=E8;	vide=0;	break;
					case WRITE:		etat_suivant=E9;	vide=0;	break;
					case EMA:		etat_suivant=E10;	vide=0;	break;
					case EMB:		etat_suivant=E11;	vide=0;	break;
					case FDL:									break;
					case JP:	case JQ:	case JI:	case J0:	case J1:
					case J2:	case J3:	case J4:	case J5:	case J6:
					case J7:	case JN:	case JZ:	case JC:
						l.saut=lex;
						vide=0;
						saut=1;
						if (po) {
							ajout_erreur(PC_OU_PO,numero_ligne,"");
							etat_suivant=E3;
						} else {
							etat_suivant=E19;
						}
						break;
					default:
						ajout_erreur(UNEXPECTED_SYM,numero_ligne,trad(lex,idf));
						etat_suivant=E3;
				}
				break;
			case E5:	
				if (l.seti!=INV) {
					ajout_warning(TOO_MUCH,numero_ligne,"'seti'");
				}
				l.seti=SETI; etat_suivant=E3; vide=0; break;
			case E6:
				if (l.clri!=INV) {
					ajout_warning(TOO_MUCH,numero_ligne,"'clri'");
				}
				l.clri=CLRI; etat_suivant=E3; vide=0; break;
			case E7:
				if (l.majflag!=INV) {
					ajout_warning(TOO_MUCH,numero_ligne,"'majflag'");
				}
				l.majflag=MAJFLAG; etat_suivant=E3; vide=0; break;
			case E8:
				if (l.read!=INV) {
					ajout_warning(TOO_MUCH,numero_ligne,"'read'");
				}
				l.read=READ; etat_suivant=E3; vide=0; break;
			case E9:
				if (l.write!=INV) {
					ajout_warning(TOO_MUCH,numero_ligne,"'write'");
				}
				l.write=WRITE; etat_suivant=E3; vide=0; break;
			case E10:
				if (l.ema!=INV) {
					ajout_warning(TOO_MUCH,numero_ligne,"'ema'");
				}
				l.ema=EMA;	etat_suivant=E3; vide=0; break;
			case E11:
				if (l.emb!=INV) {
					ajout_warning(TOO_MUCH,numero_ligne,"'emb'");
				}
				l.emb=EMB;	etat_suivant=E3;	vide=0; break;
			case E12:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				switch (lex) {
					case EGAL:	etat_suivant=E13;	break;
					case FDL:						break;
					default:
						ajout_erreur(UNEXPECTED_SYM,numero_ligne,trad(lex,idf));
						ajout_erreur(EXPECTED,numero_ligne,"'='");
						etat_suivant=E3;
				}
				break;
			case E13:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				switch (lex) {
					case PC:	case SP:	case IR:	case TIR:	case ZERO:
					case UN:	case FF:	case A:		case B:		case C:
					case D:		case E:		case F:		case MK1:	case MK2:
					case Ac:	case MB:	case LABUS:
						l.UALgauche=lex;
						etat_suivant=E16;
						dpi=0;
						break;
					case SHL: case SHR:
						if (l.decaleur!=INV) {
							ajout_erreur(TOO_MUCH_ERROR,numero_ligne,"opération décaleur");
						}
						l.decaleur=lex;
						etat_suivant=E14;
						dpi=1;
						break;
					case FDL:						break;
					default:
						ajout_erreur(UNEXPECTED_SYM,numero_ligne,trad(lex,idf));
						ajout_erreur(EXPECTED,numero_ligne,"Registre ou opération décalage");
						etat_suivant=E3;
				}
				break;
			case E14:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				switch (lex) {
					case PARO:	etat_suivant=E15;	break;
					case FDL:						break;
					default:
						ajout_erreur(UNEXPECTED_SYM,numero_ligne,trad(lex,idf));
						ajout_erreur(EXPECTED,numero_ligne,"'('");
						etat_suivant=E3;
				}
				break;
			case E15:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				switch (lex) {
					case PC:	case SP:	case IR:	case TIR:	case ZERO:
					case UN:	case FF:	case A:		case B:		case C:
					case D:		case E:		case F:		case MK1:	case MK2:
					case Ac:	case MB:	case LABUS:
						l.UALgauche=lex;
						etat_suivant=E16;
						break;
					case FDL:						break;
					default:
						ajout_erreur(UNEXPECTED_SYM,numero_ligne,trad(lex,idf));
						ajout_erreur(EXPECTED,numero_ligne,"Registre");
						etat_suivant=E3;
				}
				break;
			case E16:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				switch (lex) {
					case PLUS:	case AND:	case XOR:	case LUAL:
						if (l.UAL!=INV) {
							ajout_erreur(TOO_MUCH_ERROR,numero_ligne,"opération UAL");
						}
						l.UAL=lex;
						etat_suivant=E17;
						break;
					case SETI:	case CLRI:	case MAJFLAG:	case READ:
					case WRITE:	case EMA:	case EMB:
						etat_suivant=E4;
						break;
					case PARF:
						etat_suivant=E20;
						break;
					case FDL:	etat_suivant=E4;		break;
					default:
						ajout_erreur(UNEXPECTED_SYM,numero_ligne,trad(lex,idf));
						ajout_erreur(EXPECTED,numero_ligne,"Opérateur");
						etat_suivant=E3;
				}
				break;
			case E17:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				switch (lex) {
					case PC:	case SP:	case IR:	case TIR:	case ZERO:
					case UN:	case FF:	case A:		case B:		case C:
					case D:		case E:		case F:		case MK1:	case MK2:
					case Ac:	case LBBUS:
						l.UALdroite=lex;
						etat_suivant=E18;
						break;
					case FDL:						break;
					default:
						ajout_erreur(UNEXPECTED_SYM,numero_ligne,trad(lex,idf));
						ajout_erreur(EXPECTED,numero_ligne,"Registre");
						etat_suivant=E3;
				}
				break;
			case E18:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				etat_suivant=E20;
				break;
			case E19:
				lex=lecture_lexeme(&p,&idf,numero_ligne);
				switch (lex) {
					case IDF:
						ajout_etiquette_utilisation(idf,adresse_courante,numero_ligne);

						break;
					case FDL:                   break;
					default:
						ajout_erreur(UNEXPECTED_SYM,numero_ligne,trad(lex,idf));
						ajout_erreur(EXPECTED,numero_ligne,"etiquette");
				}
				etat_suivant=E3;
				break;
			case E20:
				if (dpi) {
					switch (lex) {
						case PARF:					break;
						case FDL: 					break;
						default:
							ajout_erreur(UNEXPECTED_SYM,numero_ligne,trad(lex,idf));
							ajout_erreur(EXPECTED,numero_ligne,"')'");
					}
					etat_suivant=E3;
				} else etat_suivant=E4;
				break;
		}
		etat=etat_suivant;
	} while (lex!=FDL);
	if ((etat!=E4) && (etat!=E1) && (etat!=E20)) {
		ajout_erreur(UNEXPECTED_SYM,numero_ligne,"<fin de ligne>");
	}
	if (!vide) {
		microinstruction MI;
		MI.addr=0; MI.bitA=0; MI.bitB=0; MI.bitC=0; MI.source=NULL;
		switch (l.saut) {
			case JP: MI.condition=16; break;
			case JQ: MI.condition=17; break;
			case JI: MI.condition=23; break;
			case J0: MI.condition=24; break;
			case J1: MI.condition=25; break;
			case J2: MI.condition=26; break;
			case J3: MI.condition=27; break;
			case J4: MI.condition=28; break;
			case J5: MI.condition=29; break;
			case J6: MI.condition=30; break;
			case J7: MI.condition=31; break;
			case JN: MI.condition=21; break;
			case JZ: MI.condition=22; break;
			case JC: MI.condition=20; break;
			default:
				MI.condition=0;
		}
		switch (l.decaleur) {
			case SHR: MI.decaleur=2; break;
			case SHL: MI.decaleur=1; break;
			default:
				MI.decaleur=0;
		}
		switch (l.UALgauche) {
			case PC:	case SP:	case IR:	case TIR:	case ZERO:
			case UN:	case FF:	case A:		case B:		case C:
			case D:		case E:		case F:		case MK1:	case MK2:
			case Ac:	
				MI.busA=l.UALgauche-PC; // pas tres propre.
				MI.bitA=0;
				MI.mux=1;
				break;
			case MB:	
				MI.mux=0;
				MI.busA=0;
				break;
			case LABUS:	
				MI.mux=1;
				MI.busA=0;
				MI.bitA=1;
				break;
			default:
				MI.mux=0;
				MI.busA=0;
				MI.bitA=0;
		}
		switch (l.emb) {
			case EMB: MI.emb=1; break;
			default: MI.emb=0;
		}
		switch (l.read) {
			case READ: MI.read=1; break;
			default: MI.read=0;
		}
		switch (l.write) {
			case WRITE: MI.write=1; break;
			default: MI.write=0;
		}
		switch (l.ema) {
			case EMA:	MI.ema=1; break;
			default:	MI.ema=0;
		}
		switch (l.seti) {
			case SETI:	MI.seti=1; break;
			default:	MI.seti=0;
		}
		switch (l.clri) {
			case CLRI:	MI.clri=1; break;
			default:	MI.clri=0;
		}
		switch (l.majflag) {
			case MAJFLAG:	MI.flags=1; break;
			default:		MI.flags=0;
		}
		switch (l.UAL) {
			case PLUS:	MI.UAL=0;	break;
			case AND:	MI.UAL=2;	break;
			case XOR:	MI.UAL=3;	break;
			default:	MI.UAL=1;
		}
		if (l.UAL==LUAL) MI.CUAL=1; else MI.CUAL=0;
		switch (l.UALdroite) {
			case PC:	case SP:	case IR:	case TIR:	case ZERO:
			case UN:	case FF:	case A:		case B:		case C:
			case D:		case E:		case F:		case MK1:	case MK2:
			case Ac:	
				MI.busB=l.UALdroite-PC; // pas tres propre.
				MI.bitB=0;
				break;
			case LBBUS:	
				MI.busB=0;
				MI.bitB=1;
				break;
			default:
				MI.busB=0;
				MI.bitB=0;
		}
		switch (l.result) {
			case PC:	case SP:	case IR:	case TIR:	case ZERO:
			case UN:	case FF:	case A:		case B:		case C:
			case D:		case E:		case F:		case MK1:	case MK2:
			case Ac:	
				MI.busC=l.result-PC; // pas tres propre.
				MI.bitC=0;
				MI.enablebusC=1;
				break;
			case LCBUS:
				MI.busC=0;
				MI.bitC=1;
				MI.enablebusC=1;
				break;
			case MB:
				MI.busC=0;
				MI.bitC=0;
				MI.enablebusC=0;
				MI.emb=1;
				if (l.read==READ) {
					ajout_erreur(READ_ET_MB,numero_ligne,"");
				}
				break;
			default:
				MI.busC=0;
				MI.bitC=0;
				MI.enablebusC=0;
		}
		if ((l.read==READ) && (l.write==WRITE)) {
			ajout_erreur(READ_ET_WRITE,numero_ligne,"");
		}
		if (tab[strlen(tab)-1]=='\n')
			tab[strlen(tab)-1]='\0';
		{
			char *coco=(char *)malloc(strlen(tab)*tabsize+1);
			int i,j;
			for(i=0,j=0;i<strlen(tab);i++) {
				if (tab[i]=='\t') {
					do {
						coco[j++]=' ';
					} while (j%tabsize);
				} else {
					coco[j++]=tab[i];
				}
			}
			coco[j]='\0';
			MI.source=coco;
		}
		// apres le while, aussi
		if (verif_read && (!MI.read)) {
			ajout_warning(PAS_ASSEZ_DE_READ,numero_ligne,"");
			verif_read=0;
		}
		if (verif_write && (!MI.write)) {
			ajout_warning(PAS_ASSEZ_DE_WRITE,numero_ligne,"");
			verif_write=0;
		}
		if (MI.read) verif_read=(verif_read+1)%2;
		if (MI.write) verif_write=(verif_write+1)%3;
		ajout_microinstruction(MI,numero_ligne);
		if (MI.source!=NULL) free(MI.source);
		adresse_courante++;
	}
}
#line 595 "microassembleur.nw"
			numero_ligne++;
		}
	}
}
#line 556 "microassembleur.nw"
	
#line 566 "microassembleur.nw"
FILE *ouvrir_fichier(char *fichier) {
	FILE *f;
	if (fichier==NULL) 
		return stdin;
	if ((f=fopen(fichier,"rb"))==NULL) {
		ajout_erreur(IMPOSSIBLE_OUVRIR_FICHIER,-1,fichier);
		return NULL;
	} else {
		return f;
	}
}
#line 10 "microassembleur.nw"
	
#line 1230 "microassembleur.nw"
void affichage_diagnostique() {
	if (nombre_erreurs)  affichage_erreurs();
	if (nombre_warnings) affichage_warnings();
	fprintf(stderr,"-------------------------\n");
	fprintf(stderr,"Nombre d'erreurs   : %d\n",nombre_erreurs);
	fprintf(stderr,"Nombre de warnings : %d\n",nombre_warnings);
	fprintf(stderr,"-------------------------\n");
}
#line 11 "microassembleur.nw"
	
#line 1243 "microassembleur.nw"
#line 1246 "microassembleur.nw"
int main(int argc,char **argv) {
	char *output, *ext;
	int code_retour=0,len;

	if (argc>2) {
		fprintf(stderr,"Utilisation : %s [<nom fichier>]\n",argv[0]);
		code_retour=1;
	} else {
		if (argc<2) {
			analyse_fichier(ouvrir_fichier(NULL));
		} else {
			analyse_fichier(ouvrir_fichier(argv[1]));
		}
		resolution_etiquettes();
		if (argc<2) {
			generation_fichier_sortie("sortie.mob");
		} else {
			len = strlen(argv[1]);
			output = (char *)malloc(len);
			strncpy(output,argv[1],len+1);
			ext = strrchr(output, '.');
			printf("Compiling %s\n", output);
			if(!strncmp(ext,".muc", 4)) {
				strcpy(ext,".mob");
				generation_fichier_sortie(output);
				printf("Output %s\n", output);
			} else {
				ajout_erreur(NON_MUC_FICHIER,-1,argv[1]);
			}
		}
		affichage_diagnostique();
	}

	return code_retour;
}
