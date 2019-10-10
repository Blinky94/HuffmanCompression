#include <iostream>
#include <fstream>
#include<stdlib.h>
#include <list>
#include<windows.h>
#include <clocale>
#include<bitset>

using namespace std;

static int size_tab_freq = 0;
static int count_IsFils = 0;

///Structure du code Huffman
typedef struct Code_Huffman
{
    int ASCII_code;
    string Huffman_code;
    string Binary_code;
};


///Structure d'une Feuille ou noeud
typedef struct Feuille
{
    Feuille* adresse;
    Feuille* successeur;
    bool code_binaire;
    char character;
    int ASCII_code;
    int frequence;
    Feuille* predecesseur_G;
    Feuille* predecesseur_D;
    bool Is_Left;
    bool Is_Visited;
};

void Affichage_Node(Feuille* feuille);

///Fonction qui retourne le tableau des feuilles
///En remplaçant les caractères accentués par leur
///codes ASCII étendus correspondant
Feuille*
Function_Tab_Char_Spec(Feuille* tab,int* size_tab)
{
    Feuille* ptr_tab = (Feuille*)malloc(sizeof(Feuille));

    ptr_tab = tab;

    for(int i = 0; i < *size_tab;i++)
    {
        switch(ptr_tab[i].ASCII_code)
        {
            case -110:ptr_tab[i].ASCII_code = 146;break;//'
            case -30:ptr_tab[i].ASCII_code = 147;break;//ô
            case -23:ptr_tab[i].ASCII_code = 163;break;//é
            case -24:ptr_tab[i].ASCII_code = 138;break;//è
            case -32:ptr_tab[i].ASCII_code = 133;break;//à
            case -25:ptr_tab[i].ASCII_code = 135;break;//ç
            case -10:ptr_tab[i].ASCII_code = 148;break;//ö
            case -17:ptr_tab[i].ASCII_code = 139;break;//ï
            case -22:ptr_tab[i].ASCII_code = 136;break;//ê
            case -18:ptr_tab[i].ASCII_code = 140;break;//î
            case -12:ptr_tab[i].ASCII_code = 147;break;//ô
            case -7 :ptr_tab[i].ASCII_code = 151;break;//ù
        }
    }

    return ptr_tab;
}


///Fonction qui tri le tableau de fréquence des charactère
///par ordre croissant en fonction de la fréquence de chaque
///charactère
Feuille*
Tri_Freq_Tableau(Feuille* tab)
{
    Feuille* ptr_tab;
    ptr_tab = tab;
    int passage = 0;
    bool permute = false;

    //Tri bulle
    do{
        permute = false;

        for(int i = 0; i < size_tab_freq -1 - passage;i++)
        {
            if(ptr_tab[i].frequence > ptr_tab[i+1].frequence){
                Feuille tmp = ptr_tab[i];
                ptr_tab[i] = ptr_tab[i+1];
                ptr_tab[i+1] = tmp;

                permute = true;
            }
        }

        passage++;

    }while(permute == true);

    return ptr_tab;
}


///Fonction qui prend en paramètre le tableau de feuilles
///qui le tri par ordre de charactères
///Et qui retourne le tableau trié
Feuille*
Tri_ASCII_Tableau(Feuille* tab, int* size_tab)
{
    Feuille* ptr_tab = (Feuille*)malloc(sizeof(Feuille));
    ptr_tab = tab;
    int passage = 0;
    bool permute = false;

    //Tri bulle
    do{
        permute = false;

        for(int i = 0; i < *size_tab - 1 - passage;i++)
        {
            if(ptr_tab[i].ASCII_code > ptr_tab[i+1].ASCII_code){
                Feuille tmp = ptr_tab[i];
                ptr_tab[i] = ptr_tab[i+1];
                ptr_tab[i+1] = tmp;

                permute = true;
            }
        }

        passage++;

    }while(permute == true);

    return ptr_tab;
}


