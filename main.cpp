#include <SDL3/SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

const int WINDOW_SIZE = 600;
const int GRID_SIZE = 5;
const int BOARD_SIZE = 500;
const int GRID_OFFSET = 50;
const int CELL_SIZE = BOARD_SIZE / GRID_SIZE;
const int MAX_TURNS = 7;

// Etat possible pour chaque case
enum CellState
{
    EMPTY,
    SEED,
    DANDELION
};

CellState grid[GRID_SIZE][GRID_SIZE];

int turns = 0;
bool gameOver = false;

std::string lastWindDirection = "Aucun";
std::string gameMessage = "Clique pour planter un pissenlit";

// Directions possibles du vent
int directions[8][2] =
{
    {-1, 0},   // Nord
    {1, 0},    // Sud
    {0, 1},    // Est
    {0, -1},   // Ouest
    {-1, 1},   // Nord-Est
    {-1, -1},  // Nord-Ouest
    {1, 1},    // Sud-Est
    {1, -1}    // Sud-Ouest
};

const char* directionNames[8] =
{
    "Nord",
    "Sud",
    "Est",
    "Ouest",
    "Nord-Est",
    "Nord-Ouest",
    "Sud-Est",
    "Sud-Ouest"
};

bool usedDirections[8] = { false };

void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius)
{
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            if (x * x + y * y <= radius * radius)
            {
                SDL_RenderPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

void updateWindowTitle(SDL_Window* window)
{
    std::string title = "Jeu du Pissenlit | Tour " + std::to_string(turns) + "/" +
                        std::to_string(MAX_TURNS) + " | Vent: " + lastWindDirection +
                        " | " + gameMessage;

    SDL_SetWindowTitle(window, title.c_str());
}

void resetGame(SDL_Window* window)
{
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            grid[row][col] = EMPTY;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        usedDirections[i] = false;
    }

    turns = 0;
    gameOver = false;
    lastWindDirection = "Aucun";
    gameMessage = "Clique pour planter un pissenlit";

    std::cout << "Nouvelle partie." << std::endl;
    updateWindowTitle(window);
}

bool isGridFull()
{
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == EMPTY)
            {
                return false;
            }
        }
    }

    return true;
}

int getRandomUnusedDirection()
{
    int direction;

    do
    {
        direction = rand() % 8;
    }
    while (usedDirections[direction]);

    usedDirections[direction] = true;
    return direction;
}

void spreadSeeds(int directionIndex)
{
    int rowDir = directions[directionIndex][0];
    int colDir = directions[directionIndex][1];

    CellState newGrid[GRID_SIZE][GRID_SIZE];

    // Copie de la grille pour éviter de modifier pendant la lecture
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            newGrid[row][col] = grid[row][col];
        }
    }

    // Chaque pissenlit envoie des graines dans la direction du vent
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == DANDELION)
            {
                int currentRow = row + rowDir;
                int currentCol = col + colDir;

                while (currentRow >= 0 && currentRow < GRID_SIZE &&
                       currentCol >= 0 && currentCol < GRID_SIZE)
                {
                    if (newGrid[currentRow][currentCol] == EMPTY)
                    {
                        newGrid[currentRow][currentCol] = SEED;
                    }

                    currentRow += rowDir;
                    currentCol += colDir;
                }
            }
        }
    }

    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            grid[row][col] = newGrid[row][col];
        }
    }
}

void drawGame(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 210, 225, 200, 255);
    SDL_RenderClear(renderer);

    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            int x = GRID_OFFSET + col * CELL_SIZE;
            int y = GRID_OFFSET + row * CELL_SIZE;

            SDL_FRect cell;
            cell.x = x;
            cell.y = y;
            cell.w = CELL_SIZE;
            cell.h = CELL_SIZE;

            // Fond de la case
            SDL_SetRenderDrawColor(renderer, 245, 240, 220, 255);
            SDL_RenderFillRect(renderer, &cell);

            int centerX = x + CELL_SIZE / 2;
            int centerY = y + CELL_SIZE / 2;

            if (grid[row][col] == SEED)
            {
                SDL_SetRenderDrawColor(renderer, 90, 180, 90, 255);
                drawFilledCircle(renderer, centerX, centerY, 12);
            }
            else if (grid[row][col] == DANDELION)
            {
                SDL_SetRenderDrawColor(renderer, 245, 210, 40, 255);
                drawFilledCircle(renderer, centerX, centerY, 22);

                SDL_SetRenderDrawColor(renderer, 80, 150, 60, 255);
                drawFilledCircle(renderer, centerX, centerY, 8);
            }

            // Bordure de la case
            SDL_SetRenderDrawColor(renderer, 80, 100, 70, 255);
            SDL_RenderRect(renderer, &cell);
        }
    }

    SDL_RenderPresent(renderer);
}

int main()
{
    srand((unsigned int)time(nullptr));

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Erreur SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Jeu du Pissenlit", WINDOW_SIZE, WINDOW_SIZE, 0);

    if (window == nullptr)
    {
        std::cout << "Erreur window : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (renderer == nullptr)
    {
        std::cout << "Erreur renderer : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    resetGame(window);

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_R)
                {
                    resetGame(window);
                }
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && !gameOver)
            {
                int mouseX = (int)event.button.x;
                int mouseY = (int)event.button.y;

                int col = (mouseX - GRID_OFFSET) / CELL_SIZE;
                int row = (mouseY - GRID_OFFSET) / CELL_SIZE;

                if (mouseX >= GRID_OFFSET && mouseX < GRID_OFFSET + BOARD_SIZE &&
                    mouseY >= GRID_OFFSET && mouseY < GRID_OFFSET + BOARD_SIZE &&
                    row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE)
                {
                    grid[row][col] = DANDELION;

                    int windDirection = getRandomUnusedDirection();
                    lastWindDirection = directionNames[windDirection];

                    std::cout << "Vent : " << lastWindDirection << std::endl;

                    spreadSeeds(windDirection);
                    turns++;

                    std::cout << "Tour " << turns << "/" << MAX_TURNS << std::endl;

                    if (isGridFull())
                    {
                        gameMessage = "Les Pissenlits gagnent ! Appuie sur R";
                        std::cout << "Les Pissenlits gagnent !" << std::endl;
                        gameOver = true;
                    }
                    else if (turns >= MAX_TURNS)
                    {
                        gameMessage = "Le Vent gagne ! Appuie sur R";
                        std::cout << "Le Vent gagne !" << std::endl;
                        gameOver = true;
                    }
                    else
                    {
                        gameMessage = "Clique pour continuer";
                    }

                    updateWindowTitle(window);
                }
            }
        }

        drawGame(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}