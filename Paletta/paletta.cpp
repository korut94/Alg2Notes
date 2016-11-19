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

long long paletta_sort(int N, int V[])
{
  int minimo = 0, // Posizione del piu' piccolo elemento nell'array
    ordinato = 0, // Attuale lunghezza del sotto array ordinato
    // Numero di ribalta da compiere per riportare un valore alla sua posizione
    nRibalta = 0,
    distanza = 0; // Distanza tra la posizione del minimo e la sua cella
  long long contatore = 0; // Somma dei ribalta eseguiti
  bool ordinabile = true; // Flag se l'array e' ordinabile attraverso paletta

  // Andiamo avanti fiche' e' ordinabile oppure non l'abbiamo ordinato tutto
  while (ordinabile && ordinato < N) {
    // Artimetica dei puntatori: min_element da l'iteratore con posizione del
    // valore minimo in V. Dato che posso trattare l'iteratore come puntatori
    // gli sottraggo il punto di partenza di V cosi da ottenere la posizione
    // del minimo.
    //
    // `V + ordinato` impedisce di andare a scansionare il sotto array
    // ordinato.
    minimo = std::min_element(V + ordinato, V + N) - V;
    // Distanza tra il minimo e dove dovrebbe risiedere. Il valore assoluto non
    // serve perche' `minimo` sara' sempre maggiore o uguale ad `ordinato`.
    distanza = minimo - ordinato;
    
    // Essendo il minimo ovviamente deve stare nella posizione piu' in fondo
    // all'array che e' esattamente il valore di ordinato.
    // Tuttavia la paletta permette di spostarsi solo di -2 a sinitra o +2 a
    // destra, percio' nel caso che la distanza non fosse un multiplo di 2
    // non sarebbe possibile ordinare l'array.
    if (distanza % 2 == 0) {
      // Quante palette sono state applicate per portare il minimo dove e'
      // stato trovato.
      nRibalta = distanza / 2;
      // Accumula i ribalta applicati
      contatore += nRibalta;
      // Effettua tanti ribalta quanti ne sono serviti per spostare il minimo
      for (int i = 0; i < nRibalta; i++) {
	// L'operazione di swap e' l'effetto della paletta
	std::swap(V[minimo-2], V[minimo]);
	// Aggiorna la posizione del minimo
	minimo -= 2;
      }

      // Un elemento e' stato portato nella posizione corretta
      ordinato += 1;
    } else {
      ordinabile = false;
      // Ordinamento fallito
      contatore = -1;
    }
  }

  return contatore;
}
