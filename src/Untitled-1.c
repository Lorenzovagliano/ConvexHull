
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL falhou ao inicializar: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Floco de neve onda senoidal 2 de von Koch", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        800, 600, 0
    );

    if (window == NULL) { 
        fprintf(stderr, "Falha ao inicializar a janela SDL: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    lenSaida = strlen(saida); 
    double inicioX = 100;
    double finalX = 100;
    double inicioY = 300;
    double finalY = 300;
    double tamanhoLinha = (600 / pow(6, estagio)); // para que a imagem continue do mesmo tamanho na tela
    double altura;
    double angulo = M_PI/3;
    double direcao = 0;


    for (int s = 0; s < lenSaida; s++) {
        if (saida[s] == 'F') {
            double deltaX = tamanhoLinha * cos(direcao);
            double deltaY = tamanhoLinha * sin(direcao);
            double finalX = inicioX + deltaX;
            double finalY = inicioY - deltaY;

            SDL_RenderDrawLine(renderer, inicioX, inicioY, finalX, finalY);

            inicioX = finalX;
            inicioY = finalY;
        } else if (saida[s] == '-') {
            direcao += angulo;
        } else if (saida[s] == '+') {
            direcao -= angulo;
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


include/SDL2include