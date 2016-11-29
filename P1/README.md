# Struttura di supporto per intervalli
Dato un array C di dimensione N inizialmente riempito con 0, l'obiettivo della
nostra struttura dati e' supportare tre operazioni: *update*, *sum* e *query*
con i vincoli di spazio e tempo.

## Vincoli spazio
La struttura dati deve occupare `O(N)`.

## Vincoli di tempo
La costruzione della struttura deve essere completata in `O(N*lg(N))`, e le
operazioni citate sopra deve essere effettuate in `O(lg(N))` ciascuna.

## Operazioni
### update(i,j,c)
La funzione opera sull'array `C` e incrementa tutte le posizioni da `i` a `j`
di `c`, con `0 <= i <= j <= N-1`, ovvero: `C[k] = C[k] + c` per ogni
`i <= k <= j`.

### sum(i,j)
Ritorna la somma dell'intervallo `C[i...j]` con `0 <= i <= j <= N-1`.

### query(i)
Ritorna la posizione di `C[i]`, con `0 <= i <= N-1`.

# Risoluzione
**Osservazione - 1**: 
> Data la richiesta di compiere le operazioni in `O(lg(N))` conviene utilizzare
una struttura ad albero binario bilanciato. 

**Osservazione - 2**:
> L'albero `T` per soddisfare il vincolo in spazio deve avere al piu' `N` nodi.

**Osservazione - 3**:
> Dato l'array `C` di dimensione `N` il numero d'intervalli e' massimo quando
questi hanno dimensione unitaria, ovvero ogni cella dell'array ha numeri
diversi.

Per **2 + 3** posso dire che i nodi o rappresentano gli intervalli o i valori
presenti nell'array. La **1** mette in luce che manca la condizione di
partizionamento dell'albero binario, il qui puo' essere scelta o usando i
valori o gli intervalli.

**Osservazione - 4**:
> L'uso dei valori come paramentro di partizionamento non garantisce il
bilanciamento dell'albero.

L'inserimento consecutivo di valori crescenti o decresenti causerebbe la
formazione di una coda, piombando il tempo per eseguire le operazioni a
`O(N)`.

Percio' per **4** i nodi rappresentano i valore dell'intervallo e si va ad
utilizzare gli intervalli per partizionare l'albero in modo che a sinistra
vadano gli intervalli a sinistra dell'estremo sinistro del nodo corrente e
a destra gli intervalli a destra dell'estremo destro.

```  
              < [i,j] > 
    < [l,k] >     |     < [m,n] >
< ... > | < ... > | < ... > | < ... >
```

Con le varie osservazioni effettuare possiamo gia' definire una struttura base
il cui occupa `O(N)` e esegue l'operazione di ricerca in `O(lg(N))`.

```C++
struct Nodo
{
	int    k;  // Valore dell'intervallo rappresentato

	int    ls, // Limite sinistro dell'intervallo
		   ld; // Limite destro dell'intervallo

	Nodo * sx, // puntatore al sottoalbero sinistro
		   dx; // puntatore al sottoalbero destro
};
```

**Nota**: tutte le implementazioni usano un pseudo codice con la sintassi del
*C++* ma possono mancare alcuni elementi per poterli eseguire (dichiarazione
di funzioni o variabili). In generale definisco `p` il nodo correntemente
puntato, `T` la radice dell'albero e `C` l'array del problema.

## Implementazione metodo *query*

L'algortimo della query risulta una immediata ricerca binaria, dove se `i` non
e' compreso nell'intervallo del nodo corrente `p` si continua la ricerca in base
se `i < p->ls` o `p->ld > i`. Dato che `0 <= i <= N-1` per ipotesi dell problema
non occorre guardare se `i` rimante all'interno dell'array.

```C++
int query(int i)
{
	if (p == 0) {
		return 0;
	} else if (p->ls >= i && i <= p->ld) { // Siamo dentro all'intervallo
		return p->k;
	} else if (i < p->ls) { // Intervallo esterno sinitro
		return query(i, p->sx);
	} else { // Intervallo esterno destro
		return query(i, p->dx);
	}
}
```

