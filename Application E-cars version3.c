#include <stdio.h>
#include <string.h>


#define MAX_VOITURES 50
#define MAX_LOCATIONS 100
#define MAX_USERS 10
#define MAX_ADMINS 2


struct Voiture {
    int id;
    long immatriculation;
    char marque[50];
    char modele[50];
    char etat[30];
    double frais; //frais journalier de la location d'une voiture
};

struct Date {
    int jour;
    int mois;
    int annee;
};

int nombreDeJoursDansMois[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int estAnneeBissextile(int annee) {
    return (annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0);
}

int nombreDeJoursDansMoisDonne(int mois, int annee) {
    if (mois == 2 && estAnneeBissextile(annee)) {
        return 29;
    } else {
        return nombreDeJoursDansMois[mois];
    }
}

int nombreDeJoursDansAnnee(struct Date date) {
    int jours = date.jour;

    for (int i = 1; i < date.mois; ++i) {
        jours += nombreDeJoursDansMoisDonne(i, date.annee);
    }

    return jours;
}

int calculerDureeEnJours(struct Date debut, struct Date fin) {
    int joursDebut = nombreDeJoursDansAnnee(debut);
    int joursFin = nombreDeJoursDansAnnee(fin);

    for (int annee = debut.annee; annee < fin.annee; ++annee) {
        joursFin += estAnneeBissextile(annee) ? 366 : 365;
    }

    return joursFin - joursDebut;
}

void afficherDate(struct Date date) {
    printf("%02d/%02d/%04d", date.jour, date.mois, date.annee);
}

struct Location {
    int id;
    int idVoiture;
    int idUtilisateur;
    struct Date date; //Date de début de la location
    struct Date dateFin;    // Date de fin de la location
    char description[220];
    int duree;  
};

struct Utilisateur {
    char identifiant[20];
    char motDePasse[20];
    char CIN[20];

};

struct Administrateur {
    char identifiant[20];
    char motDePasse[20];
};

void initialiserDonnees(struct Voiture voitures[], int* nombreVoitures) {
    // Ajouter des données initiales pour les voitures
    voitures[0] = (struct Voiture){0, 123456, "Toyota", "Corolla", "Disponible", 30.0};
    voitures[1] = (struct Voiture){1, 234567, "Honda", "Civic", "Disponible", 25.0};
    voitures[2] = (struct Voiture){2, 345678, "Ford", "Focus", "Disponible", 28.0};
    voitures[3] = (struct Voiture){3, 456789, "BMW", "3 Series", "Disponible", 40.0};
    voitures[4] = (struct Voiture){4, 567890, "Tesla", "Model S", "Disponible", 50.0};
    voitures[5] = (struct Voiture){5, 678901, "Audi", "A4", "Disponible", 35.0};
    voitures[6] = (struct Voiture){6, 789012, "Porsche", "911", "Disponible", 60.0};
    voitures[7] = (struct Voiture){7, 890123, "Jeep", "Wrangler", "Disponible", 45.0};
    voitures[8] = (struct Voiture){8, 901234, "Hyundai", "Sonata", "Disponible", 32.0};
    voitures[9] = (struct Voiture){9, 123890, "Volkswagen", "Golf", "Disponible", 38.0};


    // Mettre à jour le nombre de voitures
    *nombreVoitures = 10;
}


struct Facture {
    int id;
    int idLocation;
    double montant;
    struct Date dateFacturation;
};

static int dernierIdFacture = 0;


void afficherFacture(struct Facture facture) {
    printf("------ Facture de Location ------\n");
    printf("ID de Facture: %d\n", facture.id);
    printf("ID de Location: %d\n", facture.idLocation);
    printf("Montant: %.2f\n", facture.montant);
    printf("Date de Facturation: ");
    afficherDate(facture.dateFacturation);
    printf("\n------------------------------\n");
}

// Fonction pour générer une facture à partir d'une location
struct Facture genererFacture(struct Location location, struct Voiture voitures[], int nombreVoitures) {
    struct Facture facture;
    facture.id = ++dernierIdFacture;  // Incrémente et attribue le nouvel ID
    facture.idLocation = location.id;
    
    // Rechercher la voiture louée dans le tableau de voitures
    for (int i = 0; i < nombreVoitures; ++i) {
        if (voitures[i].id == location.idVoiture) {
            facture.montant = location.duree * voitures[i].frais;  // Montant basé sur la durée de location et les frais de la voiture
            break;
        }
    }

    facture.dateFacturation = location.date;  // La date de facturation est la date de fin de location
    return facture;
}


void afficherFacturesVoituresLouees(struct Location locations[], struct Voiture voitures[], int nombreLocations, int nombreVoitures) {
    for (int i = 0; i < nombreLocations; ++i) {
        // Vérifiez si la voiture associée à la location est disponible
        int indexVoiture = -1;
        for (int j = 0; j < nombreVoitures; ++j) {
            if (voitures[j].id == locations[i].idVoiture) {
                indexVoiture = j;
                break;
            }
        }

        if (indexVoiture != -1 && strcmp(voitures[indexVoiture].etat, "Louee") == 0) {
            // Affichez la facture uniquement si la voiture est louée
            struct Facture facture = genererFacture(locations[i], voitures, nombreVoitures);
            afficherFacture(facture);
        }
    }
}


void consulterEtatVoitures(struct Voiture voitures[], int nombreVoitures) {
    printf("------ Listes des Voitures ------\n");
    for (int i = 0; i < nombreVoitures; ++i) {
        printf("ID: %d | Immatriculation: %ld | Marque: %s | Modele: %s | Etat: %s | Frais: %.2f\n", 
               voitures[i].id, voitures[i].immatriculation, voitures[i].marque, voitures[i].modele, 
               voitures[i].etat, voitures[i].frais);
    }
    printf("------------------------------\n");
}


void ajouterNouvelleVoiture(struct Voiture voitures[], int *nombreVoitures) {
    if (*nombreVoitures < MAX_VOITURES) {
        struct Voiture nouvelleVoiture;

        // Saisir les détails de la nouvelle voiture
        printf("Entrez la marque de la nouvelle voiture : ");
        scanf("%s", nouvelleVoiture.marque);

        printf("Entrez le modèle de la nouvelle voiture : ");
        scanf("%s", nouvelleVoiture.modele);

        printf("Entrez l'état initial de la nouvelle voiture : ");
        scanf("%s", nouvelleVoiture.etat);

        printf("Entrez le numéro d'immatriculation de la nouvelle voiture : ");
        scanf("%ld", &nouvelleVoiture.immatriculation);

        printf("Saisir le frais de la location journalière de la nouvelle voiture : ");
        scanf("%lf", &nouvelleVoiture.frais);

        // Affecter un nouvel ID à la voiture
        nouvelleVoiture.id = *nombreVoitures;

        // Ajouter la nouvelle voiture à la liste
        voitures[*nombreVoitures] = nouvelleVoiture;

        // Mettre à jour le nombre de voitures
        (*nombreVoitures)++;

        printf("Nouvelle voiture ajoutée avec succès.\n");
    } else {
        printf("Impossible d'ajouter une nouvelle voiture. Limite atteinte.\n");
    }
}



void consulterHistoriqueLocations(struct Location locations[], int nombreLocations) {
    printf("------ Historique des Locations ------\n");

    if (nombreLocations == 0) {
        printf("Aucune location n'a été effectuée.\n");
    } else {
        for (int i = 0; i < nombreLocations; ++i) {
            printf("ID: %d | Voiture: %d | Début de location: ", locations[i].id, locations[i].idVoiture);

           // printf("ID: %d | Voiture: %d | Début de location: ", locations[i].id, locations[i].idVoiture);
            afficherDate(locations[i].date);  // Afficher la date de début
            printf(" | Fin de location: ");
            afficherDate(locations[i].dateFin);  // Afficher la date de fin
            printf(" | Durée: %d jours", locations[i].duree);  // Afficher la durée
            printf(" | Description: %s\n", locations[i].description);
        }
    }

    printf("--------------------------------------\n");
}



int verifierDate(struct Date date) {
    // Vérifier la validité de la date
    if (date.jour < 1 || date.jour > 31 || date.mois < 1 || date.mois > 12 || date.annee < 1900) {
        return 0; // Date invalide
    }


    return 1; // Date valide
}

int comparerDates(struct Date date1, struct Date date2) {
    // Comparaison par année
    if (date1.annee != date2.annee) {
        return date1.annee - date2.annee;
    }

    // Comparaison par mois
    if (date1.mois != date2.mois) {
        return date1.mois - date2.mois;
    }

    // Comparaison par jour
    return date1.jour - date2.jour;
}



int louerVoiture(struct Voiture voitures[], int nombreVoitures, struct Location locations[], int* nombreLocations,long immatriculation) {
    int idVoiture;
    printf("Entrez l'ID de la voiture à louer : ");
    scanf("%d", &idVoiture);


    // Vérifier si la voiture est disponible
    int indexVoiture = -1;
    for (int i = 0; i < nombreVoitures; ++i) {
        if (voitures[i].immatriculation == immatriculation && strcmp(voitures[i].etat, "Disponible") == 0) {
            indexVoiture = i;
            break;
        }
    }


    if (indexVoiture != -1) {
        // Enregistrer la location
        int nouvelIDLocation = *nombreLocations + 1;
        locations[*nombreLocations].id = nouvelIDLocation;
        locations[*nombreLocations].idVoiture = idVoiture;
        // locations[*nombreLocations].idUtilisateur = idUtilisateur;  // Mettre à jour l'ID de l'utilisateur

        // Saisir la date de location
        printf("Entrez la date de location (jour mois annee) : ");
        scanf("%d %d %d", &locations[*nombreLocations].date.jour,
              &locations[*nombreLocations].date.mois,
              &locations[*nombreLocations].date.annee);

        // Saisir la date de fin de location
        printf("Entrez la date de fin de location (jour mois annee) : ");
        scanf("%d %d %d", &locations[*nombreLocations].dateFin.jour,
              &locations[*nombreLocations].dateFin.mois,
              &locations[*nombreLocations].dateFin.annee);

      // Vérifier si la date de fin est inférieure à la date de début
        if (comparerDates(locations[*nombreLocations].dateFin, locations[*nombreLocations].date) < 0) 
        {
          printf("Erreur : La date de fin de location est antérieure à la date de début. Veuillez vérifier la saisie de vos dates .\n");
          return -1;
        }

                // Vérifier la validité de la date
        if (verifierDate(locations[*nombreLocations].date) && verifierDate(locations[*nombreLocations].dateFin)) 
        {
            // Calculer la durée entre les deux dates
            locations[*nombreLocations].duree = calculerDureeEnJours(locations[*nombreLocations].date, locations[*nombreLocations].dateFin);
            // Saisir une description
            printf("Entrez une description : ");
            scanf(" %[^\n]s", locations[*nombreLocations].description);

            // Mettre à jour l'état de la voiture
            strcpy(voitures[indexVoiture].etat, "Louee");

            (*nombreLocations)++;
            return nouvelIDLocation;  // Retourne le nouvel ID de location
        } 
        
        else 
        {
            printf("Date invalide. La voiture n'a pas été louée.\n");
        }
    } 
    else
     {
        printf("Voiture non disponible ou ID invalide.\n");
    }

    return -1;  // Retourne -1 si la location n'a pas réussi
}



void afficherVoituresLouees(struct Voiture voitures[], struct Location locations[], int nombreLocations, int nombreVoitures) {
    printf("------ Voitures Louees ------\n");

    int aucuneVoitureLouee = 1;  // Variable pour suivre si aucune voiture n'est louée

    for (int i = 0; i < nombreLocations; ++i) {
        int idVoitureLouee = locations[i].idVoiture;
        for (int j = 0; j < nombreVoitures; ++j) {
            if (voitures[j].id == idVoitureLouee && strcmp(voitures[j].etat, "Louee") == 0) {
                printf("ID: %d | Immatriculation: %ld | Marque: %s | Modele: %s | Date de location: ", voitures[j].id,voitures[j].immatriculation ,voitures[j].marque, voitures[j].modele);
                afficherDate(locations[i].date);
                printf(" | Description: %s\n", locations[i].description);
                aucuneVoitureLouee = 0;  // Au moins une voiture est louée
                break;
            }
        }
    }

    if (aucuneVoitureLouee) {
        printf("Vous n'avez pas encore loué une voiture ! \n");
    }

    printf("-------------------------------\n");
}

void retournerVoitureLouee(struct Voiture voitures[], int nombreVoitures, int idVoiture) {
    for (int i = 0; i < nombreVoitures; ++i) {
        if (voitures[i].id == idVoiture && strcmp(voitures[i].etat, "Louee") == 0) {
            // Mettre à jour l'état de la voiture
            strcpy(voitures[i].etat, "Disponible");
            printf("Retour de la voiture louée (ID: %d) effectué avec succès.\n", idVoiture);
            return;
        }
    }

    // Si la voiture n'est pas trouvée ou n'est pas louée
    printf("Erreur : Voiture non trouvée ou n'est pas en état loué (ID: %d).\n", idVoiture);
}



void afficherDescriptionVoiture(struct Voiture voitures[], int nombreVoitures) {
    int idVoiture;
    printf("Entrez l'ID de la voiture : ");
    scanf("%d", &idVoiture);

    for (int i = 0; i < nombreVoitures; ++i) {
        if (voitures[i].id == idVoiture) {
            printf("Numéro d'immatriculation : %d\n", voitures[i].immatriculation);
            printf("Description de la voiture (ID: %d)\n", voitures[i].id);
            printf("Marque: %s\n", voitures[i].marque);
            printf("Modele: %s\n", voitures[i].modele);
            printf("Etat: %s\n", voitures[i].etat);
            return;
        }
    }

    printf("Voiture non trouvée (ID: %d).\n", idVoiture);
}


void supprimerVoitureEnPanne(struct Voiture voitures[], int* nombreVoitures) {
    int idVoiture;
    printf("Entrez l'ID de la voiture en panne à supprimer : ");
    scanf("%d", &idVoiture);

    for (int i = 0; i < *nombreVoitures; ++i) {
        if (voitures[i].id == idVoiture && strstr(voitures[i].etat, "Panne") != NULL) {
            printf("Voiture en panne trouvée - Immatriculation: %d\n", voitures[i].immatriculation);

            // Supprimer la voiture en décalant les éléments suivants
            for (int j = i; j < *nombreVoitures - 1; ++j) {
                voitures[j] = voitures[j + 1];
            }
            (*nombreVoitures)--;
            printf("Voiture supprimée avec succès.\n");
            return;
        }
    }

    printf("Voiture non trouvée ou non en panne (ID: %d).\n", idVoiture);
}


void modifierVoiture(struct Voiture voitures[], int nombreVoitures) {
    int idVoiture;
    char Marque[50], Modele[50], nouvelEtat[20];

    printf("Entrez l'ID de la voiture à modifier : ");
    scanf("%d", &idVoiture);

    // Vider le tampon d'entrée pour éviter les problèmes de newline restants
    while (getchar() != '\n');

    for (int i = 0; i < nombreVoitures; ++i) {
        if (voitures[i].id == idVoiture) {
            printf("Entrez le modele de la voiture : ");
            fgets(Modele, sizeof(Modele), stdin);
            Modele[strlen(Modele) - 1] = '\0';  // Supprimer le caractère de nouvelle ligne

            printf("Entrez le nouvel état de la voiture : ");
            fgets(nouvelEtat, sizeof(nouvelEtat), stdin);
            nouvelEtat[strlen(nouvelEtat) - 1] = '\0';  // Supprimer le caractère de nouvelle ligne

            // Mettre à jour le modèle et l'état de la voiture
            strcpy(voitures[i].modele, Modele);
            strcpy(voitures[i].etat, nouvelEtat);

            printf("Modification de l'état de la voiture (ID: %d) effectuée avec succès.\n", idVoiture);
            return;
        }
    }

    printf("Voiture non trouvée (ID: %d).\n", idVoiture);
}



void afficherVoituresEnPanne(struct Voiture voitures[], int nombreVoitures) {
    printf("------ Voitures en Panne ------\n");
    int aucuneVoitureEnPanne = 1;  // Variable pour suivre si aucune voiture n'est en panne

    for (int i = 0; i < nombreVoitures; ++i) {
        if (strstr(voitures[i].etat, "panne") != NULL || strstr(voitures[i].etat, "Panne") != NULL) {
            printf("ID: %d | Num_Immatriculation: %d | Marque: %s | Modele: %s\n", voitures[i].id, voitures[i].immatriculation, voitures[i].marque, voitures[i].modele);
            aucuneVoitureEnPanne = 0;  // Au moins une voiture est en panne
        }
    }

    if (aucuneVoitureEnPanne) {
        printf("Aucune voiture n'est actuellement en panne.\n");
    }

    printf("-------------------------------\n");
}


// Le cas normal du retour de la voiture louee --> disponible
void RetournerVoiture(struct Voiture voitures[], int nombreVoitures) {
    int idVoiture;
    printf("Entrez l'ID de la voiture à retourner : ");
    scanf("%d", &idVoiture);

    // Saisir le numéro d'immatriculation
    long immatriculation;
    printf("Saisir son numéro d'immatriculation : ");
    scanf("%ld", &immatriculation);

    for (int i = 0; i < nombreVoitures; ++i) {
        if (voitures[i].id == idVoiture && strcmp(voitures[i].etat, "Louee") == 0) {
            // Vérifier le numéro d'immatriculation
            if (voitures[i].immatriculation == immatriculation) {
                // Mettre à jour l'état de la voiture
                strcpy(voitures[i].etat, "Disponible");
                printf("Retour de la voiture louée (ID: %d) effectué avec succès.\n", idVoiture);
                return;
            } else {
                printf("Erreur : Le numéro d'immatriculation ne correspond pas à la voiture louée (ID: %d).\n", idVoiture);
                return;
            }
        }
    }

    // Si la voiture n'est pas trouvée ou n'est pas louée
    printf("Erreur : Voiture non trouvée ou n'est pas en état loué (ID: %d).\n", idVoiture);
}


// Fonction pour réclamer à propos d'une voiture louée
void reclamerVoiture(struct Voiture voitures[], int nombreVoitures) {
    int idVoiture;
    printf("Entrez l'ID de la voiture à réclamer : ");
    scanf("%d", &idVoiture);
    
    long immatriculation;
    printf("Saisir son numéro d'immatriculation : ");
    scanf("%ld", &immatriculation);

    for (int i = 0; i < nombreVoitures; ++i) {

        if (voitures[i].id == idVoiture && strcmp(voitures[i].etat, "Louee") == 0 && voitures[i].immatriculation == immatriculation) {
            // Demander à l'utilisateur s'il veut envoyer une réclamation
            printf("Voulez-vous envoyer une réclamation à propos de cette voiture louée ? (Oui/Non) : ");
            char choixReclamation[5];
            scanf("%s", choixReclamation);

            if (strcmp(choixReclamation, "Oui") == 0) {
                // L'utilisateur veut réclamer, changer l'état de la voiture en "En Panne"
                strcpy(voitures[i].etat, "Panne");
                printf("La réclamation pour la voiture a été enregistrée, l'état est maintenant 'En Panne'.\n");
               
            } else {
                // L'utilisateur ne veut pas réclamer
                if (strcmp(voitures[i].etat, "En Panne") == 0) {
                    // Si l'état est "En Panne", appeler RetournerVoiture
                    RetournerVoiture(voitures, nombreVoitures);
                } else {
                    // Sinon, la voiture reste en état loué
                    printf("La voiture reste en état loué.\n");
                }
            }

            return;
        }
    }

    printf("Voiture non trouvée, non louée ou numéro d'immatriculation incorrect (ID: %d).\n", idVoiture);
}

// Version 1 : Modification qui se base sur la vérification de l'identifiant et du mot de passe de l'utilisateur
void modifierCompteUtilisateurV1(struct Utilisateur utilisateurs[], int nombreUtilisateurs) {
    char identifiant[20];
    char motDePasseActuel[20];
    char nouveauIdentifiant[20];
    char nouveauMotDePasse[20];

    printf("Entrez votre identifiant actuel : ");
    scanf("%s", identifiant);

    // Recherche de l'indice de l'utilisateur dans le tableau
    int indiceUtilisateur = -1;
    for (int i = 0; i < nombreUtilisateurs; ++i) {
        if (strcmp(utilisateurs[i].identifiant, identifiant) == 0) {
            indiceUtilisateur = i;
            break;
        }
    }

    if (indiceUtilisateur == -1) {
        printf("Utilisateur non trouvé.\n");
        return;
    }

    // Saisie et vérification du mot de passe actuel
    printf("Entrez votre mot de passe actuel : ");
    scanf("%s", motDePasseActuel);

    if (strcmp(utilisateurs[indiceUtilisateur].motDePasse, motDePasseActuel) != 0) {
        printf("Mot de passe incorrect.\n");
        return;
    }

    // Modification de l'identifiant
    printf("Entrez votre nouveau identifiant : ");
    scanf("%s", nouveauIdentifiant);
    strcpy(utilisateurs[indiceUtilisateur].identifiant, nouveauIdentifiant);

    // Modification du mot de passe
    printf("Entrez votre nouveau mot de passe : ");
    scanf("%s", nouveauMotDePasse);
    strcpy(utilisateurs[indiceUtilisateur].motDePasse, nouveauMotDePasse);

    printf("Compte utilisateur modifié avec succès.\n");

    // Affichage des nouvelles informations
    printf("Nouvel identifiant : %s\n", utilisateurs[indiceUtilisateur].identifiant);
    printf("Nouveau mot de passe : %s\n", utilisateurs[indiceUtilisateur].motDePasse);
}

// Version 2 : Modification qui se base sur la vérification de l'identifiant , du mot de passe de l'utilisateur et de la CIN

void modifierCompteUtilisateurV2(struct Utilisateur utilisateurs[], int nombreUtilisateurs) {
    char identifiant[20];
    char motDePasseActuel[20];
    char CINActuel[20];
    char nouveauIdentifiant[20];
    char nouveauMotDePasse[20];
    char nouveauCIN[20];

    printf("Entrez votre identifiant : ");
    scanf("%s", identifiant);

    // Recherche de l'indice de l'utilisateur dans le tableau
    int indiceUtilisateur = -1;
    for (int i = 0; i < nombreUtilisateurs; ++i) {
        if (strcmp(utilisateurs[i].identifiant, identifiant) == 0) {
            indiceUtilisateur = i;
            break;
        }
    }

    if (indiceUtilisateur == -1) {
        printf("Utilisateur non trouvé.\n");
        return;
    }

    // Saisie et vérification du mot de passe actuel
    printf("Entrez votre mot de passe actuel : ");
    scanf("%s", motDePasseActuel);

    // Vérification du mot de passe
    if (strcmp(utilisateurs[indiceUtilisateur].motDePasse, motDePasseActuel) != 0) {
        printf("Mot de passe incorrect.\n");
        return;
    }

    // Saisie et vérification du CIN actuel
    printf("Entrez votre CIN actuel : ");
    scanf("%s", CINActuel);

    // Vérification du CIN
    if (strcmp(utilisateurs[indiceUtilisateur].CIN, CINActuel) != 0) {
        printf("CIN incorrect.\n");
        return;
    }

    // Modification de l'identifiant
    printf("Entrez votre nouveau identifiant : ");
    scanf("%s", nouveauIdentifiant);
    strcpy(utilisateurs[indiceUtilisateur].identifiant, nouveauIdentifiant);

    // Modification du mot de passe
    printf("Entrez votre nouveau mot de passe : ");
    scanf("%s", nouveauMotDePasse);
    strcpy(utilisateurs[indiceUtilisateur].motDePasse, nouveauMotDePasse);

    // Modification du CIN
    printf("Entrez votre nouveau CIN : ");
    scanf("%s", nouveauCIN);
    strcpy(utilisateurs[indiceUtilisateur].CIN, nouveauCIN);

    printf("Compte utilisateur modifié avec succès.\n");

    // Affichage des nouvelles informations
    printf("Nouvel identifiant : %s\n", utilisateurs[indiceUtilisateur].identifiant);
    printf("Nouveau mot de passe : %s\n", utilisateurs[indiceUtilisateur].motDePasse);
    printf("Nouveau CIN : %s\n", utilisateurs[indiceUtilisateur].CIN);
}


// Fonction d'authentification pour les utilisateurs
int authentifierUtilisateur(struct Utilisateur utilisateurs[], int nombreUtilisateurs, char identifiant[], char motDePasse[]) {
    for (int i = 0; i < nombreUtilisateurs; ++i) {
        if (strcmp(utilisateurs[i].identifiant, identifiant) == 0 && strcmp(utilisateurs[i].motDePasse, motDePasse) == 0) {
            return 0;  // Utilisateur authentifié
        }
    }
    return -1;  // Échec de l'authentification
}

// Fonction d'authentification pour les administrateurs
int authentifierAdministrateur(struct Administrateur administrateurs[], int nombreAdministrateurs, char identifiant[], char motDePasse[]) {
    for (int i = 0; i < nombreAdministrateurs; ++i) {
        if (strcmp(administrateurs[i].identifiant, identifiant) == 0 && strcmp(administrateurs[i].motDePasse, motDePasse) == 0) {
            return 1;  // Administrateur authentifié
        }
    }
    return -1;  // Échec de l'authentification
}


int main() {
    struct Voiture voitures[MAX_VOITURES];
    struct Location locations[MAX_LOCATIONS];
    struct Utilisateur utilisateurs[MAX_USERS] = {{"Mariem", "mbk1", "16789505"}, {"User2", "user2", "56789421"}, {"Cyril", "Cyril23", "34872192"}};
    struct Administrateur administrateurs[MAX_ADMINS] = {{"Prof1", "pswrd1"},{"admin2", "pswrd2"}};

    int nombreVoitures = 0;
    int nombreLocations = 0;
    int nombreUtilisateurs = 2;
    int nombreAdministrateurs = 2;

    // Initialiser les données
    initialiserDonnees(voitures, &nombreVoitures);

    // Authentification
    int role = -1; // 0 pour utilisateur, 1 pour administrateur
    char identifiant[20];
    char motDePasse[20];

    printf("Identifiant : ");
    scanf("%s", identifiant);

    printf("Mot de passe : ");
    scanf("%s", motDePasse);

    long immatriculation;

    // Authentifier en tant qu'utilisateur
        
    role = authentifierUtilisateur(utilisateurs, nombreUtilisateurs, identifiant, motDePasse);

    // Si l'authentification en tant qu'utilisateur échoue, essayer en tant qu'administrateur
    if (role == -1) {
        role = authentifierAdministrateur(administrateurs, nombreAdministrateurs, identifiant, motDePasse);
    }

    if (role == -1) {
        printf("Erreur : Identifiant ou mot de passe incorrect.\n");
        return 1;  // Quitter le programme avec code d'erreur
    }

    printf("Authentification réussie en tant que %s.\n", (role == 0) ? "utilisateur" : "administrateur");


     
    // Menu principal
    int choix;
    // Pour les utilisateurs
    if (role == 0) {
        do {
            printf("\n-------- Menu Utilisateur --------\n");
            printf("1. Consulter la liste des voitures\n");
            printf("2. Consulter l'historique des locations\n");
            printf("3. Louer une voiture\n");
            printf("4. Afficher la description d'une voiture\n");
            printf("5. Retour d'une voiture\n");
            printf("6. Afficher les voitures louees\n");
            printf("7. Envoyer des reclamations\n");
            printf("8. Afficher ma facture de location des voitures\n");
            printf("9. Changer le compte utilisateur\n");
            printf("10. Quitter\n");
            printf("------------------------------\n");
            
            printf("Entrez votre choix : ");
            scanf("%d", &choix);

            switch (choix) {
                case 1:
                consulterEtatVoitures(voitures, nombreVoitures);
                break;
           
            case 2:
                consulterHistoriqueLocations(locations, nombreLocations);
                break;
            case 3:
                printf("Entrez le numéro d'immatriculation de la voiture à louer : ");
                scanf("%ld", &immatriculation);
                louerVoiture(voitures, nombreVoitures, locations, &nombreLocations, immatriculation);
                break;
            case 4:
                afficherDescriptionVoiture(voitures, nombreVoitures);
                break;
            case 5:
                RetournerVoiture(voitures, nombreVoitures);
                break;
            case 6:
                afficherVoituresLouees(voitures, locations, nombreLocations, nombreVoitures);
                break;
            case 7:
                reclamerVoiture(voitures,nombreVoitures);
                break;

            case 8:
                afficherFacturesVoituresLouees(locations, voitures, nombreLocations, nombreVoitures);
                break;
            case 9:
                modifierCompteUtilisateurV2(utilisateurs, nombreUtilisateurs);
                break;
       
            case 10:
                printf("Programme terminé.\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
        } while (choix != 10);
    }
    // Pour les administrateurs
    else if (role == 1) {
        do {
            printf("\n-------- Menu Administrateur --------\n");
            printf("1. Consulter la liste des voitures\n");
            printf("2. Ajouter une nouvelle voiture\n");
            printf("3. Consulter l'historique des locations\n");
            printf("4. Louer une voiture\n");
            printf("5. Afficher la description d'une voiture\n");
            printf("6. Supprimer une voiture en panne\n");
            printf("7. Modifier la description et l'etat d'une voiture\n");
            printf("8. Retour d'une voiture\n");
            printf("9. Afficher les voitures louees\n");
            printf("10. Afficher les voitures en panne\n");
            printf("11. Afficher la facture de location de la voiture\n");
            printf("12. Quitter\n");
            printf("------------------------------\n");

            printf("Entrez votre choix : ");
            scanf("%d", &choix);

            switch (choix) {
            case 1:
                consulterEtatVoitures(voitures, nombreVoitures);
                break;
            case 2:
                ajouterNouvelleVoiture(voitures, &nombreVoitures);
                break;
            case 3:
                consulterHistoriqueLocations(locations, nombreLocations);
                break;
            case 4:
                printf("Entrez le numéro d'immatriculation de la voiture à louer : ");
                scanf("%ld", &immatriculation);
                louerVoiture(voitures, nombreVoitures, locations, &nombreLocations, immatriculation);
                break;
            case 5:
                afficherDescriptionVoiture(voitures, nombreVoitures);
                break;
            case 6:
                supprimerVoitureEnPanne(voitures, &nombreVoitures);
                break;
            case 7:
                modifierVoiture(voitures, nombreVoitures);
                break;
            case 8:
                RetournerVoiture(voitures, nombreVoitures);
                break;
            case 9:
                afficherVoituresLouees(voitures, locations, nombreLocations, nombreVoitures);
                break;
            case 10:
                afficherVoituresEnPanne(voitures, nombreVoitures);
                break;
            case 11:
                afficherFacturesVoituresLouees(locations, voitures, nombreLocations, nombreVoitures);
                break;

            case 12:
                printf("Programme terminé.\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
        }
        while (choix != 12);
    }

    return 0;
}