#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <cmath>

#include <SFML/Graphics.hpp>
#include "Ponto.hpp"
#include "Reta.hpp"
#include "Fecho.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "\n\tERRO DE USO, utilize o Formato de Execução Correto: \n" << "\t'make run ENTRADA=<nome do seu arquivo de entrada> VIDEO=<sim ou nao>'\n"; 
        std::cout << "\n\tExemplo: 'make run ENTRADA=entrada10.txt VIDEO=nao'\n";
        std::cout << "\n\tLembrando, esse programa possui capacidades gráficas e requer que o usuario crie um arquivo de entrada contendo\n\tos pontos(coordenadas) a serem analisados. Devido ao funcionamento do Makefile, para que o usuário selecione se\n\tdeseja ver a demonstração gráfica e selecione o arquivo de entrada desejado pela linha de comando, ele deve\n\tinserir essas informações após o 'make run', com ENTRADA= e VIDEO= seguindo o formato descrito acima.\n";
        std::cout << "\n\tAviso: A demonstração gráfica pode ser lenta a depender da sua máquina.\n\n";
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

    const int maxPontos = 1000000;  // Maximum number of pontos
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
            std::cout << "\n\tFormato de entradas do arquivo com formatação errada. Os pontos devem estar dispostos como nas entradas dadas de exemplo pelos professores.\n";
            std::cout << "\n\tExemplo:\n\t2 3\n\t1 0\n\t23 5\n\t55 100\n\n";
            std::cout << "\n\tTambém certifique-se de que não existam linhas vazias no final do seu arquivo de entrrada\n\n";
            return 1;
        }
        n++;
    }

    Ponto pontos1[n], pontos2[n], pontos3[n], pontos4[n];
    for(int i = 0; i < n; i++){
        pontos1[i] = pontos[i];
        pontos2[i] = pontos[i];
        pontos3[i] = pontos[i];
        pontos4[i] = pontos[i];
    }

    Fecho fecho = Fecho(pontos);

    int tamanhoFecho = 0;

    //Chamando a função do Graham + MergeSort e calculando o tempo para executá-la
    double tempoGrahamMerge;
    auto start2 = std::chrono::high_resolution_clock::now();

    Ponto* GrahamMergeConvexHullPoints = fecho.MergeConvexHullGraham(pontos2, n, tamanhoFecho);

    auto end2 = std::chrono::high_resolution_clock::now();
    tempoGrahamMerge = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count();

    //Chamando a função do Graham + InsertionSort e calculando o tempo para executá-la
    double tempoGrahamInsert;
    auto start3 = std::chrono::high_resolution_clock::now();

    Ponto* GrahamInsertConvexHullPoints = fecho.InsertConvexHullGraham(pontos3, n, tamanhoFecho);

    auto end3 = std::chrono::high_resolution_clock::now();
    tempoGrahamInsert = std::chrono::duration_cast<std::chrono::nanoseconds>(end3 - start3).count();

    //Chamando a função do Graham + RadixSort e calculando o tempo para executá-la
    double tempoGrahamBucket;
    auto start4 = std::chrono::high_resolution_clock::now();

    Ponto* GrahamBucketConvexHullPoints = fecho.BucketConvexHullGraham(pontos4, n, tamanhoFecho);

    auto end4 = std::chrono::high_resolution_clock::now();
    tempoGrahamBucket = std::chrono::duration_cast<std::chrono::nanoseconds>(end4 - start4).count();

    //Chamando a função do Jarvis e calculando o tempo para executá-la
    double tempoJarvis;
    auto start1 = std::chrono::high_resolution_clock::now();

    Ponto* JarvisConvexHullPoints = fecho.JarvisFecho(pontos1, n, tamanhoFecho);

    auto end1 = std::chrono::high_resolution_clock::now();
    tempoJarvis = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1).count();


    //Realizando a impressão das saídas
    std::cout << "FECHO CONVEXO\n";
    for (int i = 0; i < fecho.tamanho; i++) {
        std::cout << JarvisConvexHullPoints[i].x << ' ' << JarvisConvexHullPoints[i].y << std::endl;
    }

    std::cout << std::endl;

    std::cout << "GRAHAM+MERGESORT: " << tempoGrahamMerge/1000000000 << "s" << std::endl;
    std::cout << "GRAHAM+INSERTIONSORT: " << tempoGrahamInsert/1000000000 << "s" << std::endl;
    std::cout << "GRAHAM+BUCKETSORT: " << tempoGrahamBucket/1000000000 << "s" << std::endl;
    std::cout << "JARVIS: " << tempoJarvis/1000000000 << "s" << std::endl;

    //SFML(Biblioteca Gráfica)
    if(video == true){
        std::cout << "\nInicializando Demonstração Gráfica do Algoritmo. Isso pode demorar a depender da sua máquina\n";
        std::cout << "\nPara fechar a demonstração e terminar o programa, simplesmente feche a janela gerada. Após clicar no X, a janela ainda pode demorar um tempo para ser fechada.\n";
        std::cout << "\nA depender da máquina, um SegFault pode ocorrer ao fechar a janela. Porém, isso é natural.\n\n";

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
            for (int i = 0; i < tamanhoFecho; ++i) {
                float distance = std::max(std::abs(JarvisConvexHullPoints[i].x), std::abs(JarvisConvexHullPoints[i].y));
                maxDistance = std::max(maxDistance, distance);
            }

            //Fator de escala para as coordenadas
            float scaleFactor = std::min(400.f / maxDistance, 300.f / maxDistance);

            //Calcular centro do Polígono
            sf::Vector2f centroid(0.0f, 0.0f);
            for (int i = 0; i < tamanhoFecho; ++i) {
                centroid.x += JarvisConvexHullPoints[i].x;
                centroid.y += JarvisConvexHullPoints[i].y;
            }
            centroid /= static_cast<float>(tamanhoFecho);

            //Desenhar todos os pontos em Azul
            for (int i = 0; i < n; ++i) {
                sf::Vector2f translatedPoint = windowCenter + sf::Vector2f((pontos[i].x - centroid.x) * scaleFactor, (pontos[i].y - centroid.y) * scaleFactor);

                //Desenhar vertex
                sf::CircleShape vertex(2.5f);
                vertex.setFillColor(sf::Color::Blue);
                vertex.setOrigin(1.5f, 1.5f);
                vertex.setPosition(translatedPoint);
                window.draw(vertex);
            }

            //Desenhar vértices em Verde
            for (int i = 0; i < tamanhoFecho; ++i) {
                sf::Vector2f translatedPoint = windowCenter + sf::Vector2f((JarvisConvexHullPoints[i].x - centroid.x) * scaleFactor, (JarvisConvexHullPoints[i].y - centroid.y) * scaleFactor);

                //Desenhar vertex
                sf::CircleShape vertex(3.0f);
                vertex.setFillColor(sf::Color::Green);
                vertex.setOrigin(1.5f, 1.5f);
                vertex.setPosition(translatedPoint);
                window.draw(vertex);
            }

            //Desenhar Linhas em Vermelho
            for (int i = 0; i < tamanhoFecho; ++i) {
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