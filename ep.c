#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int *v;
    int topo;
    int max;
} pilha;


/* Funcoes da pilha */

pilha *criaPilha(int n) {
    pilha *p;

    p = malloc(sizeof(pilha));
    p->v = malloc(n*sizeof(int));
    p->max = n;
    p->topo = 0;

    return p;
}

void realoca(pilha *p) {
    int *tmp, i, novo_max;

    novo_max = p->max+1;

    tmp = malloc(novo_max*sizeof(int));

    for(i = 0; i < p->max; i++)   tmp[i] = p->v[i];

    free(p->v);

    p->v = tmp;
    p->max = novo_max;
}

void empilha(pilha *p, int x) {
    if (p->max == p->topo)    realoca(p);

    p->v[p->topo] = x;
    p->topo++;
}

int desempilha(pilha *p) {
    p->topo--;

    return p->v[p->topo];
}


/* Funcoes auxiliares */

int buscaMenor(int *v, int n, int num) {
    int i, ind = num;

    for (i = num+1; i < n; i++)
        if (v[ind] > v[i])    ind = i;
        
    return ind;
}

int ladoDireito(int n, int a, int b) {
    if (a%2 == b%2) {    /* Se a e b sao indices de mesma paridade */
        if (!(n%2)) {
            if (a < b) {
                if (b-a < n/2 )    return 1;
    
                return 0;
            }

            /* Se b < a */
            if (a-b > n/2)    return 0;

            return 1;
        }

        /* Se n for impar */
        if (a < b) {
            if (b-a > a+n-b)    return 0;

            return 1;
        }

        if (a-b > b+n-a)    return 1;
    }

    /* Se a e b sao indices de paridade diferente */
    if (b > a) 
        return 0;

    return 1;
}

int mod(int x, int n) {
    if (x < 0)    return x+n;

    if (x >= n)    return x-n;

    return x;
}

void troca(int *v, int n, int a, int b) {
    int aux = v[a];

    v[a] = v[b];
    v[b] = aux;
}

int troca3(int *v, int n, int origem, int destino, pilha *p) {
    int soma1 = 2, soma2 = 0, proximo;
    pilha *aux;

    if (origem%2 != destino%2 && !(n%2))    /* Se nao for possivel ir da origem ate o destino */
        return 0;

    if (!ladoDireito(n, origem, destino)) {
        soma1 = -soma1;
        soma2-=2;
    }

    aux = criaPilha(1);

    while (origem != destino) {
        proximo = mod(origem+soma1, n);

        if (v[origem] > v[proximo])    /* Se algum elemento ja ordenado for deslocado */
            empilha(aux, mod(origem+soma2, n));

        troca(v, n, origem, proximo);
        empilha(p, mod(proximo+soma2, n));

        origem = proximo;
    }

    while (aux->topo > 0) {
        proximo = desempilha(aux);
        
        troca(v, n, proximo, mod(proximo+2, n));
        empilha(p, proximo);
    }

    free(aux->v);
    free(aux);

    return 1;
}

int main (){
    int n, i, ind, *v, *ord;
    pilha *passos;

    /* Recebendo n e os valores de v */
    scanf("%d", &n);

    v = malloc(n*sizeof(int));

    for(i = 0; i < n; i++)   scanf("%d", &v[i]);

    passos = criaPilha(1);

    for (i = 0; i < n; i++) {
        ind = buscaMenor(v, n, i);
        
        if (!troca3(v, n, ind, i, passos)) {    /* Se nao for possivel ordenar o vetor */
            printf("Nao e possivel\n");

            return 0;
        }

        printf("%d\n", i);
    }

    for (i = 0; i < n; i++)    printf("%d ",  v[i]);
    printf("\n");

    return 1;
}
