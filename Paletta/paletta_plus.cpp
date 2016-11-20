// TODO: Manca calcolare la somma

/**
 * Soluzione Paletta utilizzando la conta delle inverse
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

#include <climits>

#define INF INT_MAX

// Sancisce la parita' degli indici: 1 se pari, 0 se dispari.
int pari = 1;
int dispari = 0;

// Sposta il puntatore p di i*2 posizioni. Se il vettore p punta alla posizione
// dispari dell'array, la funzione next dara' la prossima i-esima posizione
// dispari, se la posizione e' pari il discorso e' analogo.
int * next(int *p, int i)
{
  return p + i*2;
}

void merge(int *V, const int p, const int q, const int r, int *inv, int *sum)
{
  // Dimensione del sotto array sinitro verso q
  int sinistro = q - p + dispari;
  // Dimensione del sotto array destro verso q
  int destro = r - q + pari;
  // Array di supporto, la dimensione deve supportare la sentinella finale.
  int L[sinistro+1], R[destro+1];
  // Indici generici utili nelle varie fasi dell'algoritmo
  int i = 0, j = 0;
  
  // Applica la swap inserendo in L la parte sinistra dell'array ed in R la
  // parte destra. Nella fase di inizializzaione e' possibile calcolarsi
  // direttamente la somma.
  for (; i < sinistro; i++) {
    L[i] = *(next(V, p+i) - dispari);
  }

  for (; j < destro; j++) {
    R[j] = *(next(V, q+j) + dispari);
  }

  // Sentinella. Impedira' agli indici dichiarati dopo (i e j) di uscire dai
  // limiti del primo array in cui tutti gli elementi sono gia' stati
  // posizionati in V.
  L[sinistro] = R[destro] = INF;
  
  i = 0;
  j = 0;

  int lunghezza = sinistro + destro;
  
  // Esecuzione del merge dei due array
  for (int k = p; k < lunghezza; k++) {
    if (L[i] <= R[j]) {
      *(next(V,k)) = L[i];
      i += 1;
    } else {
      *(next(V,k)) = R[j];
      j += 1;

      // La sentinella introduce dei falsi positivi
      if (L[i] != INF) {
	*inv = *inv + sinistro - i;
      }
    }
  }
}

// Merge sort modificato per il problema. Gli indici p ed r sono le posizioni
// valide per l'array.
void mergeInvSum(int *V, const int p, const int r, int *inv, int *sum)
{
  int q = 0; // Punto di divisione dell'array

  if (p < r) {
    q = (p + r) / 2; // Divisione dell'array
    mergeInvSum(V, p, q-pari, inv, sum);
    mergeInvSum(V, q+dispari, r, inv, sum);
    // Combinazione dei due sotto array
    merge(V, p, q, r, inv, sum);
  }
}

// Tramite side-effect ritorna il numero d'inversioni e la somma degli elementi
// degli array costruiti tramite gli elementi nelle posizioni pari e dispari di
// V.
void InvSum(int *V,
	    const int N,
	    int *inv_pari,
	    int *inv_dispari,
	    int *sum_pari,
	    int *sum_dispari)
{
  // Array posizioni pari di V
  int *Vp = V;
  // Array posizioni dispari di V
  int *Vd = V + 1;
  // Dimensione dell'array pari e dispari
  int Np = 0, Nd = 0;
  
  if (N % 2 == 0) {
    Np = N / 2;
    Nd = N - Np;
  } else {
    Nd = (N - 1) / 2;
    Np = N - Nd;
  }
  
  mergeInvSum(Vp, 0, Np-1, inv_pari, sum_pari);

  pari = 0;
  dispari = 1;
  
  mergeInvSum(Vd, 1, Nd, inv_dispari, sum_dispari);
}

long long paletta_sort(int N, int V[])
{
  int sum_pari = 0, sum_dispari = 0,
      inv_pari = 0, inv_dispari = 0;

  // Ottieni le informazioni valide per determinare le informazioni del
  // problema.
  InvSum(V, N, &inv_pari, &inv_dispari, &sum_pari, &sum_dispari);
  // Aggiungi commenti
  return (sum_dispari < sum_pari) ? -1 : inv_pari + inv_dispari;
}
