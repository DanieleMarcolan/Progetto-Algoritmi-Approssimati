#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <bits/stdc++.h>

#include "endgame.h"

using namespace std;

#define INF INT_MAX

ifstream in("input.txt");
ofstream out("output.txt");

//input
int numeroPietre;
double capacitaGuanto;
double R;
double VMin;
double VMax;        
double bounds;

//output
double energiaRaccolta;
double tempoImpiegato;

//strutture dati
struct Pietra {
    double massa;
    double energia;
    int inQuanteCitta;
    int *inQualiCitta;
    bool raccolta = false;
    int raccoltaDove = -1;
};
int **matriceAdiacenza;
int numeroCitta;
int cittaDiPartenza;
bool *visited;
vector<int> percorso;
Pietra *listaPietre;
vector<vector<int>> pietreInCitta;

void visita() {
  	energiaRaccolta = 0.0;
   	tempoImpiegato = 0.0;
    	
    double massaAttuale = 0.0;
    double velocitaAttuale = VMax;
    	
    percorso.push_back(cittaDiPartenza);
    int cittaAttuale = cittaDiPartenza;
        
    //cerca le pietre candidate per la città di partenza
    vector<Pietra*> candidate;
    for(int i : pietreInCitta[cittaAttuale]){
        if (massaAttuale <= capacitaGuanto && !listaPietre[i].raccolta && listaPietre[i].inQuanteCitta > 0) {
            if(listaPietre[i].massa + massaAttuale <= capacitaGuanto){
                candidate.push_back(&listaPietre[i]);
            }
        }
    }

    //seleziona la miglior pietra per la città di partenza
    int migliorRapporto = -INF;
    int massaPresa = -INF;
    int scelta = -1;
    for (int i = 0; i < candidate.size(); i++) {
    	int rapportoLocale = candidate[i] -> massa == 0 ? INF : candidate[i] -> energia / candidate[i] -> massa;
    	if (rapportoLocale > migliorRapporto) {
    		migliorRapporto = rapportoLocale;
    		massaPresa = candidate[i] -> massa;
    		scelta = i;
    	}
    	else if (rapportoLocale == migliorRapporto) {
    		if (candidate[i] -> massa < massaPresa) {
    			migliorRapporto = rapportoLocale;
    			massaPresa = candidate[i] -> massa;
    			scelta = i;
    		}
    	}
    }
    
	//raccoglie la miglior pietra, se ne è stata scelta una
    if (scelta > -1) {
   		candidate[scelta] -> raccoltaDove = cittaAttuale;
    	candidate[scelta] -> raccolta = true;
    	massaAttuale += candidate[scelta] -> massa;
   		// cout << "massa: " << massaAttuale << endl;
	   	energiaRaccolta += candidate[scelta] -> energia;
   		// cout << "energia: " << energiaRaccolta << endl;
    	velocitaAttuale = VMax - (double)(massaAttuale * bounds);
   		// cout << "velocita: " << velocitaAttuale << endl;
   		// cout << endl;
    }
    
    //visita tutto il grafo   		
    while(percorso.size() < numeroCitta) {
        visited[cittaAttuale] = true;
            
        //scelgo la prossima miglior città
     	int migliorCitta;
        int minLocale = INF;
        for(int i = 0; i < numeroCitta; i++) {
            if(!visited[i] && i != cittaAttuale) {
                if(matriceAdiacenza[cittaAttuale][i] < minLocale){
                    migliorCitta = i;
                    minLocale = matriceAdiacenza[cittaAttuale][i];
                }
            }
        }
        
        tempoImpiegato += velocitaAttuale == 0 ? INF : ((double)matriceAdiacenza[cittaAttuale][migliorCitta]) / velocitaAttuale;
        //cout << "tempo: " << tempoImpiegato << endl;
        
        //seleziona le pietre candidate per la prossima città
        candidate.clear();
        for(int i : pietreInCitta[migliorCitta]){
        	if (massaAttuale <= capacitaGuanto && !listaPietre[i].raccolta && listaPietre[i].inQuanteCitta > 0) {
        	    if(listaPietre[i].massa + massaAttuale <= capacitaGuanto){
        	        candidate.push_back(&listaPietre[i]);
        	    }
        	}
    	}

    	//sceglie la pietra migliore fra le possibili
    	int migliorRapporto = -INF;
	    int massaPresa = -INF;
   		int scelta = -1;
    	for (int i = 0; i < candidate.size(); i++) {
    		int rapportoLocale = candidate[i] -> massa == 0 ? INF : candidate[i] -> energia / candidate[i] -> massa;
    		if (rapportoLocale > migliorRapporto) {
    			migliorRapporto = rapportoLocale;
    			massaPresa = candidate[i] -> massa;
    			scelta = i;
    		}
    		else if (rapportoLocale == migliorRapporto) {
    			if (candidate[i] -> massa < massaPresa) {
    				migliorRapporto = rapportoLocale;
    				massaPresa = candidate[i] -> massa;
    				scelta = i;
    			}
    		}
    	}	

    	//raccoglie la pietra, se ne ha scelta una
    	if (scelta > -1) {
   			candidate[scelta] -> raccoltaDove = migliorCitta;
    		candidate[scelta] -> raccolta = true;
    		massaAttuale += candidate[scelta] -> massa;
    		// cout << "massa: " << massaAttuale << endl;
    		energiaRaccolta += candidate[scelta] -> energia;
    		// cout << "energia: " << energiaRaccolta << endl;
    		velocitaAttuale = VMax - (double)(massaAttuale * bounds);
    		// cout << "velocita: " << velocitaAttuale << endl;
    		// cout << endl;
    	}
        
        percorso.push_back(migliorCitta);
        cittaAttuale = migliorCitta;
    }
        
    //aggiunge l'arco di ritorno al nodo di partenza
    percorso.push_back(cittaDiPartenza);
    tempoImpiegato += velocitaAttuale == 0 ? INF : ((double)matriceAdiacenza[cittaAttuale][cittaDiPartenza]) / velocitaAttuale;
    //cout << "TEMPO TOTALE: " << tempoImpiegato << endl;
}