**Osservazione - 5**:
> Quando `p` o un suo figlio non sono inizializzati allora si sa che
`C[h] == 0` dove `h` e' la profondita' del nodo puntato da `p`.

Per **5** allora si ha:
```C++
// Metodo query V1
int query(int i, Nodo *p)
{
	// Struttura dati vuota
	if (p == 0) {
		return 0;
	} else if (p->ls >= i && i <= p->ld) { // Siamo dentro all'intervallo
		return p->k;
	} else if (i < p->ls) { // Intervallo esterno sinitro
		return (p->sx) ? query(i, p->sx) : 0;
	} else { // Intervallo esterno destro
		return (p->dx) ? query(i, p->dx) : 0;
	}
}
```
Essendo che si sceglie solo un percorso predefinito la complessita' e'
`O(B)` dove `B` e' la lunghezza del percorso. Essendo l'albero bilanciato
(non ancora dimostrato) `B ~= lg(N)` e quindi il tempo per eseguire
*query(_V1)* diventa `O(lg(N))`.


Sempre per **5** abbiamo l'algortimo per inizializzare la nostra struttura
dati:
```C++
Nodo * costruisci()
{
	// Array di soli 0
	return 0;
}
```
Si noti che e' manteniamo la proprieta' che il nostro albero sia bilanciato
oltre ai vincoli di spazio e tempo richiesti.

```C++
int distanza(Intervallo i, int centro) {
  return (i / 2) - centro;
}

// sez e' la sezione del sottoarray che sto considerando
Nodo * update(Intervallo i, Nodo *p, Sezione sez)
{
  // Caso (0) Nessun intervallo
  if (p == 0) {
    return new Nodo(i.c, i.s, i.d);
    // Caso (1) Aggiornamento sull'intero intervallo
  } else if (p->ls == i.s && p->ld == i.d) {
    p->k += i.c;
    return p;
    // Caso (2) Dentro all'intervallo presente
  } else if (p->ls >= i.s && i.d <= p->ld) {
    Insieme<Intervallo> intervalli = {};

    // Inserisco l'intervallo considerato solo se la sua lunghezza e' > 0.
    // I due intervalli esterni non sono sicuro che esistano perche' se
    // l'aggiornamtno tocca gli estremi di p ma non entrambi si andranno a
    // creare due solo sottointervalli.
    intervalli.pushIfNotZero(Intervallo(p->ls, i.s-1, p->k));
    // La sezione centrale sara' sempre valida
    intervalli.push(Intervallo(i.s, i.d, p->k + i.c));
    intervalli.pushIfNotZero(Intervallo(i.d + 1, p->ld, p->k));

    Intervallo centro = min_distanza(intervalli, sez.centro());
    intervalli = intervalli - centro;
    // Inserisci nel nodo p tutte le informazioni relative alla sezione
    aggiorna(p, centro);

    // Esegui l'update per ogni intervallo dell'insieme. Questa chiamata
    // funziona piu' da dispatcher dato che non si sa come distribuire i
    // nodi rappresentanti i vari intervalli.
    for (Intervallo intervallo : intervalli) {
      update(intervallo, p, sez);
    }

    // Ritorno il nodo aggiornato
    return p;

    // Caso (3) Fuori dall'intervallo presente
    // Da notare che non si sa a priori in che modo l'aggiornamento vada fuori
    // dall'intervallo attuale.
  } else {
    Insieme<Intervallo> intervalli = {};

    // Ritorna l'intervallo [i.s,ls[. Se l'intervallo i e' piu' piccolo la
    // funzione tornera' l'intero intervallo i, se ((p->ls - 1) - i.s) < 0
    // tornera' l'intervallo nullo.
    intervalli.pushIfNotZero(i.subIntervallo(i.s, p->ls - 1));
    // Ritorna l'intervallo [ls,ld]
    Intervallo intC = i.subIntervallo(p->ls, p->ld);
    // Ritorna l'intervallo ]ld, i.d]
    intervalli.pushIfNotZero(i.subIntervallo(p->ld + 1, i.d));

    // Gestione intervalli fuoriuscenti da p
    for (Intervallo intervallo : intervalli) {
      Intervallo succ = intervallo;

      int distanzaInt = distanza(intervallo, sez.centro());
      int distanzaP = dinstanza(p, sez.centro());
      int distanzaSucc = distanzaInt;
      
      // L'intervallo e' piu' vicino al centro di quello attuale
      if (|distanzaInt| < |distanzaP|) {
	// Prendi tutte le informazioni da p in modo da costruire l'intervallo
	// che rappresenta.
	succ = p->intervallo();
	// Sostituisco l'intervallo di p con il nuovo intervallo
	aggiorna(p, intervallo);
	distanzaSucc = distanzaP;
      }

      if (distanzaSucc >= 0) {
	p->dx = update(succ, p->dx, sez.parteDestra());
      } else {
	p->sx = update(succ, p->sx, sez.parteSinistra());
      }
    }

    // Gestione del caso centrale
    if (!intC.isZero()) {
      // La chiamata ricorsiva ha termine perche' o e' gestita da (1) o da (2)
      update(intC, p, sez);
    }
    
    return p;
  }
}
```

