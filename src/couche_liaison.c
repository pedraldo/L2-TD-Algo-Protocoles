#include <stdio.h>
#include "couche_liaison.h"
#include "services_physique.h"


/* Ajouter ici des fonctions utilitaires
 * (generer_controle, verifier_controle, connexion,...)
 */
 

 /*Fonction permettant de renvoyer un caractère représentant les données d'une trame*/
char generer_controle(trame_t trame)
{
	char c;
	c = trame.type ^ trame.num_seq ^ trame.lg_info;
	int i;
	for(i=0 ;i<trame.lg_info; i++) c = c ^ trame.info[i];
	return c;
}


/*Fonction qui vérifie les sommes de controle d'une trame reçue avec celles qu'on aurait du recevoir*/
int verifier_controle(trame_t trame)
{
	char c;
	c = generer_controle(trame);

	if(c == trame.fcs) return 1;
	else return 0;
}