void stampaSoluzione() {
    out << scientific << setprecision(10) << energiaRaccolta - (R * tempoImpiegato) << " ";
    //cout << energiaRaccolta - (R * tempoImpiegato) << " ";
    out << scientific << setprecision(10) << energiaRaccolta << " ";
    //cout << energiaRaccolta << " ";
    out << scientific << setprecision(10) << tempoImpiegato << endl;
    //cout << tempoImpiegato << endl;
    
    for(int i = 0; i < numeroPietre; i++) {
        out << listaPietre[i].raccoltaDove << " ";
    }
    out << endl;
    
    for(int i = 0; i < percorso.size(); i++) {
        out << percorso[i] << " ";
    }
    out << endl;
    
    out << "***" << endl;
    
    out.flush();
}

int main() {
    in >> numeroCitta >> cittaDiPartenza;
    
	visited = new bool[numeroCitta];
	for (int i = 0; i < numeroCitta; i++) {
    	visited[i] = false;
	}

    in >> numeroPietre >> capacitaGuanto >> R >> VMin >> VMax;
    
    bounds = (double)capacitaGuanto == 0 ? 0  : ((double)(VMax - VMin)) / ((double)capacitaGuanto);

    listaPietre = new Pietra[numeroPietre];

    //crea le pietre
    for (int i = 0; i < numeroPietre; i++) {
        Pietra p;
        in >> p.massa >> p.energia;
        listaPietre[i] = p;
    }
	
    //posiziona le pietre nelle citta
    pietreInCitta.resize(numeroCitta);
    for (int i = 0; i < numeroPietre; i++) {
        in >> listaPietre[i].inQuanteCitta;
        listaPietre[i].inQualiCitta = new int[listaPietre[i].inQuanteCitta];
        for (int j = 0; j < listaPietre[i].inQuanteCitta; j++) {
            int citta;
            in >> citta;
            listaPietre[i].inQualiCitta[j] = citta;
            pietreInCitta[citta].push_back(i); //mette la pietra i-esima nelle citte giuste
        }
    }

    //crea la matrice di adiacenza
    matriceAdiacenza = new int* [numeroCitta];
    for (int i = 0; i < numeroCitta; i++) {
        matriceAdiacenza[i] = new int[numeroCitta];
        for (int j = 0; j < i; j++) {
            int tmp;
            in >> tmp;
            matriceAdiacenza[i][j] = tmp;
            matriceAdiacenza[j][i] = tmp;
        }
        matriceAdiacenza[i][i] = INF;
    }
	
	visita();
    
    stampaSoluzione();
    
    return 0;
}