///Permet de calculer la fréquence de chaque charactères
///Dans le tableau et d'implémenter la partie de la Feuille
///avec les bonnes valeurs
Feuille*
Genere_Fils_Huffman(Feuille* tab, int* size_tab)
{
    Feuille* ptr_tab;
    ptr_tab = tab;
    size_tab_freq = 1;
    Feuille feuille;

    //Le dernier charactère du tableau
    feuille.ASCII_code = ptr_tab[0].ASCII_code;
    feuille.character = ptr_tab[0].character;
    feuille.frequence = 0;
    feuille.code_binaire = false;
    feuille.predecesseur_D = NULL;
    feuille.predecesseur_G = NULL;

    Feuille* ptr_tab_freq =  new Feuille[size_tab_freq]{feuille};
    Feuille* copy_ptr_tab_freq =  ptr_tab_freq;
    feuille.adresse = ptr_tab_freq;
    feuille.successeur = NULL;

    for (int i = 0 ; i < *size_tab;i++)
    {
        for (int o = 0; o < size_tab_freq;o++)
        {
            if(o < size_tab_freq && ptr_tab_freq[o].ASCII_code == ptr_tab[i].ASCII_code)
            {
                ptr_tab_freq[o].frequence++;
                delete [] copy_ptr_tab_freq;
                Feuille* copy_ptr_tab_freq =  ptr_tab_freq;
                break;
            }

            else if(o == size_tab_freq - 1 && ptr_tab_freq[o].ASCII_code != ptr_tab[i].ASCII_code)
            {
                delete [] copy_ptr_tab_freq;
                Feuille* copy_ptr_tab_freq =  ptr_tab_freq;
                delete [] ptr_tab_freq;
                size_tab_freq++;
                Feuille* ptr_tab_freq =  new Feuille[size_tab_freq];
                ptr_tab_freq = copy_ptr_tab_freq;

                //On crée la nouvelle feuille à la dernière place du tableau de fréquence
                //et on ajoute le nouveau charactère
                ptr_tab_freq[size_tab_freq-1].ASCII_code = tab[i].ASCII_code;
                ptr_tab_freq[size_tab_freq-1].successeur = NULL;
                ptr_tab_freq[size_tab_freq-1].character = tab[i].character;
                ptr_tab_freq[size_tab_freq-1].code_binaire = false;
                ptr_tab_freq[size_tab_freq-1].frequence = 0;
                ptr_tab_freq[size_tab_freq-1].predecesseur_D = NULL;
                ptr_tab_freq[size_tab_freq-1].predecesseur_G = NULL;
                ptr_tab_freq[size_tab_freq-1].adresse = &ptr_tab_freq[size_tab_freq-1];
                ptr_tab_freq[size_tab_freq-1].Is_Visited = false;
            }
        }
    }

    //Ajout du charactère de fin de texte
    delete [] copy_ptr_tab_freq;
    Feuille* copy_ptr_tab_freq2 =  ptr_tab_freq;
    delete [] ptr_tab_freq;
    size_tab_freq++;
    Feuille* ptr_tab_freq2 =  new Feuille[size_tab_freq];
    ptr_tab_freq2 = copy_ptr_tab_freq2;

    //On crée la nouvelle feuille à la dernière place du tableau de fréquence
    //et on ajoute le charactère de fin de
    ptr_tab_freq2[0].successeur = NULL;//premier élément de la liste rentré tout à l'heure, un petit ajustement...
    ptr_tab_freq2[0].adresse = ptr_tab_freq2;//On remet les pendules à l'heure pour le premier élément de la liste
    ptr_tab_freq2[size_tab_freq-1].ASCII_code = 4;
    ptr_tab_freq2[size_tab_freq-1].successeur = NULL;
    ptr_tab_freq2[size_tab_freq-1].character = (char)4;
    ptr_tab_freq2[size_tab_freq-1].frequence = 1;
    ptr_tab_freq2[size_tab_freq-1].code_binaire = false;
    ptr_tab_freq2[size_tab_freq-1].predecesseur_D = NULL;
    ptr_tab_freq2[size_tab_freq-1].predecesseur_G = NULL;
    ptr_tab_freq2[size_tab_freq-1].adresse = &ptr_tab_freq[size_tab_freq-1];
    ptr_tab_freq2[size_tab_freq-1].Is_Visited = false;

   //on tri un petit coup !
    Tri_Freq_Tableau(ptr_tab_freq2);

    return ptr_tab_freq2;
}


///Permet d'afficher le tableau de liste
void
Affichage_Liste(list<Feuille>* Liste)
{
    string Is_True = "false";
     cout << "\n\n\n";
       //Affichage liste
        cout << "AFFICHAGE LISTE"<<endl;
        list<Feuille>::iterator iter = Liste->begin();
        while(iter != Liste->end())
        {
           //Affichage_Node(&(*iter));
            if(iter->Is_Visited == 0){ Is_True = "false";}else {Is_True = "true";}
            cout << "IsVisit : " << Is_True << "\tCod bin : " << iter->code_binaire << "\tchar : " << (char)iter->ASCII_code << "\tASCII : " << iter->ASCII_code << "\tfreq : " << iter->frequence  << "\tsucc : " << iter->successeur << "\tAdress : " << iter->adresse << "\tpred G : " << iter->predecesseur_G << "\t  pred D : " << iter->predecesseur_D << "\n";
            iter++;
        }
      cout << "\n\n\n";
}


