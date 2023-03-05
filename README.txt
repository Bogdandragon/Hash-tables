Se initializeaza o tabela hash goala, cu codurile corespunzatoare literelor. Se introduc comenzi din fisier pana cand se ajunge la final.

Comanda de inserare introduce pe rand cuvintele in tabela hash astfel:

    1) Lista de lungimi
    - Se cauta in lista data de codul hash elementul cu lungimea asociata egala cu lungimea cuvantului.
    - Daca nu exista niciun element in lista, se creeaza unul nou corespunzator numarului de litere din cuvant si nu se mai verifica nimic.
    - Daca lista nu este vida si nu s-a gasit elementul cautat, se creeaza si se insereaza un element nou in functie de lungimea data la o pozitie potrivita.
    - Daca s-a gasit elementul sau s-a creat, se acceseaza lista care se afla in campul de informatii.
    
    2) Lista de cuvinte
    - Se cauta in lista cuvantul. Daca se gaseste se actualizeaza numarul de aparitii si se repozitioneaza corespunzator.
    - Daca nu se gaseste, se introduce un cuvant nou relativ la celelalte elemente (daca exista).

Pentru printari se parcurge tabela hash si se afiseaza corespunzator tipului de printare si formatului fiecaruia.
ATENTIE!!! Formatul fiecarui tip de afisare este diferit de celelalte, desi nu se cere acest lucru in document, dar asa apare in referinte.

Rezultate locale checker:

                    = Tema 1 SD HashTable =

[in0.txt]...........................................passed  [05/85]
[VALGRIND: in0.txt].................................passed  [02/85]
[in1.txt]...........................................passed  [05/85]
[VALGRIND: in1.txt].................................passed  [02/85]
[in2.txt]...........................................passed  [05/85]
[VALGRIND: in2.txt].................................passed  [02/85]
[in3.txt]...........................................passed  [10/85]
[VALGRIND: in3.txt].................................passed  [02/85]
[in4.txt]...........................................passed  [10/85]
[VALGRIND: in4.txt].................................passed  [02/85]
[in5.txt]...........................................passed  [10/85]
[VALGRIND: in5.txt].................................passed  [02/85]
[in6.txt]...........................................passed  [10/85]
[VALGRIND: in6.txt].................................passed  [02/85]
[in7.txt]...........................................passed  [10/85]
[VALGRIND: in7.txt].................................passed  [02/85]
[in8.txt]...........................................passed  [10/85]
[VALGRIND: in8.txt].................................passed  [02/85]
[in9.txt]...........................................passed  [10/85]
[VALGRIND: in9.txt].................................passed  [02/85]

                                           Total: [105/85]
