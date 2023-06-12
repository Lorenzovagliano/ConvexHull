#include "Fecho.hpp"

int produtoVetorial3(const Ponto& A, const Ponto& B, const Ponto& C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

bool isEsquerda(const Ponto& A, const Ponto& B, const Ponto& P) {
    return produtoVetorial3(A, B, P) > 0;
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

void insertionSort2(Ponto* points, int size) {
    for (int i = 1; i < size; ++i) {
        Ponto key = points[i];
        int j = i - 1;

        while (j >= 0 && points[j].y > key.y) {
            points[j + 1] = points[j];
            j--;
        }

        points[j + 1] = key;
    }
}

void bucketSort(Ponto* points, int n) {
    const int numBuckets = sqrt(n);

    int max = points[0].x;
    for (int i = 1; i < n; i++) {
        if (points[i].x > max) {
            max = points[i].x;
        }
    }

    int buckets[numBuckets][n];
    int bucketSize[numBuckets] = {0};

    for (int i = 0; i < n; i++) {
        int bucketIndex = (numBuckets * points[i].x) / (max + 1);
        buckets[bucketIndex][bucketSize[bucketIndex]] = points[i].x;
        bucketSize[bucketIndex]++;
    }

    for (int i = 0; i < numBuckets; i++) {
        for (int j = 1; j < bucketSize[i]; j++) {
            int key = buckets[i][j];
            int k = j - 1;
            while (k >= 0 && buckets[i][k] > key) {
                buckets[i][k + 1] = buckets[i][k];
                k--;
            }
            buckets[i][k + 1] = key;
        }
    }

    int index = 0;
    for (int i = 0; i < numBuckets; i++) {
        for (int j = 0; j < bucketSize[i]; j++) {
            points[index++].x = buckets[i][j];
        }
    }
}

Fecho::Fecho(Ponto* _pontos){
    this->pontos = _pontos;
    this->tamanho = sizeof(_pontos) / sizeof(int);
}

Ponto* Fecho::JarvisFecho(Ponto _pontos[], int _n, int& _tamanhoFecho){
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int maisEsquerda = 0;
    for (int i = 1; i < _n; i++) {
        if (_pontos[i].x < _pontos[maisEsquerda].x)
            maisEsquerda = i;
    }

    int p = maisEsquerda, q;
    _tamanhoFecho = 0;

    Ponto* pontosFecho = new Ponto[_n];

    do {
        pontosFecho[_tamanhoFecho++] = _pontos[p];
        q = (p + 1) % _n;

        for (int i = 0; i < _n; i++) {
            if (i == p){
                continue;  
            }
            if (isEsquerda(_pontos[p], _pontos[i], _pontos[q])){
                q = i;
            }
        }

        p = q;
    } while (p != maisEsquerda && _tamanhoFecho < _n);

    this->pontos = pontosFecho;
    this->tamanho = _tamanhoFecho;
    return pontosFecho;
}

Ponto* Fecho::MergeConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho){
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int menorIndice = 0;
    for (int i = 1; i < _n; i++) {
        if (_pontos[i].y < _pontos[menorIndice].y || (_pontos[i].y == _pontos[menorIndice].y && _pontos[i].x < _pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    Ponto temp = _pontos[0];
    _pontos[0] = _pontos[menorIndice];
    _pontos[menorIndice] = temp;

    mergeSort(_pontos, 1, _n - 1, _pontos[0]);

    Ponto* pontosFecho = new Ponto[_n];
    _tamanhoFecho = 0;

    pontosFecho[_tamanhoFecho] = _pontos[0];
    _tamanhoFecho++;

    pontosFecho[_tamanhoFecho] = _pontos[1];
    _tamanhoFecho++;

    for (int i = 2; i < _n; i++) {
        while (_tamanhoFecho > 1 && !compararPontos(pontosFecho[_tamanhoFecho - 2], pontosFecho[_tamanhoFecho - 1], _pontos[i]))
            _tamanhoFecho--;

        pontosFecho[_tamanhoFecho] = _pontos[i];
        _tamanhoFecho++;
    }

    this->pontos = pontosFecho;
    this->tamanho = _tamanhoFecho;
    return pontosFecho;
}

Ponto* Fecho::InsertConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho) {
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int menorIndice = 0;
    for (int i = 1; i < _n; i++) {
        if (_pontos[i].y < _pontos[menorIndice].y || (_pontos[i].y == _pontos[menorIndice].y && _pontos[i].x < _pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    Ponto temp = _pontos[0];
    _pontos[0] = _pontos[menorIndice];
    _pontos[menorIndice] = temp;

    insertionSort(_pontos + 1, _n - 1, _pontos[0]);

    Ponto* pontosFecho = new Ponto[_n];
    _tamanhoFecho = 0;

    pontosFecho[_tamanhoFecho] = _pontos[0];
    _tamanhoFecho++;

    pontosFecho[_tamanhoFecho] = _pontos[1];
    _tamanhoFecho++;

    for (int i = 2; i < _n; i++) {
        while (_tamanhoFecho > 1 && !compararPontos(pontosFecho[_tamanhoFecho - 2], pontosFecho[_tamanhoFecho - 1], _pontos[i])) {
            _tamanhoFecho--;
        }

        pontosFecho[_tamanhoFecho] = _pontos[i];
        _tamanhoFecho++;
    }

    this->pontos = pontosFecho;
    this->tamanho = _tamanhoFecho;
    return pontosFecho;
}

Ponto* Fecho::BucketConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho) {
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int menorIndice = 0;
    for (int i = 1; i < _n; i++) {
        if (_pontos[i].y < _pontos[menorIndice].y || (_pontos[i].y == _pontos[menorIndice].y && _pontos[i].x < _pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    Ponto temp = _pontos[0];
    _pontos[0] = _pontos[menorIndice];
    _pontos[menorIndice] = temp;

    bucketSort(_pontos, _n);

    Ponto* pontosFecho = new Ponto[_n];
    _tamanhoFecho = 0;

    pontosFecho[_tamanhoFecho] = _pontos[0];
    _tamanhoFecho++;

    pontosFecho[_tamanhoFecho] = _pontos[1];
    _tamanhoFecho++;

    for (int i = 2; i < _n; i++) {
        while (_tamanhoFecho > 1 && !compararPontos(pontosFecho[_tamanhoFecho - 2], pontosFecho[_tamanhoFecho - 1], _pontos[i])) {
            _tamanhoFecho--;
        }

        pontosFecho[_tamanhoFecho] = _pontos[i];
        _tamanhoFecho++;
    }

    this->pontos = pontosFecho;
    this->tamanho = _tamanhoFecho;
    return pontosFecho;
}