///Permet d'afficher le tableau de liste
void
Affichage_Table(Feuille* tab)
{
    cout << "------------------TABLEAU FREQUENCES TRIES----------------- \n";

    cout << endl<<endl<<endl;

    for(int i = 0; i < size_tab_freq;i++)
      {
           cout <<  "  Code binaire : " << tab[i].code_binaire << "   character : " << (char)tab[i].ASCII_code << " : ASCII : " << tab[i].ASCII_code << " : frequence : " << tab[i].frequence << ":  successeur : " << tab[i].successeur << "  Adresse : " << tab[i].adresse << "   predecesseur G : " << tab[i].predecesseur_G << "   predecesseur D : " << tab[i].predecesseur_D << "\n";
      }
     cout << endl<<endl<<endl;
}


///Permet d'afficher le contenu d'un noeud
void
Affichage_Node(Feuille* feuille)
{
    cout << "-------CONTENU DE LA FEUILLE----------\n";
    cout << endl << endl << endl;
    cout << "current node frequence : "             <<  feuille->frequence << endl;
    cout << "current adress pointer : "             <<  feuille->adresse << endl;
    cout << "current node adress successeur : "     <<  feuille->successeur << endl;
    cout << "current node ASCII : "                 <<  feuille->ASCII_code << endl;
    cout << "current node charactere : "            <<  feuille->character << endl;
    cout << "current node code binaire : "          <<  feuille->code_binaire << endl;
    cout << "current node is visited : "            <<  feuille->Is_Visited << endl;
    cout << "current node is left : "               <<  feuille->Is_Left << endl;
    cout << "current node predecesseur G : "        <<  feuille->predecesseur_G << endl;
    cout << "current node predecesseur D : "        <<  feuille->predecesseur_D << endl;
    cout << endl << endl << endl;
}


///Permet d'afficher le contenu d'un noeud Huffman
void
Affichage_Node_Huffman(Code_Huffman* node)
{
    cout << "-------CONTENU DU NOEUD HUFFMAN----------\n";
    cout << endl << endl << endl;

    cout << "current node ASCII : "                 <<  node->ASCII_code << endl;
    cout << "current node charactere : "            << (char)node->ASCII_code << endl;
    cout << "current node code binaire : "          <<  node->Binary_code << endl;
    cout << "current node code Huffman : "          <<  node->Huffman_code << endl;

    cout << endl << endl << endl;
}


///Procédure qui affiche le contenu de la liste des codes Huffman
void
Affichage_Code_Huffman(list<Code_Huffman>* list_code_huffman)
{
     list<Code_Huffman>::iterator iter = list_code_huffman->begin();

    while(iter != list_code_huffman->end())
    {
        if(iter->ASCII_code != 0)
        {
            string binary = bitset<8>(iter->ASCII_code).to_string(); //Conversion binaire
            cout << "Char : " << (char)iter->ASCII_code << "   Binary_code : " << iter->Binary_code << "   Huffman_code :  " <<  iter->Huffman_code << "\n";
        }
        iter++;
    }
}


///Fonction qui retourne une nouvelle feuille dans une liste chainée
Feuille*
Create_Node(Feuille* new_feuille,Feuille* adresse_G,Feuille* adresse_D,int *somme_freq)
{
    new_feuille->predecesseur_G = adresse_G;
    new_feuille->predecesseur_D = adresse_D;
    new_feuille->character = '\0';
    new_feuille->successeur = NULL;
    new_feuille->ASCII_code = 0;
    new_feuille->code_binaire = false;
    new_feuille->frequence = *somme_freq;
    new_feuille->Is_Left = true;
    new_feuille->Is_Visited = false;//Sera inclu dans le prochain tri MIN
    new_feuille->adresse = new_feuille;

   return new_feuille;
}


///Fonction qui retoune le plus petit élément de 2 éléments comparés par rapport à leur fréquences respectives
bool
 Compare_Smaller_Than(Feuille& noeud1, Feuille& noeud2)
{
    return noeud1.frequence < noeud2.frequence;
}


///Fonction qui cherche et remplace un noeud dans la liste de Huffman
list<Feuille>*
 Replace_node_Liste(Feuille* node,list<Feuille>* list_huffman)
{
    list<Feuille>::iterator iter = list_huffman->begin();

    while(iter != list_huffman->end())
    {
        if(node->ASCII_code == iter->ASCII_code && node->frequence == iter->frequence)
        {
            *iter = *node;
        }

        iter++;
    }

    return list_huffman;
}


