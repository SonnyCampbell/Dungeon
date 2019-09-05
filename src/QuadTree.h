#include <vector>
#include "SDL.h"
#include "Game Components/Game.h"

class QuadTree
{
private:
    const int max_objects = 1;
    const int max_levels = 5;

    int level;
    std::vector<SDL_Rect> objects; //TODO - Some sort of transformation object with entity id
    SDL_Rect bounds;
    std::vector<QuadTree> nodes;

public:
    QuadTree(int p_level, SDL_Rect p_bounds);
    ~QuadTree();

    void clear();
    void split();
    int getIndex(SDL_Rect rect);
    void insert(SDL_Rect rect);
    std::vector<SDL_Rect> retrieve(std::vector<SDL_Rect> &return_list, SDL_Rect rect);
    void draw(SDL_Renderer *renderer);
};

QuadTree::QuadTree(int p_level, SDL_Rect p_bounds)
{
    level = p_level;
    objects = std::vector<SDL_Rect>();
    bounds = p_bounds;
    nodes = std::vector<QuadTree>();
}

QuadTree::~QuadTree()
{
}

void QuadTree::clear()
{
    objects.clear();

    for (int i = 0; i < nodes.size(); i++)
    {
        nodes[i].clear();
    }

    nodes.clear();
}
void QuadTree::split()
{
    printf("Splitting quad %i %i %i %i \n", bounds.x, bounds.y, bounds.w, bounds.h);

    int sub_width = (int)(bounds.w / 2);
    int sub_height = (int)(bounds.h / 2);
    int x = bounds.x;
    int y = bounds.y;

    nodes.push_back(QuadTree(level + 1, {x + sub_width, y, sub_width, sub_height}));
    nodes.push_back(QuadTree(level + 1, {x, y, sub_width, sub_height}));
    nodes.push_back(QuadTree(level + 1, {x, y + sub_height, sub_width, sub_height}));
    nodes.push_back(QuadTree(level + 1, {x + sub_width, y + sub_height, sub_width, sub_height}));
}

/*
 * Determine which node the object belongs to. -1 means
 * object cannot completely fit within a child node and is part
 * of the parent node
 */
int QuadTree::getIndex(SDL_Rect rect)
{
    int index = -1;
    double vertical_midpoint = bounds.x + (bounds.w / 2);
    double horizontal_midpoint = bounds.y + (bounds.h / 2);

    // Object can completely fit within the top quadrants
    bool topQuadrant = (rect.y < horizontal_midpoint && rect.y + rect.h < horizontal_midpoint);
    // Object can completely fit within the bottom quadrants
    bool bottomQuadrant = (rect.y > horizontal_midpoint);

    // Object can completely fit within the left quadrants
    if (rect.x < vertical_midpoint && rect.x + rect.w < vertical_midpoint)
    {
        if (topQuadrant)
        {
            index = 1;
        }
        else if (bottomQuadrant)
        {
            index = 2;
        }
    }
    // Object can completely fit within the right quadrants
    else if (rect.x > vertical_midpoint)
    {
        if (topQuadrant)
        {
            index = 0;
        }
        else if (bottomQuadrant)
        {
            index = 3;
        }
    }

    return index;
}

void QuadTree::insert(SDL_Rect rect)
{
    if (!nodes.empty())
    {
        int index = getIndex(rect);

        if (index != -1)
        {
            nodes[index].insert(rect);

            return;
        }
    }

    objects.push_back(rect);

    if (objects.size() > max_objects && level < max_levels)
    {
        if (nodes.empty())
        {
            split();
        }

        int i = 0;
        while (i < objects.size())
        {
            int index = getIndex(objects[i]);
            if (index != -1)
            {
                auto object = objects[i];

                std::swap(objects[i], objects.back());
                objects.pop_back();

                nodes[index].insert(object);
            }
            else
            {
                i++;
            }
        }
    }
}

std::vector<SDL_Rect> QuadTree::retrieve(std::vector<SDL_Rect> &return_list, SDL_Rect rect)
{
    int index = getIndex(rect);
    if (index != -1 && !nodes.empty())
    {
        nodes[index].retrieve(return_list, rect);
    }

    for (auto object : objects)
    {
        return_list.push_back(object);
    }

    return return_list;
}

void QuadTree::draw(SDL_Renderer *renderer)
{
    for (auto node : nodes)
    {
        node.draw(renderer);
    }

    SDL_Rect debug_rect = {bounds.x - Game::camera.x, bounds.y - Game::camera.y, bounds.w, bounds.h};
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 00, 0xFF);
    SDL_RenderDrawRect(renderer, &debug_rect);
}
