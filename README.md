<h2>Prova Finale di Algoritmi e Principi dell'Informatica</h2>

---

<h2>Specifiche di Progetto</h2>

L'obiettivo del progetto è la gestione di una classifica tra grafi diretti pesati. Il programma da realizzare riceve in ingresso<br>
• due parametri, una sola volta<br>
  • d: il numero di nodi dei grafi<br>
  • k: la lunghezza della classifica<br>
• Una sequenza di comandi tra<br>
• AggiungiGrafo [matrice-di-adiacenza]<br>
• TopK<br>
d, k e il numero di grafi sono rappresentabili con interi a 32 bit<br>

<h4>AggiungiGrafo</h4>

Richiede di aggiungere un grafo a quelli considerati per stilare la classifica. È seguito dalla matrice di adiacenza del grafo stesso, stampata una riga per 
ogni rigo, con gli elementi separati da virgole. I nodi del grafo sono da considerarsi etichettati logicamente con un indice intero tra 0 e d-1; il nodo in
posizione 0 è quello la cui stella uscente è descritta dalla prima riga della matrice.

<h4>AggiungiGrafo</h4>

• Si consideri ogni grafo dall'inizio del programma fino al comando TopK etichettato con un indice intero corrispondente al numero di grafi letti prima di esso<br>
• TopK richiede al programma di stampare gli indici interi dei k grafi aventi i k valori più piccoli della seguente metrica:<br>
  • Somma dei cammini più brevi tra il nodo 0 e tutti gli altri nodi del grafo raggiungibili da 0<br>
• Se ci sono più grafi con lo stesso valore della metrica, si dà la precedenza ai primi arrivati<br>
• Le distanze dei nodi non raggiungibili da 0 sono considerate nulle<br>
• I k indici interi sono stampati, su un unico rigo, separati da uno spazio, in un qualunque ordine