La procedura di *update* aggiorna l'albero in modo corretto ma **non** l'ho
fa in `O(lg(N))`. Questo perche' abbiamo la necessita' nel caso l'intervallo
`[i,j]` comprendesse l'intervallo `[ls,ld]` di `p` di scendere ricorsivamente
nei sotto alberi sinistro e destro di `p`, portando inevitabilemente la
complessita' a `O(N)`.

**Osservazione - 6**:
> Il valore effettivo in una data posizione dell'array e' richiesto solo nel
caso in una *query* dove si vuole l'esatto valore della posizione i-esima dell'
array.

Applicando una procedura *Lazy* ad *update* si potra' portare il carico dell'
aggiornamento dell'array su *query* dove effettivaente diventa necessario
aggioranare la cella di cui si vuole conoscere il valore.

La questione ora e' capire dove salvare la quantita' che sarebbe dovuta essere
aggiunta da *update* ma pigramente lo lascia come onere a *query*. Come prima
idea potrebbe si potrebbe salvare nei nodi due interi `residuo` che indicano 
di quando c'e' da aggiungere ai sotto alberi adiacenti. Il risultato sarebbe
dunque:

```C++
struct Nodo
{
  ...
  int residuo_s, residuo_d; // Aggiunta dei residui da applicare ai sottoalberi
  ...
};
```

Modifichiamo *query* in modo che trasporti il residuo durante il suo percorso.

```C++
// Metodo query V2
int query(int i, Nodo *p)
{
  // Struttura dati vuota
  if (p == 0) {
    return 0;
  } else if (p->ls >= i && i <= p->ld) { // Siamo dentro all'intervallo
    // La procedura lazy evita di aggiornare i nodi dei sotto alberi ma
    // garanitisce di aggiornare il nodo corrente.
    return p->k;
  } else {
    Nodo *succ = (i < p->ls) ? p->sx : p->dx;
    int& residuo = (i < p->ls) ? p->residuo_s : p->residuo_d;

    if (succ == 0) {
      // Il valore sara' sicuramente il residuo da aggiungere
      return residuo;
    } else {
      // Aggiungo al valore dell'intervallo il residuo
      succ->k += residuo;
      // L'incremento dei residuo del nodo adiacente permette di garanire
      // che a tutto il sottoalbero sara' applicato il residuo ma mano
      // che la procedura di query avanza.
      succ->residuo_s += residuo;
      succ->residuo_d += residuo;
      // Passato il residuo salvato
      residuo = 0;
      return query(i, succ);
    }
  }
}
```
Il metodo *query* mantiene ancora complessita' `O(lg(N))` rimanendo una ricerca
binaria svolta in un albero binario bilanciato. 
