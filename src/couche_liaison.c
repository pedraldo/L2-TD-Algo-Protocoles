#include <stdio.h>
#include "couche_liaison.h"
#include "services_physique.h"


/* Ajouter ici des fonctions utilitaires
 * (generer_controle, verifier_controle, connexion,...)
 */
 
char generer_controle(trame_t trame)
{
	char c;
	int i;
	for(i=0 ;i<trame.lg_info; i++)
	{
		c = c ^ trame.info[i];
	}
	return c;
}

int verifier_controle(trame_t trame)
{
	char c;
	c = generer_controle(trame);

	if(c == trame.fcs) return 1;
	else return 0;
}