///Fonction qui prend la liste de noeuds avec les noeuds visités et non visités et retourne une nouvelle liste
///avec uniquement les noeuds non visités ordonnée par ordre croissant
list<Feuille>*
 Genere_Arbre_Huffman(Feuille* tab,list<Feuille>* list_huffman)
{
    list<Feuille> list_tmp;
    list<Feuille>* ptr_list_tmp = &list_tmp;

    //Parcours de tab, et remplissage des 2 listes
    for(int i = 0; i < size_tab_freq;i++)
    {
        list_huffman->push_front(tab[i]);
    }

    //Inititialisation des adresses de chaque noeud dans la liste Huffman (adresses de réferences)
    list<Feuille>::iterator itera = list_huffman->begin();
    while(itera != list_huffman->end())
    {
        itera->adresse = &(*itera);
        ptr_list_tmp->push_back(*itera);
        itera++;
    }

    while(ptr_list_tmp->size() > 1)
    {
        //Tri liste tmp ordre croissant des 2 listes
        list_huffman->sort(Compare_Smaller_Than);
        ptr_list_tmp->sort(Compare_Smaller_Than);


        //2 itérateurs : 1 pour le 1er élément plus petit, 1 pour le 2eme élément plus petit
        list<Feuille>::iterator iter = ptr_list_tmp->begin();
        list<Feuille>::iterator iter2 = iter++;

        //Addition des 2 frequences des +petits éléments de la liste
        int somme_freq  = 0;
        somme_freq = iter->frequence + iter2->frequence;

        //Création d'un nouvelle feuille "Noeud" avec la somme des 2 fréquences
        Feuille* prt_new_feuille = new Feuille();
        Create_Node(prt_new_feuille,iter->adresse,iter2->adresse,&somme_freq);


        iter->Is_Visited = true;
        iter->successeur = prt_new_feuille->adresse;
        iter->code_binaire = 1;
        iter2->Is_Visited = true;
        iter2->successeur = prt_new_feuille->adresse;
        iter2->code_binaire = 0;

        //Remplacement des anciens noeuds par les 2 nouveaux dans la liste de Huffman
        Replace_node_Liste(&(*iter),list_huffman);
        Replace_node_Liste(&(*iter2),list_huffman);

        //Ajout du nouveau noeud dans la liste TMP et dans la liste HUFFMAN
        ptr_list_tmp->push_back(*prt_new_feuille);
        list_huffman->push_back(*prt_new_feuille);

        //Suppression des 2 noeuds ayant servi à faire le calcul de la liste TMP
        ptr_list_tmp->pop_front();
        ptr_list_tmp->pop_front();
    }

    list_huffman->sort(Compare_Smaller_Than);

    return list_huffman;
}


///Procédure qui converti et affichage le code binaire d'un code ASCII donné
void
Affiche_Binary_Code(list<Feuille>* list_huffman)
{
    list<Feuille>::iterator iter = list_huffman->begin();

    cout << "\n\n\n";

    while(iter != list_huffman->end())
    {
        if(iter->ASCII_code != 0)
        {
            string binary = bitset<8>(iter->ASCII_code).to_string(); //Conversion binaire
            cout << "ASCII : \t" << iter->ASCII_code << "   :   "  << "Binaire : \t " << binary << "\n";
        }
        iter++;
    }

    cout << "\n\n\n";
}


///Affiche le contenu de la liste Huffman fonction de l'adresse et des successeurs de liste chainées
void
Affiche_code_Huffman_list(list<Feuille>* list_huffman)
{
    list<Feuille>::iterator iter = list_huffman->begin();

    cout << "ICI DEBUT DU PARCOURS DE LA LISTE PAR POINTEUR \n\n\n";
    while(iter != list_huffman->end())
    {
        Feuille* pointeur = &(*iter);
        while(pointeur)
        {
           // Affichage_Node(pointeur);
            cout << "adresse node : " << pointeur->adresse <<  " : " << "adresse successeur node : " << pointeur->successeur << "\n";
            pointeur = pointeur->successeur;
        }

        iter++;
    }

    cout << "\n\n\n";
}


