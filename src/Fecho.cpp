#include "Fecho.hpp"

long int produtoVetorial3(const Ponto& A, const Ponto& B, const Ponto& C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

bool isEsquerda(const Ponto& A, const Ponto& B, const Ponto& P) {
    return produtoVetorial3(A, B, P) > 0;
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

    const long int bucketRange = 10;
    const long int numBuckets = (maxX - minX) / bucketRange + 1;
    long int* bucketSizes = new long int[numBuckets]();
    Ponto** buckets = new Ponto*[numBuckets]();

    for (long int i = 0; i < size; ++i) {
        long int bucketIndex = (points[i].x - minX) / bucketRange;
        bucketSizes[bucketIndex]++;
    }

    for (long int i = 0; i < numBuckets; ++i) {
        buckets[i] = new Ponto[bucketSizes[i]];
    }

    for (long int i = 0; i < numBuckets; ++i) {
        bucketSizes[i] = 0;
    }

    for (long int i = 0; i < size; ++i) {
        long int bucketIndex = (points[i].x - minX) / bucketRange;
        buckets[bucketIndex][bucketSizes[bucketIndex]++] = points[i];
    }

    for (long int i = 0; i < numBuckets; ++i) {
        insertionSort2(points, size);
    }

    long int index = 0;
    for (long int i = 0; i < numBuckets; ++i) {
        for (long int j = 0; j < bucketSizes[i]; ++j) {
            points[index++] = buckets[i][j];
        }
    }

    for (long int i = 0; i < numBuckets; ++i) {
        delete[] buckets[i];
    }
    delete[] buckets;
    delete[]bucketSizes;
}

Fecho::Fecho(Ponto* _pontos){
    this->pontos = _pontos;
    this->tamanho = sizeof(_pontos) / sizeof(int);
}

Ponto* Fecho::JarvisFecho(Ponto _pontos[], long int _n, long int& _tamanhoFecho){
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    long int maisEsquerda = 0;
    for (long int i = 1; i < _n; i++) {
        if (_pontos[i].x < _pontos[maisEsquerda].x)
            maisEsquerda = i;
    }

    long int p = maisEsquerda, q;
    _tamanhoFecho = 0;

    Ponto* pontosFecho = new Ponto[_n];

    do {
        pontosFecho[_tamanhoFecho++] = _pontos[p];
        q = (p + 1) % _n;

        for (long int i = 0; i < _n; i++) {
            if (isEsquerda(_pontos[p], _pontos[i], _pontos[q]))
                q = i;
        }

        p = q;
    } while (p != maisEsquerda);

    this->pontos = pontosFecho;
    this->tamanho = _tamanhoFecho;
    return pontosFecho;
}

Ponto* Fecho::MergeConvexHullGraham(Ponto _pontos[], long int _n, long int& _tamanhoFecho){
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    long int menorIndice = 0;
    for (long int i = 1; i < _n; i++) {
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

    for (long int i = 2; i < _n; i++) {
        while (_tamanhoFecho > 1 && !compararPontos(pontosFecho[_tamanhoFecho - 2], pontosFecho[_tamanhoFecho - 1], _pontos[i]))
            _tamanhoFecho--;

        pontosFecho[_tamanhoFecho] = _pontos[i];
        _tamanhoFecho++;
    }

    this->pontos = pontosFecho;
    this->tamanho = _tamanhoFecho;
    return pontosFecho;
}

Ponto* Fecho::InsertConvexHullGraham(Ponto _pontos[], long int _n, long int& _tamanhoFecho) {
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    long int menorIndice = 0;
    for (long int i = 1; i < _n; i++) {
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

    for (long int i = 2; i < _n; i++) {
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

Ponto* Fecho::BucketConvexHullGraham(Ponto _pontos[], long int _n, long int& _tamanhoFecho) {
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    long int menorIndice = 0;
    for (long int i = 1; i < _n; i++) {
        if (_pontos[i].y < _pontos[menorIndice].y || (_pontos[i].y == _pontos[menorIndice].y && _pontos[i].x < _pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    Ponto temp = _pontos[0];
    _pontos[0] = _pontos[menorIndice];
    _pontos[menorIndice] = temp;

    bucketSort(_pontos + 1, _n);

    Ponto* pontosFecho = new Ponto[_n];
    _tamanhoFecho = 0;

    pontosFecho[_tamanhoFecho] = _pontos[0];
    _tamanhoFecho++;

    pontosFecho[_tamanhoFecho] = _pontos[1];
    _tamanhoFecho++;

    for (long int i = 2; i < _n; i++) {
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