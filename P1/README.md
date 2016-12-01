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
Inanzitutto si puo' notare che il vincolo di spazio e' gia' rispettato per la natura del problema.

**Osservazione**:
> Ogni livello dell'albero, a partire dal basso, ha sempre la meta' dei nodi.

Questo e' dovuta al modo di costruzione dell'albero. Per ogni due figli, di un livello. ci sara'
un padre in quello successivo.

**Osservazione**:
> Se `C(N)` e' il numero di nodi del nostro albero, se `N` e' un multiplo di 2 abbiamo che
`C(N) = N + C(N/2)`

Per il *Master Theorem* abbiamo `C(N) = O(N)`. In generale per ogni `N` abbiamo un numero di nodi
`O(N)`: basta prendere il primo `M > N` potenza di 2 e per l'ultima osservazione si ha la tesi.
