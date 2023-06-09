#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <cmath>

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


int main(long int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "\n\tERRO DE USO, utilize o Formato de Execução Correto: " << "'make run ENTRADA=" << "<nome do seu arquivo de entrada> " << "VIDEO=" << "<sim ou nao>'\n"; 
        std::cout << "\n\tExemplo: 'make run ENTRADA=entrada10.txt VIDEO=nao'\n";
        std::cout << "\n\tLembrando, esse programa possui capacidades gráficas e requer que o usuario crie um arquivo de entrada contendo\n\tos pontos(coordenadas) a serem analisados. Devido ao funcionamento do Makefile, para que o usuário selecione se\n\tdeseja ver a demonstração gráfica e selecione o arquivo de entrada desejado pela linha de comando, ele deve\n\tinserir essas informações após o 'make run', com ENTRADA= e VIDEO= seguindo o formato descrito acima.\n\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cout << "\n\tErro ao abir arquivo de entrada. Lembre-se, ele deve estar localizado na raiz do projeto.\n\n";
        return 1;
    }

    bool video;
    std::string teste = argv[2];
    if(teste == "sim"){
        video = true;
    }
    else if(teste == "nao"){
        video = false;
    }
    else{
        std::cout << "\n\tOpção de vídeo inválida. Lembre-se, você só pode inserir 'VIDEO=sim' ou 'VIDEO=nao'.\n\n";
        return 1;
    }

    const long int maxPontos = 100000;  // Maximum number of pontos
    Ponto pontos[maxPontos];
    long int n = 0;

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        if (n >= maxPontos) {
            std::cout << "Número máximo de pontos atingido, diminuir entrada\n";
            return 1;
        }
        if (!(iss >> pontos[n].x >> pontos[n].y)) {
            std::cout << "\n\tFormato de entradas do arquivo com formatação errada. Os pontos devem estar dispostos como nas entradas dadas de exemplo pelos professores.\n";
            std::cout << "\n\tExemplo:\n\t2 3\n\t1 0\n\t23 5\n\t55 100\n\n";
            return 1;
        }
        n++;
    }

    Ponto pontos1[n], pontos2[n], pontos3[n], pontos4[n];
    for(long int i = 0; i < n; i++){
        pontos1[i] = pontos[i];
        pontos2[i] = pontos[i];
        pontos3[i] = pontos[i];
        pontos4[i] = pontos[i];
    }

    long int tamanhoFecho = 0;

    //Chamando a função do Graham + MergeSort e calculando o tempo para executá-la
    long double tempoGrahamMerge;
    auto start2 = std::chrono::high_resolution_clock::now();

    Ponto* GrahamMergeConvexHullPoints = MergeConvexHullGraham(pontos2, n, tamanhoFecho);

    auto end2 = std::chrono::high_resolution_clock::now();
    tempoGrahamMerge = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count();

    //Chamando a função do Graham + InsertionSort e calculando o tempo para executá-la
    long double tempoGrahamInsert;
    auto start3 = std::chrono::high_resolution_clock::now();

    Ponto* GrahamInsertConvexHullPoints = InsertConvexHullGraham(pontos3, n, tamanhoFecho);

    auto end3 = std::chrono::high_resolution_clock::now();
    tempoGrahamInsert = std::chrono::duration_cast<std::chrono::nanoseconds>(end3 - start3).count();

    //Chamando a função do Graham + RadixSort e calculando o tempo para executá-la
    long double tempoGrahamBucket;
    auto start4 = std::chrono::high_resolution_clock::now();

    Ponto* GrahamBucketConvexHullPoints = BucketConvexHullGraham(pontos4, n, tamanhoFecho);

    auto end4 = std::chrono::high_resolution_clock::now();
    tempoGrahamBucket = std::chrono::duration_cast<std::chrono::nanoseconds>(end4 - start4).count();

    //Chamando a função do Jarvis e calculando o tempo para executá-la
    long double tempoJarvis;
    auto start1 = std::chrono::high_resolution_clock::now();

    Ponto* JarvisConvexHullPoints = JarvisFecho(pontos1, n, tamanhoFecho);

    auto end1 = std::chrono::high_resolution_clock::now();
    tempoJarvis = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1).count();


    //Realizando a impressão das saídas
    std::cout << "FECHO CONVEXO JARVIS\n";
    for (long int i = 0; i < tamanhoFecho; i++) {
        std::cout << JarvisConvexHullPoints[i].x << ' ' << JarvisConvexHullPoints[i].y << std::endl;
    }

    std::cout << std::endl;

    std::cout << "GRAHAM+MERGESORT: " << tempoGrahamMerge/1000000000 << "s" << std::endl;
    std::cout << "GRAHAM+INSERTIONSORT: " << tempoGrahamInsert/1000000000 << "s" << std::endl;
    std::cout << "GRAHAM+BUCKETSORT: " << tempoGrahamBucket/1000000000 << "s" << std::endl;
    std::cout << "JARVIS: " << tempoJarvis/1000000000 << "s" << std::endl;

    //SFML(Biblioteca Gráfica)
    if(video == true){
        std::cout << '\n' << "Inicializando Demonstração Gráfica do Algoritmo." << std::endl;
        std::cout << "Para fechar a demonstração e terminar o programa, simplesmente feche a janela gerada. Após clicar no X, a janela ainda pode demorar um tempo para ser fechada.\n";
        std::cout << "A depender da máquina, um SegFault pode ocorrer ao fechar a janela. Porém, isso é natural.\n\n";

        //Criar Janela
        sf::RenderWindow window(sf::VideoMode(800, 600), "Fecho Convexo");

        //Calcular posição para centralizar a janela
        sf::Vector2i screenCenter(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2);
        sf::Vector2i windowPosition(screenCenter - sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));

        //Setar a posição da janela
        window.setPosition(windowPosition);
        
        while (window.isOpen()) {
            //Processar eventos
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed){
                    window.close();
                }
            }

            //Limpar a Janela
            window.clear();

            //Calcular centro da janela
            sf::Vector2f windowCenter(800 / 2.f, 600 / 2.f);

            //Calcular distância máxima permitida do centro
            float maxDistance = 0.f;
            for (long int i = 0; i < tamanhoFecho; ++i) {
                float distance = std::max(std::abs(JarvisConvexHullPoints[i].x), std::abs(JarvisConvexHullPoints[i].y));
                maxDistance = std::max(maxDistance, distance);
            }

            //Fator de escala para as coordenadas
            float scaleFactor = std::min(400.f / maxDistance, 300.f / maxDistance);

            //Calcular centro do Polígono
            sf::Vector2f centroid(0.0f, 0.0f);
            for (long int i = 0; i < tamanhoFecho; ++i) {
                centroid.x += JarvisConvexHullPoints[i].x;
                centroid.y += JarvisConvexHullPoints[i].y;
            }
            centroid /= static_cast<float>(tamanhoFecho);

            //Desenhar todos os pontos em Azul
            for (long int i = 0; i < n; ++i) {
                sf::Vector2f translatedPoint = windowCenter + sf::Vector2f((pontos[i].x - centroid.x) * scaleFactor, (pontos[i].y - centroid.y) * scaleFactor);

                //Desenhar vertex
                sf::CircleShape vertex(2.5f);
                vertex.setFillColor(sf::Color::Blue);
                vertex.setOrigin(1.5f, 1.5f);
                vertex.setPosition(translatedPoint);
                window.draw(vertex);
            }

            //Desenhar vértices em Verde
            for (long int i = 0; i < tamanhoFecho; ++i) {
                sf::Vector2f translatedPoint = windowCenter + sf::Vector2f((JarvisConvexHullPoints[i].x - centroid.x) * scaleFactor, (JarvisConvexHullPoints[i].y - centroid.y) * scaleFactor);

                //Desenhar vertex
                sf::CircleShape vertex(3.0f);
                vertex.setFillColor(sf::Color::Green);
                vertex.setOrigin(1.5f, 1.5f);
                vertex.setPosition(translatedPoint);
                window.draw(vertex);
            }

            sf::sleep(sf::seconds(1.0f));

            //Desenhar Linhas em Vermelho
            for (long int i = 0; i < tamanhoFecho; ++i) {
                sf::Vector2f translatedPoint = windowCenter + sf::Vector2f((JarvisConvexHullPoints[i].x - centroid.x) * scaleFactor, (JarvisConvexHullPoints[i].y - centroid.y) * scaleFactor);
                sf::Vector2f nextTranslatedPoint = windowCenter + sf::Vector2f((JarvisConvexHullPoints[(i + 1) % tamanhoFecho].x - centroid.x) * scaleFactor, (JarvisConvexHullPoints[(i + 1) % tamanhoFecho].y - centroid.y) * scaleFactor);

                //Desenhar linha
                sf::Vertex line[] = {
                    sf::Vertex(translatedPoint),
                    sf::Vertex(nextTranslatedPoint)
                };
                line[0].color = sf::Color::Red;
                line[1].color = sf::Color::Red;
                window.draw(line, 2, sf::Lines);

                window.display();
                sf::sleep(sf::seconds(0.5f));
            }

            //Mostrar Janela
            window.display();
        }
    }
    

    delete[] JarvisConvexHullPoints;
    delete[] GrahamMergeConvexHullPoints;
    delete[] GrahamInsertConvexHullPoints;
    delete[] GrahamBucketConvexHullPoints;

    return 0;
}