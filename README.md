# ADRC

---

## Projecto 1 
### Binary prefix codes and Huffman’s codes

Para comilar o PrefixCode:

    gcc -Wall -g PrefixCode.c Estruturas/tree.c -o PrefixCode

Para compilar o Huffman:

    gcc -Wall -g Huffman.c Estruturas/heap.c Estruturas/tree.c -o Huffman 

---

## Projecto 2
### Inter-domain routing

    gcc -Wall -g main.c Estruturas/avl_tree.c Estruturas/list.c Estruturas/fifo.c topology.c route.c -o main

Responder à questão:
Quao mais longe é o caminho quando estou restrito a rotas comercias?


`Substituir flags -Wall e -g por -O para melhor performance`
