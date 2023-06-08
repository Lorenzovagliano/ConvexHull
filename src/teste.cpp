#include <iostream>
#include <fstream>
#include <sstream>

#include "Ponto.hpp"
#include "Reta.hpp"

int produtoVetorial3(const Ponto& A, const Ponto& B, const Ponto& C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

bool isEsquerda(const Ponto& A, const Ponto& B, const Ponto& P) {
    return produtoVetorial3(A, B, P) > 0;
}

Ponto* JarvisFecho(Ponto pontos[], int n, int& tamanhoFecho) {
    if (n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int maisEsquerda = 0;
    for (int i = 1; i < n; i++) {
        if (pontos[i].x < pontos[maisEsquerda].x)
            maisEsquerda = i;
    }

    int p = maisEsquerda, q;
    tamanhoFecho = 0;

    Ponto* pontosFecho = new Ponto[n];

    do {
        pontosFecho[tamanhoFecho++] = pontos[p];
        q = (p + 1) % n;

        for (int i = 0; i < n; i++) {
            if (isEsquerda(pontos[p], pontos[i], pontos[q]))
                q = i;
        }

        p = q;
    } while (p != maisEsquerda);

    return pontosFecho;
}

int distanciaEuclidiana(const Ponto& A, const Ponto& B) {
    int dx = B.x - A.x;
    int dy = B.y - A.y;
    return dx * dx + dy * dy;
}

bool compararPontos(const Ponto& A, const Ponto& B, const Ponto& reference) {
    int produtoVetorial = (A.x - reference.x) * (B.y - reference.y) - (B.x - reference.x) * (A.y - reference.y);
    if (produtoVetorial == 0) {
        return distanciaEuclidiana(A, reference) < distanciaEuclidiana(B, reference);
    }
    return produtoVetorial > 0;
}

void merge(Ponto pontos[], int esquerda, int meio, int direita, const Ponto& reference) {
    int n1 = meio - esquerda + 1;
    int n2 = direita - meio;

    Ponto* arrEsq = new Ponto[n1];
    Ponto* arrDir = new Ponto[n2];

    for (int i = 0; i < n1; i++)
        arrEsq[i] = pontos[esquerda + i];
    for (int j = 0; j < n2; j++)
        arrDir[j] = pontos[meio + 1 + j];

    int i = 0, j = 0, k = esquerda;

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

void mergeSort(Ponto pontos[], int esquerda, int direita, const Ponto& reference) {
    if (esquerda < direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        mergeSort(pontos, esquerda, meio, reference);
        mergeSort(pontos, meio + 1, direita, reference);
        merge(pontos, esquerda, meio, direita, reference);
    }
}

Ponto* MergeConvexHullGraham(Ponto pontos[], int n, int& tamanhoFecho) {
    if (n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int menorIndice = 0;
    for (int i = 1; i < n; i++) {
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

    for (int i = 2; i < n; i++) {
        while (tamanhoFecho > 1 && !compararPontos(pontosFecho[tamanhoFecho - 2], pontosFecho[tamanhoFecho - 1], pontos[i]))
            tamanhoFecho--;

        pontosFecho[tamanhoFecho] = pontos[i];
        tamanhoFecho++;
    }

    return pontosFecho;
}

void insertionSort(Ponto pontos[], int n, const Ponto& reference) {
    for (int i = 1; i < n; i++) {
        Ponto chave = pontos[i];
        int j = i - 1;
        while (j >= 0 && !compararPontos(pontos[j], chave, reference)) {
            pontos[j + 1] = pontos[j];
            j--;
        }
        pontos[j + 1] = chave;
    }
}

Ponto* InsertConvexHullGraham(Ponto pontos[], int n, int& tamanhoFecho) {
    if (n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int menorIndice = 0;
    for (int i = 1; i < n; i++) {
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

    for (int i = 2; i < n; i++) {
        while (tamanhoFecho > 1 && !compararPontos(pontosFecho[tamanhoFecho - 2], pontosFecho[tamanhoFecho - 1], pontos[i])) {
            tamanhoFecho--;
        }

        pontosFecho[tamanhoFecho] = pontos[i];
        tamanhoFecho++;
    }

    return pontosFecho;
}

void bucketSort(Ponto points[], int n) {
    const int numBuckets = n;
    Ponto* buckets = new Ponto[numBuckets];
    for (int i = 0; i < numBuckets; i++) {
        buckets[i] = points[0];
    }

    Ponto reference = points[0];
    for (int i = 1; i < n; i++) {
        if (compararPontos(points[i], buckets[i], reference)) {
            reference = points[i];
        }
        while (compararPontos(buckets[i], points[i], reference)) {
            i++;
        }
        if (i < n) {
            std::swap(points[i], buckets[i]);
        }
        i--;
    }

    delete[] buckets;
}

Ponto* BucketConvexHullGraham(Ponto pontos[], int n, int& tamanhoFecho) {
    if (n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int menorIndice = 0;
    for (int i = 1; i < n; i++) {
        if (pontos[i].y < pontos[menorIndice].y || (pontos[i].y == pontos[menorIndice].y && pontos[i].x < pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    Ponto temp = pontos[0];
    pontos[0] = pontos[menorIndice];
    pontos[menorIndice] = temp;

    bucketSort(pontos + 1, n - 1);

    Ponto* pontosFecho = new Ponto[n];
    tamanhoFecho = 0;

    pontosFecho[tamanhoFecho] = pontos[0];
    tamanhoFecho++;

    pontosFecho[tamanhoFecho] = pontos[1];
    tamanhoFecho++;

    for (int i = 2; i < n; i++) {
        while (tamanhoFecho > 1 && !compararPontos(pontosFecho[tamanhoFecho - 2], pontosFecho[tamanhoFecho - 1], pontos[i])) {
            tamanhoFecho--;
        }

        pontosFecho[tamanhoFecho] = pontos[i];
        tamanhoFecho++;
    }

    return pontosFecho;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <input_file.txt>\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cout << "Erro ao abir arquivo de entrada.\n";
        return 1;
    }

    const int maxPontos = 10000;  // Número máximo de
    Ponto pontos[maxPontos];
    int n = 0;

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        if (n >= maxPontos) {
            std::cout << "Número máximo de pontos atingido, diminuir entrada\n";
            return 1;
        }
        if (!(iss >> pontos[n].x >> pontos[n].y)) {
            std::cout << "Invalid input format.\n";
            return 1;
        }
        n++;
    }

    int tamanhoFecho = 0;
    Ponto* pontosFecho = JarvisFecho(pontos, n, tamanhoFecho);
    tamanhoFecho = 0;
    Ponto* GrahamMergeConvexHullPoints = MergeConvexHullGraham(pontos, n, tamanhoFecho);
    tamanhoFecho = 0;
    Ponto* GrahamBucketConvexHullPoints = BucketConvexHullGraham(pontos, n, tamanhoFecho);
    tamanhoFecho = 0;
    Ponto* GrahamInsertConvexHullPoints = InsertConvexHullGraham(pontos, n, tamanhoFecho);

    std::cout << "Jarvis Convex Hull Points:\n";
    for (int i = 0; i < tamanhoFecho; i++) {
        std::cout << "(" << pontosFecho[i].x << ", " << pontosFecho[i].y << ")\n";
    }

    std::cout << std::endl;

    std::cout << "Graham + MergeSort Convex Hull Points:\n";
    for (int i = 0; i < tamanhoFecho; i++) {
        std::cout << "(" << GrahamMergeConvexHullPoints[i].x << ", " << GrahamMergeConvexHullPoints[i].y << ")\n";
    }

    std::cout << std::endl;

    std::cout << "Graham + InsertionSort Convex Hull Points:\n";
    for (int i = 0; i < tamanhoFecho; i++) {
        std::cout << "(" << GrahamInsertConvexHullPoints[i].x << ", " << GrahamInsertConvexHullPoints[i].y << ")\n";
    }    

    std::cout << std::endl;

    std::cout << "Graham + BucketSort Convex Hull Points:\n";
    for (int i = 0; i < tamanhoFecho; i++) {
        std::cout << "(" << GrahamBucketConvexHullPoints[i].x << ", " << GrahamBucketConvexHullPoints[i].y << ")\n";
    }    

    delete[] pontosFecho;
    delete[] GrahamMergeConvexHullPoints;
    delete[] GrahamInsertConvexHullPoints;
    delete[] GrahamBucketConvexHullPoints;

    return 0;
}