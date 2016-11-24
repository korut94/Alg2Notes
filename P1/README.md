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
Data la richiesta di compiere le operazioni in `O(lg(N))` conviene utilizzare
una struttura ad albero binario bilanciato. 

**Osservazione - 2**:
L'albero `T` per soddisfare il vincolo in spazio deve avere al piu' `N` nodi.

**Osservazione - 3**:
Dato l'array `C` di dimensione `N` il numero d'intervalli e' massimo quando
questi hanno dimensione unitaria, ovvero ogni cella dell'array ha numeri
diversi.

Per **2 + 3** posso dire che i nodi o rappresentano gli intervalli o i valori
presenti nell'array. La **1** mette in luce che manca la condizione di
partizionamento dell'albero binario, il qui puo' essere scelta o usando i
valori o gli intervalli.

**Osservazione - 4**:
L'uso dei valori come paramentro di partizionamento non garantisce il
bilanciamento dell'albero.

L'inserimento consecutivo di valori crescenti o decresenti causerebbe la
formazione di una coda, piombando il tempo per eseguire le operazioni a
`O(N)`.

Percio' per **4** i nodi rappresentano i valore dell'intervallo e si va ad
utilizzare gli intervalli per partizionare l'albero. A seconda del livello
di profondita' del nodo si puo' determinare la zona dell'array eseguendo
il partizionamento `0 <= N/2^h N` dove `h` e' il livello di profondita' del
nodo corrente.

```  
        N
   N/2  |  N/2
p 1 | 2 | 3 | 4
 ---|---|---|---
i 0 | 1 | 2 | 3
C 2 | 8 | 8 | 0
```

Con le varie osservazioni effettuare possiamo gia' definire una struttura base
il cui occupa `O(N)` e esegue l'operazione di ricerca in `O(lg(N))`.

```C++
struct Nodo {
	int    k;  // Valore dell'intervallo rappresentato

	int    ls, // Limite sinistro dell'intervallo
		   ld; // Limite destro dell'intervallo

	Nodo * sx, // puntatore al sottoalbero sinistro
		   dx, // puntatore al sottoalbero destro
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
	// Siamo dentro all'intervallo
	if (p->ls >= i && i <= p->ls) {
		return p->k;
	} else if (i < p->ls) { // Intervallo esterno sinitro
		return query(i, p->sx);
	} else { // Intervallo esterno destro
		return query(i, p->dx);
	}
}
```

**Osservazione - 5**:
Quando `p` o un suo figlio non sono inizializzati allora si sa che
`C[h] == 0` dove `h` e' la profondita' del nodo puntato da `p`.

Per **5** allora si ha:
```C++
int query_V1(int i)
{
	// Struttura dati vuota
	if (p == 0) {
		return 0;
	} else if (p->ls >= i && i <= p->ls) { // Siamo dentro all'intervallo
		return p->k;
	} else if (i < p->ls) { // Intervallo esterno sinitro
		return (p->ls) ? query(i, p->sx) : 0;
	} else { // Intervallo esterno destro
		return (p->ld) ? query(i, p->dx) : 0;
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
Nodo * update(int i, int j, int c, Nodo *p)
{
	// Caso (1) Intervallo di soli zeri
	if (p == 0) {
		// Costruiamo un nodo foglia con il valore dell'intevallo [i,j] uguale
		// a c.
		return new Nodo(c, i, j);
	// Caso (2) intervallo fuori completamente da p
	} else if (j < p->ls) {
		p->sx = update(i, j, c, p->sx);
		return p;
	} else if (i > p->ld) {
		p->dx = update(i, j, c p->dx);
		return p;
	} else {
		// Caso (3) i < ls < j
		// L'intervallo va sopra il limite inferiore del nodo corrente
		if (p->ls > i && p->ls < j) {
			// Aggiorna solo l'intervallo a sinistra di p
			p->sx = updadate(i, p->ls - 1, c);
			// Sposta l'intervallo da aggiornare a [ls,j]
			i = p->ls;
		}
		
		// Caso (4) i < ld < j
		// L'intervallo va sopra il limite superiore del nodo corrente
		if (p->ld > i && p->ld < j) {
			// Aggiorna solo l'intervallo a destra di p
			p->dx = update(p->ld + 1, j, c);
			// Sposta l'intervallo da aggioranre a [i,ld]
			j = p->ld;
		}
		
		// Caso (5) aggiornare il nodo corrente
	        if (i >= p->ls && p->ld <= j) {
			// Aggiornamenti per i due intervalli creati
			p->sx = (p->ls == i) ? p->sx : update(p->ls, i - 1, p->k, p->sx);
			p->dx = (p->ld == j) ? p->dx : update(j + 1, p->ds, p->k, p->dx);
			
			// Ristringo l'intervallo del nodo attuale
			p->ls = i;
			p->ld = j;
		}
		
		// Incremento il nodo attuale
		p->k += c;
		
		return p;
	}
}

```

La procedura di *update* aggiorna l'albero in modo corretto ma **non** l'ho fa in `O(lg(N))` questo
perche' abbiamo la necessita' in caso l'intervallo `[i,j]` comprenda l'intervallo `[ls,ld]` di `p`
e' necessario scendere ricorsivamente nei due sottoalberi di p, portando inevitabilemente la
complessita' a `O(N)`.












