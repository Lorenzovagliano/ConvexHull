#include "Fecho.hpp"

// Calcula o produto vetorial entre três pontos A, B e C
int produtoVetorial3(const Ponto& A, const Ponto& B, const Ponto& C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

// Verifica se o ponto P está à esquerda da linha definida pelos pontos A e B
bool isEsquerda(const Ponto& A, const Ponto& B, const Ponto& P) {
    // Calcula o produto vetorial entre os vetores AB e AP
    // Se o resultado for maior que zero, o ponto P está à esquerda da linha AB
    // Caso contrário, o ponto P está à direita ou sobre a linha AB
    return produtoVetorial3(A, B, P) > 0;
}

// Calcula a distância euclidiana ao quadrado entre dois pontos A e B
int distanciaEuclidiana(const Ponto& A, const Ponto& B) {
    int dx = B.x - A.x;
    int dy = B.y - A.y;
    return dx * dx + dy * dy;
}

// Compara dois pontos A e B em relação a um ponto de referência
bool compararPontos(const Ponto& A, const Ponto& B, const Ponto& reference) {
    int produtoVetorial = (A.x - reference.x) * (B.y - reference.y) - (B.x - reference.x) * (A.y - reference.y);
    if (produtoVetorial == 0) {
        // Se for igual a zero, compara as distâncias euclidianas ao quadrado entre A e a referência e B e a referência
        // Retorna verdadeiro se a distância de A for menor que a distância de B, caso contrário, retorna falso
        return distanciaEuclidiana(A, reference) < distanciaEuclidiana(B, reference);
    }
    // Se o produto vetorial for maior que zero, retorna verdadeiro
    // Caso contrário, retorna falso
    return produtoVetorial > 0;
}

// Função auxiliar do mergeSort
void merge(Ponto pontos[], int esquerda, int meio, int direita, const Ponto& reference) {
    // Calcula o tamanho de cada parte do array
    int n1 = meio - esquerda + 1;
    int n2 = direita - meio;

    // Cria arrays temporários para armazenar as partes esquerda e direita
    Ponto* arrEsq = new Ponto[n1];
    Ponto* arrDir = new Ponto[n2];

    // Copia os elementos da parte esquerda do array original para o array temporário da esquerda
    for (int i = 0; i < n1; i++)
        arrEsq[i] = pontos[esquerda + i];
    // Copia os elementos da parte direita do array original para o array temporário da direita
    for (int j = 0; j < n2; j++)
        arrDir[j] = pontos[meio + 1 + j];

    int i = 0, j = 0, k = esquerda;

    // Combina as duas partes ordenadas do array original
    while (i < n1 && j < n2) {
        // Compara os pontos das partes esquerda e direita usando a função compararPontos
        if (compararPontos(arrEsq[i], arrDir[j], reference)) {
            // Se o ponto da parte esquerda for menor, copia para o array original e avança o índice i
            pontos[k].x = arrEsq[i].x;
            pontos[k].y = arrEsq[i].y;
            i++;
        } else {
            // Se o ponto da parte direita for menor, copia para o array original e avança o índice j
            pontos[k].x = arrDir[j].x;
            pontos[k].y = arrDir[j].y;
            j++;
        }
        // Avança o índice k no array original
        k++;
    }

    // Copia os elementos restantes do array temporário da parte esquerda, se houver
    while (i < n1) {
        pontos[k].x = arrEsq[i].x;
        pontos[k].y = arrEsq[i].y;
        i++;
        k++;
    }
    // Copia os elementos restantes do array temporário da parte direita, se houver
    while (j < n2) {
        pontos[k].x = arrDir[j].x;
        pontos[k].y = arrDir[j].y;
        j++;
        k++;
    }

    // Libera a memória alocada para os arrays temporários
    delete[] arrEsq;
    delete[] arrDir;
}

// Aplica o algoritmo de merge sort em um array de pontos
void mergeSort(Ponto pontos[], int esquerda, int direita, const Ponto& reference) {
    if (esquerda < direita) {
        // Calcula o índice do meio do array
        int meio = esquerda + (direita - esquerda) / 2;

        // Aplica o merge sort recursivamente na parte esquerda do array
        mergeSort(pontos, esquerda, meio, reference);

        // Aplica o merge sort recursivamente na parte direita do array
        mergeSort(pontos, meio + 1, direita, reference);

        // Realiza o merge das partes ordenadas
        merge(pontos, esquerda, meio, direita, reference);
    }
}

// Aplica o algoritmo de insertion sort em um array de pontos
void insertionSort(Ponto pontos[], int n, const Ponto& reference) {
    for (int i = 1; i < n; i++) {
        Ponto chave = pontos[i]; // Armazena o elemento atual em uma variável temporária

        int j = i - 1;

        // Move os elementos maiores que a chave para a direita
        while (j >= 0 && !compararPontos(pontos[j], chave, reference)) {
            pontos[j + 1] = pontos[j];
            j--;
        }
        pontos[j + 1] = chave; // Insere a chave na posição correta
    }
}

// Aplica o algoritmo de bucket sort em um array de pontos
void bucketSort(Ponto* points, int n) {
    const int numBuckets = sqrt(n); // Determina o número de baldes com base na raiz quadrada do tamanho do array

    int max = points[0].x;
    for (int i = 1; i < n; i++) {
        if (points[i].x > max) {
            max = points[i].x;
        }
    }

    int buckets[numBuckets][n]; // Cria uma matriz de baldes
    int bucketSize[numBuckets] = {0}; // Inicializa o tamanho de cada balde como zero

    // Distribui os pontos nos baldes com base nas coordenadas x
    for (int i = 0; i < n; i++) {
        int bucketIndex = (numBuckets * points[i].x) / (max + 1);
        buckets[bucketIndex][bucketSize[bucketIndex]] = points[i].x;
        bucketSize[bucketIndex]++;
    }

    // Ordena os elementos dentro de cada balde usando o algoritmo de insertion sort
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

    // Combina os elementos ordenados de cada balde de volta ao array de pontos
    int index = 0;
    for (int i = 0; i < numBuckets; i++) {
        for (int j = 0; j < bucketSize[i]; j++) {
            points[index++].x = buckets[i][j];
        }
    }
}

//Construtor de Fecho
Fecho::Fecho(Ponto* _pontos){
    this->pontos = _pontos;
    this->tamanho = sizeof(_pontos) / sizeof(int);
}

// Encontra o fecho convexo de um conjunto de pontos usando o algoritmo de Jarvis
Ponto* Fecho::JarvisFecho(Ponto _pontos[], int _n, int& _tamanhoFecho){
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int maisEsquerda = 0;
    // Encontra o ponto mais à esquerda no eixo x
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

        // Encontra o próximo ponto mais à esquerda em relação ao ponto atual
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

// Encontra o fecho convexo de um conjunto de pontos usando o Scan de Graham com Merge Sort
Ponto* Fecho::MergeConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho){
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int menorIndice = 0;
    // Encontra o ponto de menor coordenada y, com quebra de empate pela menor coordenada x
    for (int i = 1; i < _n; i++) {
        if (_pontos[i].y < _pontos[menorIndice].y || (_pontos[i].y == _pontos[menorIndice].y && _pontos[i].x < _pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    // Troca o ponto de menor índice com o primeiro ponto
    Ponto temp = _pontos[0];
    _pontos[0] = _pontos[menorIndice];
    _pontos[menorIndice] = temp;

    // Ordena os pontos em sentido anti-horário em relação ao primeiro ponto
    mergeSort(_pontos, 1, _n - 1, _pontos[0]);

    Ponto* pontosFecho = new Ponto[_n];
    _tamanhoFecho = 0;

    pontosFecho[_tamanhoFecho] = _pontos[0];
    _tamanhoFecho++;

    pontosFecho[_tamanhoFecho] = _pontos[1];
    _tamanhoFecho++;

    // Realiza o algoritmo Graham's scan para encontrar o fecho convexo
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

// Encontra o fecho convexo de um conjunto de pontos usando o Scan de Graham com Inserion Sort
Ponto* Fecho::InsertConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho) {
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int menorIndice = 0;
    // Encontra o ponto de menor coordenada y, com quebra de empate pela menor coordenada x
    for (int i = 1; i < _n; i++) {
        if (_pontos[i].y < _pontos[menorIndice].y || (_pontos[i].y == _pontos[menorIndice].y && _pontos[i].x < _pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    // Troca o ponto de menor índice com o primeiro ponto
    Ponto temp = _pontos[0];
    _pontos[0] = _pontos[menorIndice];
    _pontos[menorIndice] = temp;

    // Ordena os pontos em sentido anti-horário em relação ao primeiro ponto
    insertionSort(_pontos + 1, _n - 1, _pontos[0]);

    Ponto* pontosFecho = new Ponto[_n];
    _tamanhoFecho = 0;

    pontosFecho[_tamanhoFecho] = _pontos[0];
    _tamanhoFecho++;

    pontosFecho[_tamanhoFecho] = _pontos[1];
    _tamanhoFecho++;

    // Realiza o algoritmo Graham's scan para encontrar o fecho convexo
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

// Encontra o fecho convexo de um conjunto de pontos usando o Scan de Graham com Bucket Sort
Ponto* Fecho::BucketConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho) {
    if (_n < 3) {
        std::cout << "Não há Fecho Convexo\n";
        return nullptr;
    }

    int menorIndice = 0;
    // Encontra o ponto de menor coordenada y, com quebra de empate pela menor coordenada x
    for (int i = 1; i < _n; i++) {
        if (_pontos[i].y < _pontos[menorIndice].y || (_pontos[i].y == _pontos[menorIndice].y && _pontos[i].x < _pontos[menorIndice].x)) {
            menorIndice = i;
        }
    }

    // Troca o ponto de menor índice com o primeiro ponto
    Ponto temp = _pontos[0];
    _pontos[0] = _pontos[menorIndice];
    _pontos[menorIndice] = temp;

    // Ordena os pontos em sentido anti-horário em relação ao primeiro ponto
    bucketSort(_pontos, _n);

    Ponto* pontosFecho = new Ponto[_n];
    _tamanhoFecho = 0;

    pontosFecho[_tamanhoFecho] = _pontos[0];
    _tamanhoFecho++;

    pontosFecho[_tamanhoFecho] = _pontos[1];
    _tamanhoFecho++;

    // Realiza o algoritmo Graham's scan para encontrar o fecho convexo
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