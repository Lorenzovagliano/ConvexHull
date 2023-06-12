#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <cmath>
#include <random>

#include <SFML/Graphics.hpp>
#include "Ponto.hpp"

long int produtoVetorial3(const Ponto& A, const Ponto& B, const Ponto& C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

bool isEsquerda(const Ponto& A, const Ponto& B, const Ponto& P) {
    return produtoVetorial3(A, B, P) > 0;
}

Ponto* JarvisFecho(Ponto pontos[], long int n, long int& tamanhoFecho) {
    if (n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    long int maisEsquerda = 0;
    for (long int i = 1; i < n; i++) {
        if (pontos[i].x < pontos[maisEsquerda].x)
            maisEsquerda = i;
    }

    long int p = maisEsquerda, q;
    tamanhoFecho = 0;

    Ponto* pontosFecho = new Ponto[n];

    do {
        pontosFecho[tamanhoFecho++] = pontos[p];
        q = (p + 1) % n;

        for (long int i = 0; i < n; i++) {
            if (isEsquerda(pontos[p], pontos[i], pontos[q]))
                q = i;
        }

        p = q;
    } while (p != maisEsquerda);

    return pontosFecho;
    delete[] pontosFecho;
}

long int distanciaEuclidiana(const Ponto& A, const Ponto& B) {
    long int dx = B.x - A.x;
    long int dy = B.y - A.y;
    return dx * dx + dy * dy;
}

bool compararPontos(const Ponto& A, const Ponto& B, const Ponto& reference) {
    long int produtoVetorial = (A.x - reference.x) * (B.y - reference.y) - (B.x - reference.x) * (A.y - reference.y);
    if (produtoVetorial == 0) {
        return distanciaEuclidiana(A, reference) < distanciaEuclidiana(B, reference);
    }
    return produtoVetorial > 0;
}

void merge(Ponto pontos[], long int esquerda, long int meio, long int direita, const Ponto& reference) {
    long int n1 = meio - esquerda + 1;
    long int n2 = direita - meio;

    Ponto* arrEsq = new Ponto[n1];
    Ponto* arrDir = new Ponto[n2];

    for (long int i = 0; i < n1; i++)
        arrEsq[i] = pontos[esquerda + i];
    for (long int j = 0; j < n2; j++)
        arrDir[j] = pontos[meio + 1 + j];

    long int i = 0, j = 0, k = esquerda;

    while (i < n1 && j < n2) {
        if (compararPontos(arrEsq[i], arrDir[j], reference)) {
            pontos[k].x = arrEsq[i].x;
            pontos[k].y = arrEsq[i].y;
            i++;
        } else {
            pontos[k].x = arrDir[j].x;
            pontos[k].y = arrDir[j].y;
            j++;
        }
        k++;
    }

    while (i < n1) {
        pontos[k].x = arrEsq[i].x;
        pontos[k].y = arrEsq[i].y;
        i++;
        k++;
    }
    while (j < n2) {
        pontos[k].x = arrDir[j].x;
        pontos[k].y = arrDir[j].y;
        j++;
        k++;
    }

    delete[] arrEsq;
    delete[] arrDir;
}

void mergeSort(Ponto pontos[], long int esquerda, long int direita, const Ponto& reference) {
    if (esquerda < direita) {
        long int meio = esquerda + (direita - esquerda) / 2;
        mergeSort(pontos, esquerda, meio, reference);
        mergeSort(pontos, meio + 1, direita, reference);
        merge(pontos, esquerda, meio, direita, reference);
    }
}

Ponto* MergeConvexHullGraham(Ponto pontos[], long int n, long int& tamanhoFecho) {
    if (n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    long int menorIndice = 0;
    for (long int i = 1; i < n; i++) {
        if (pontos[i].y < pontos[menorIndice].y || (pontos[i].y == pontos[menorIndice].y && pontos[i].x < pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    Ponto temp = pontos[0];
    pontos[0] = pontos[menorIndice];
    pontos[menorIndice] = temp;

    mergeSort(pontos, 1, n - 1, pontos[0]);

    Ponto* pontosFecho = new Ponto[n];
    tamanhoFecho = 0;

    pontosFecho[tamanhoFecho] = pontos[0];
    tamanhoFecho++;

    pontosFecho[tamanhoFecho] = pontos[1];
    tamanhoFecho++;

    for (long int i = 2; i < n; i++) {
        while (tamanhoFecho > 1 && !compararPontos(pontosFecho[tamanhoFecho - 2], pontosFecho[tamanhoFecho - 1], pontos[i]))
            tamanhoFecho--;

        pontosFecho[tamanhoFecho] = pontos[i];
        tamanhoFecho++;
    }

    return pontosFecho;
}

void insertionSort(Ponto pontos[], long int n, const Ponto& reference) {
    for (long int i = 1; i < n; i++) {
        Ponto chave = pontos[i];
        long int j = i - 1;
        while (j >= 0 && !compararPontos(pontos[j], chave, reference)) {
            pontos[j + 1] = pontos[j];
            j--;
        }
        pontos[j + 1] = chave;
    }
}

Ponto* InsertConvexHullGraham(Ponto pontos[], long int n, long int& tamanhoFecho) {
    if (n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    long int menorIndice = 0;
    for (long int i = 1; i < n; i++) {
        if (pontos[i].y < pontos[menorIndice].y || (pontos[i].y == pontos[menorIndice].y && pontos[i].x < pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    Ponto temp = pontos[0];
    pontos[0] = pontos[menorIndice];
    pontos[menorIndice] = temp;

    insertionSort(pontos + 1, n - 1, pontos[0]);

    Ponto* pontosFecho = new Ponto[n];
    tamanhoFecho = 0;

    pontosFecho[tamanhoFecho] = pontos[0];
    tamanhoFecho++;

    pontosFecho[tamanhoFecho] = pontos[1];
    tamanhoFecho++;

    for (long int i = 2; i < n; i++) {
        while (tamanhoFecho > 1 && !compararPontos(pontosFecho[tamanhoFecho - 2], pontosFecho[tamanhoFecho - 1], pontos[i])) {
            tamanhoFecho--;
        }

        pontosFecho[tamanhoFecho] = pontos[i];
        tamanhoFecho++;
    }

    return pontosFecho;
}

void insertionSort2(Ponto* arr, long int size) {
    for (long int i = 1; i < size; ++i) {
        Ponto key = arr[i];
        long int j = i - 1;

        while (j >= 0 && arr[j].y > key.y) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

void bucketSort(Ponto* points, long int size) {
    // Find the minimum and maximum values of x-coordinate
    long int minX = points[0].x;
    long int maxX = points[0].x;
    for (long int i = 1; i < size; ++i) {
        if (points[i].x < minX) {
            minX = points[i].x;
        }
        if (points[i].x > maxX) {
            maxX = points[i].x;
        }
    }

    // Create buckets based on the range of x-coordinates
    const long int bucketRange = 10; // Adjust the bucket range as per your needs
    const long int numBuckets = (maxX - minX) / bucketRange + 1;
    long int* bucketSizes = new long int[numBuckets]();
    Ponto** buckets = new Ponto*[numBuckets]();

    // Assign points to their respective buckets based on x-coordinate
    for (long int i = 0; i < size; ++i) {
        long int bucketIndex = (points[i].x - minX) / bucketRange;
        bucketSizes[bucketIndex]++;
    }

    // Allocate memory for each bucket
    for (long int i = 0; i < numBuckets; ++i) {
        buckets[i] = new Ponto[bucketSizes[i]];
    }

    // Reset bucket sizes for reuse
    for (long int i = 0; i < numBuckets; ++i) {
        bucketSizes[i] = 0;
    }

    // Assign points to their respective buckets based on x-coordinate
    for (long int i = 0; i < size; ++i) {
        long int bucketIndex = (points[i].x - minX) / bucketRange;
        buckets[bucketIndex][bucketSizes[bucketIndex]++] = points[i];
    }

    // Sort points within each bucket using insertion sort
    for (long int i = 0; i < numBuckets; ++i) {
        insertionSort2(points, size);
    }

    // Concatenate the sorted buckets
    long int index = 0;
    for (long int i = 0; i < numBuckets; ++i) {
        for (long int j = 0; j < bucketSizes[i]; ++j) {
            points[index++] = buckets[i][j];
        }
    }

    // Deallocate memory
    for (long int i = 0; i < numBuckets; ++i) {
        delete[] buckets[i];
    }
    delete[] buckets;
    delete[]bucketSizes;
}


Ponto* BucketConvexHullGraham(Ponto pontos[], long int n, long int& tamanhoFecho) {
    if (n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    long int menorIndice = 0;
    for (long int i = 1; i < n; i++) {
        if (pontos[i].y < pontos[menorIndice].y || (pontos[i].y == pontos[menorIndice].y && pontos[i].x < pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    Ponto temp = pontos[0];
    pontos[0] = pontos[menorIndice];
    pontos[menorIndice] = temp;

    bucketSort(pontos + 1, n);

    Ponto* pontosFecho = new Ponto[n];
    tamanhoFecho = 0;

    pontosFecho[tamanhoFecho] = pontos[0];
    tamanhoFecho++;

    pontosFecho[tamanhoFecho] = pontos[1];
    tamanhoFecho++;

    for (long int i = 2; i < n; i++) {
        while (tamanhoFecho > 1 && !compararPontos(pontosFecho[tamanhoFecho - 2], pontosFecho[tamanhoFecho - 1], pontos[i])) {
            tamanhoFecho--;
        }

        pontosFecho[tamanhoFecho] = pontos[i];
        tamanhoFecho++;
    }

    return pontosFecho;
}

int* generateRandomArray(int size) {
    int* arr = new int[size];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100000);

    for(int i = 0; i < size; i++){
        arr[i] = dis(gen);
    }

    return arr;
    delete[] arr;
}

int main(){
    int *arr1 = generateRandomArray(10000);
    int *arr2 = generateRandomArray(10000);

    /*for(int i = 0; i < 100000; i++){
        std::cout << arr1[i] << ' ' << arr2[i] << std::endl;
    }*/

    Ponto pontos[10000];

    for(int i = 0; i < 10000; i+=1){
        for(int j = 0; j < i; j++){
            pontos[j] = Ponto(arr1[j], arr2[j]);
        }
        for(int j = 0; j < i; j++){
            std::cout << pontos[j].x << ' ' << pontos[j].y << std::endl;
        }

        long int tamanhoFecho = 0;
        long double tempoGrahamMerge;
        auto start = std::chrono::high_resolution_clock::now();
        
        //Ponto* fecho = JarvisFecho(pontos, i, tamanhoFecho);

        auto end = std::chrono::high_resolution_clock::now();
        tempoGrahamMerge = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        //std::cout << i << std::endl;
        //std::cout << tempoGrahamMerge/1000 << std::endl;

        //delete[] fecho;
    }

    delete[] arr1;
    delete[] arr2;

    return 0;
}