///Function qui retourne la liste des nouveaux codes Huffman après avoir parcouru chaque feuille
///jusqu'à la racine de l'arbre de Huffman.
list<Code_Huffman>*
Genere_List_Code_Huffman(list<Feuille>* list_huffman, list<Code_Huffman>* list_code_huffman)
{
    //On parcours la liste des noeuds
    list<Feuille>::iterator iter = list_huffman->begin();

    while(iter != list_huffman->end())
    {
        //Pointeur sur l'iterateur courant
        Feuille* ptr_current_node = NULL;

        if(iter->ASCII_code != 0)
        {
            ptr_current_node = &(*iter);

            cout << "contenu du pointeur courant : \n";
            Affichage_Node(ptr_current_node);
            cout << "Adresse pointeur courant en dehors de la boucle : " << &ptr_current_node << endl;
            cout << "adresse successeur : " << ptr_current_node->successeur << endl;

            //Création d'un nouvelle feuille "Noeud" avec la somme des 2 fréquences
            Code_Huffman* prt_new_code = new Code_Huffman();
            prt_new_code->ASCII_code = ptr_current_node->ASCII_code;
            prt_new_code->Binary_code = bitset<8>(ptr_current_node->ASCII_code).to_string();

            while(ptr_current_node->successeur != 0)
            {
                cout << "NOEUD DANS LA BOUCLE \n";

                Affichage_Node(ptr_current_node);
                cout << "Adresse successeur : " << &ptr_current_node->successeur << endl;

                cin.get();
                cin.ignore();
                if(ptr_current_node->code_binaire == true)
                {
                    prt_new_code->Huffman_code += "1";
                }

                else
                {
                    prt_new_code->Huffman_code += "0";
                }


                 //On parcours la liste des noeuds jusqu'à trouver l'adresse du successeur
                list<Feuille>::iterator iter2 = list_huffman->begin();

                while(iter2 != list_huffman->end())
                {
                    if(iter2->adresse == ptr_current_node->successeur)
                    {
                      ptr_current_node = &(*iter2);
                      break;
                    }
                    else
                    iter2++;
                }
            }
              prt_new_code->Huffman_code += "0";
              cout << "NOEUD CREE : \n";
              Affichage_Node_Huffman(prt_new_code);
            list_code_huffman->push_back(*prt_new_code);
        }
        iter++;
    }

    return list_code_huffman;
}


///Main
int
 main()
{
    //setlocale( LC_ALL , "french" );
    string path_file = "\\ProjetsC++\\Compression_Huffman";
    string file_name = "test3.txt";
    Feuille feuille;
    ifstream inFile;
    char singleCharacter;
    list<Feuille> List_Character;

    inFile.open(path_file + "\\" + file_name);

    inFile.get(singleCharacter);

    while(!inFile.eof())
    {
        feuille.character = singleCharacter;
        feuille.ASCII_code = (int)singleCharacter;
        feuille.frequence = 0;
        List_Character.push_front(feuille);
        inFile.get(singleCharacter);
    }

    List_Character.reverse();

    list<Feuille>::iterator iter = List_Character.begin();
    int nbr_elem_liste = 0;

    while(iter != List_Character.end())
    {
        iter++;
        nbr_elem_liste++;
    }

    Feuille tableau_frequence[nbr_elem_liste];
    Feuille *ptr_tableau_frequence = tableau_frequence;

    int compteur = 0;

    list<Feuille>::iterator iter2 = List_Character.begin();

    while(iter2 != List_Character.end())
    {
        tableau_frequence[compteur] = *iter2;
        compteur++;
        iter2++;
    }

    Tri_ASCII_Tableau(tableau_frequence,&nbr_elem_liste);

    Function_Tab_Char_Spec(tableau_frequence,&nbr_elem_liste);

    Tri_ASCII_Tableau(tableau_frequence,&nbr_elem_liste);

    Feuille* tab_freq = new Feuille[size_tab_freq];//Nouveau pointeur sur tableau de fréquences avec la taille statique

    tab_freq = Genere_Fils_Huffman(tableau_frequence,&nbr_elem_liste);

    //tri de la nouvelle liste, avec les éléments Nodes et Fils
    Tri_Freq_Tableau(tab_freq);

    list<Feuille> list_huffman ;
    Genere_Arbre_Huffman(tab_freq,&list_huffman);

    Affichage_Liste(&list_huffman);

    //Liste qui reçoit les codes ASCII, les codes binaires, et les codes Huffman des charactères
    list<Code_Huffman> list_code_huffman;
    list<Code_Huffman>* ptr_list_code_huffman = & list_code_huffman;

    Genere_List_Code_Huffman(&list_huffman,ptr_list_code_huffman);

    //Affiche_Binary_Code(&list_huffman);

    //Affiche le resultat final (caractère, code binaire, code Huffman)
    Affichage_Code_Huffman(ptr_list_code_huffman);

    delete[] tab_freq;

    cin.ignore();
    cin.get();
}
