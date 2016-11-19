/**
 * Soluzione Hogwarts senza implementazione della coda di priorita'
 * Copyright (c) 2016 Andrea Mantovani
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

typedef MinQueue std::priority_queue<int,
				     std::vector<int>,
				     std::greater<int> >;

#define INF INT_MAX

int raggiungi(int N, int M, int A[], int B[], int inizio[], int fine[])
{
  int attuale = 0; // rappresenta il nodo attualmente visitato
  int destinazione = N-1;
  int t = 0; // istante di tempo dalla partenza alla destinazione
  int distanza[N];
  bool raggiunto[N];
  std::vector<int> adjacencyMatrix[N];

  // Build the rappresentaiton of the graph
  for (int i=0, i < M, i++) {
    // Dato il passaggio successivo, alcuni nodi non sono segnati nella matrice
    // A, percio' bisogna salvare la lora destinazione insieme all'indice della
    // scala.
    adjacencyMatrix[A[i]].push_back(std::make_pair(i,B[i]));
    // Questo inserimento e' per creare un grafo non-direzionato. Uno puo' sia
    // salire che scendere.
    adjacencyMatrix[B[i]].push_back(std::make_pair(i,A[i]));
    // Inizializzazione dei nodi raggiunti.
    // Primo ogni nodo e' bianco quindi ancora non e' stato visitato
    raggiunto[A[i]] = raggiunto[B[i]] = false;
    // Infine ogni nodo lo si raggiunge con una distanza di "infinito" perche'
    // non esiste ancora un cammino a partire da un nodo che conduce ad esso.
    distanza[A[i]] = distanza[B[i]] = INF;
  }
  
  MinQueue q;

  // Inizializzazione del punto di partenza
  distanza[0] = 0;

  while (attuale != destinazione) {
    raggiunto[attuale] = true;
    // L'algoritmo di Dijkstra configuare la minima distanza dei nodi. Percio'
    // se sono arrivato al nodo attuale ci ho messo la distanza fissata.
    t = distanza[attuale];
    
    for (auto scala : adjacencyGraph[attuale]) {
      // Sole se non ho gia' un percorso minimo per il mio vicino
      if (!raggiunto[scala.second]) {
	// Aggiorna la distanza.
	// Calcolanda il massimo tra 0 e inizio[i] - t si risparmia l'if di
	// controllo se t e' minore o maggiore di inzio. Nel caso t <, ovvero
	// bisogna aspettare la comparsa della scala, la sottrazione diventera'
	// positiva e quindi la distanza aumenta. Al contrario se fosse t > allora
	// la sottrazione diventa negativa e quindi e' corretto prendere 0 come
	// tempo d'attesa. Il piu' 1 finale serve per contare comunque il consumo
	// d'unita' per spostarsi da una scala all'altra.
	//
	// Rapitolando:
	// distanza = tempAttesaMin + tempRaggiuntaNodo + tempScala
	// ovviamente nel caso si possa ancora percorrerla.
	distanza[scala.second] = (t >= fine[scala.first]) ? INF :
	  std::max(0, inizio[scala.first] - t) + t + 1;
	
	q.push(scala.second);
      }
    }

    attuale = q.pop();
  }

  // Quantita' di tempo per arrivare alla destinazione
  t = distanza[attuale];
  
  return (t == INF) ? -1 : t;
}
