# ADRC

---

## Projecto 1 
### Binary prefix codes and Huffman’s codes

Para comilar o PrefixCode:

    gcc -Wall -g -std=c99 PrefixCode.c Estruturas/tree.c -o PrefixCode

Para compilar o Huffman:

    gcc -Wall -g -std=c99 Huffman.c Estruturas/heap.c Estruturas/tree.c -o Huffman 

---

## Projecto 2
### Inter-domain routing

    gcc -Wall -g -std=c99 main.c Estruturas/avl_tree.c Estruturas/list.c Estruturas/fifo.c topology.c route.c -o main

Responder à questão:
Quao mais longe é o caminho quando estou restrito a rotas comercias?


---

## Projecto 3
### Edge-connectivity in graphs

    gcc -Wall -g -std=c99 main.c Estruturas/avl_tree.c Estruturas/list.c Estruturas/fifo.c topology.c algorithm.c -o main

`Substituir flags -Wall e -g por -O para melhor performance`
