#include <vector>
#include "SDL.h"
#include "Game Components/Game.h"

struct QuadCollionObject
{
    int id;
    SDL_Rect collision_rect;
};

class QuadTree
{
private:
    const int max_objects = 1;
    const int max_levels = 3;

    int level;
    std::vector<QuadCollionObject> objects; //TODO - Some sort of transformation object with entity id
    SDL_Rect bounds;
    std::vector<QuadTree> nodes;

public:
    QuadTree(int p_level, SDL_Rect p_bounds);
    ~QuadTree();

    void clear();
    void split();
    int getIndex(QuadCollionObject rect);
    void insert(QuadCollionObject rect);
    void retrieve(std::vector<QuadCollionObject> &return_list, QuadCollionObject rect);
    void draw(SDL_Renderer *renderer);
};

QuadTree::QuadTree(int p_level, SDL_Rect p_bounds)
{
    level = p_level;
    objects = std::vector<QuadCollionObject>();
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
    //printf("Splitting quad %i %i %i %i \n", bounds.x, bounds.y, bounds.w, bounds.h);

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
int QuadTree::getIndex(QuadCollionObject object)
{
    int index = -1;
    double vertical_midpoint = bounds.x + (bounds.w / 2);
    double horizontal_midpoint = bounds.y + (bounds.h / 2);

    // Object can completely fit within the top quadrants
    bool topQuadrant = (object.collision_rect.y < horizontal_midpoint && object.collision_rect.y + object.collision_rect.h < horizontal_midpoint);
    // Object can completely fit within the bottom quadrants
    bool bottomQuadrant = (object.collision_rect.y > horizontal_midpoint);

    // Object can completely fit within the left quadrants
    if (object.collision_rect.x < vertical_midpoint && object.collision_rect.x + object.collision_rect.w < vertical_midpoint)
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
    else if (object.collision_rect.x > vertical_midpoint)
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

void QuadTree::insert(QuadCollionObject object)
{
    if (!nodes.empty())
    {
        int index = getIndex(object);

        if (index != -1)
        {
            nodes[index].insert(object);

            return;
        }
    }

    objects.push_back(object);

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

void QuadTree::retrieve(std::vector<QuadCollionObject> &return_list, QuadCollionObject object)
{
    int index = getIndex(object);
    if (index != -1 && !nodes.empty())
    {
        nodes[index].retrieve(return_list, object);
    }

    for (auto object : objects)
    {
        return_list.push_back(object);
    }

    // TODO: More thought into how to handle this
    // If player on border, need to add collision
    // objects from sibling quads, but what sort
    // of effect does this have on performance?
    if (index == -1 && !nodes.empty())
    {
        for (int i = 0; i < 4; i++)
        {
            nodes[i].retrieve(return_list, object);
        }
    }
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
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
}
