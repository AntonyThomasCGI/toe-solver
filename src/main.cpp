
#include <iostream>
#include <memory>
#include <random>
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>

#include "engine/app.hpp"
#include "engine/engine.hpp"
#include "engine/components/material.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/transform.hpp"


const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 900;

const unsigned int GRID_WIDTH = WIDTH / 40;
const unsigned int GRID_HEIGHT = HEIGHT / 40;


class Grid
{
public:
    const float tileWidth = 40.0;

    Grid(Engine *engine) {

        float halfWidth = (WIDTH - tileWidth) / 2.0f;
        float halfHeight = (HEIGHT - tileWidth) / 2.0f;

        std::cout << "Building grid (" << GRID_WIDTH << ", " << GRID_HEIGHT << ")" << std::endl;

        std::shared_ptr<Material> tileMat = std::make_shared<Material>(
            *engine->graphics->ctx, *engine->graphics->commandPool, *engine->graphics->swapChain);

        tileMat->setTexturePath("resources/textures/dirt.png");
        tileMat->setShader(
            *engine->graphics->swapChain, "resources/shaders/flat_vert.spv", "resources/shaders/flat_frag.spv");
        tileMat->setMaxMeshCount(10000);

        std::shared_ptr<MaterialComponent> tileMatComponent = std::make_shared<MaterialComponent>();
        tileMatComponent->material = tileMat;

        //std::random_device dev;
        //std::mt19937 rng(dev());
        //std::uniform_int_distribution<std::mt19937::result_type> dist10(1,15);

        for (int i = 0; i < GRID_HEIGHT; i++ ) {
            for (int j = 0; j < GRID_WIDTH; j++ ) {
                //std::stringstream s;
                //s << "tile_" << std::to_string(i) << "_" << std::to_string(j);
                grid[i][j] = std::make_shared<GameObject>();
                engine->graphics->addGameObject(grid[i][j]);

                float yPos = i * tileWidth - halfHeight;
                float xPos = j * tileWidth - halfWidth;

                std::shared_ptr<TransformComponent> tc = std::make_shared<TransformComponent>();

                tc->translate = glm::vec2(xPos, yPos);
                tc->scale = glm::vec2(tileWidth);

                grid[i][j]->addComponent(tc);

                std::shared_ptr<MeshComponent> mc = std::make_shared<MeshComponent>();
                std::shared_ptr<Mesh> square = std::make_shared<Square>(*engine->graphics->ctx);
                square->createBuffers(*engine->graphics->commandPool);
                mc->mesh = square;

                grid[i][j]->addComponent(mc);
                grid[i][j]->addComponent(tileMatComponent);
            }
        }
    }

    std::shared_ptr<GameObject> gameObjectAt(float worldY, float worldX) {
        float halfWidth = (WIDTH - tileWidth) / 2.0f;
        float halfHeight = (HEIGHT - tileWidth) / 2.0f;

        int scaledY = (worldY + halfHeight) / tileWidth;
        int scaledX = (worldX + halfWidth) / tileWidth;

        return grid[scaledY][scaledX];
    }

private:
    std::shared_ptr<GameObject> grid[GRID_HEIGHT][GRID_WIDTH];
};



class App : public AppBase
{
public:
    using AppBase::AppBase;

    App(unsigned int width, unsigned int height, int argc, char **argv) : AppBase(width, height, argc, argv)
    {
        grid = std::make_unique<Grid>(engine);

    }

    ~App()
    {
    }

    void update(float deltaTime) {
    }

    void processInput(float deltaTime) {
        if (keys[GLFW_KEY_ESCAPE]) {
            exitApp();
            return;
        }

    }

private:
    std::unique_ptr<Grid> grid;
};


int main(int argc, char** argv) {
    try {
        App app = App(WIDTH, HEIGHT, argc, argv);
        app.mainLoop();
    } catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
};
