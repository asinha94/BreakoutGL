#include "GameLevel.h"
#include "resource_manager.h"


namespace Bout {

    void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
    {
        // clear old data
        this->Bricks.clear();
        // load from file
        unsigned int tileCode;
        GameLevel level;
        std::string line;
        std::ifstream fstream(file);
        std::vector<std::vector<unsigned int>> tileData;
        if (fstream)
        {
            while (std::getline(fstream, line)) // read each line from level file
            {
                std::istringstream sstream(line);
                std::vector<unsigned int> row;
                while (sstream >> tileCode) // read each word separated by spaces
                    row.push_back(tileCode);
                tileData.push_back(row);
            }
            if (tileData.size() > 0)
                this->init(tileData, levelWidth, levelHeight);
        }
        else {
            std::cout << "Couldn't find level file: " << file << '\n';
        }
    }


    void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int lvlWidth, unsigned int lvlHeight)
    {
        unsigned int height = tileData.size();
        unsigned int width = tileData[0].size();
        float unit_width = lvlWidth / static_cast<float>(width);
        float unit_height = lvlHeight / height;

        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {

                if (tileData[y][x] == 0)
                    continue;

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);

                if (tileData[y][x] == 1) {
                    

                    auto solid = ResourceManager::GetTexture("block_solid");
                    GameObject obj(pos, size, solid, glm::vec3(0.8f, 0.8f, 0.7f));
                    obj.IsSolid = true;
                    this->Bricks.push_back(obj);
                    continue;
                }

                glm::vec3 color = glm::vec3(1.0f);
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                this->Bricks.push_back(
                    GameObject(pos, size, ResourceManager::GetTexture("block"), color)
                );
            }
        }
    }

    void GameLevel::Draw(SpriteRenderer& renderer)
    {
        for (GameObject& brick : this->Bricks)
            if (!brick.Destroyed)
                brick.Draw(renderer);
    }

    bool GameLevel::IsCompleted()
    {
        for (GameObject& brick : this->Bricks)
            if (!brick.IsSolid && !brick.Destroyed)
                return false;
        return true;
    }

}


