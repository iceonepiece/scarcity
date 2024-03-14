#include "GridUtils.h"
#include "Components/GridComponent.h"

std::map<std::pair<int, int>, int> floodFill(std::map<std::pair<int, int>, int>& cellMap)
{
    std::map<std::pair<int, int>, int> connected;
    std::stack<std::pair<int, int>> pixels;
    std::pair<int, int> current = cellMap.begin()->first;
    int prevColor = (cellMap.begin())->second;

    pixels.push(current);

    while (!pixels.empty()) {
        current = pixels.top();
        pixels.pop();
        int i = current.first;
        int j = current.second;

        if (cellMap.find(current) == cellMap.end() || cellMap.at(current) != prevColor)
            continue;

        connected.insert({ current, cellMap.at(current) });
        cellMap.erase(current);

        pixels.push(std::make_pair(i + 1, j));
        pixels.push(std::make_pair(i - 1, j));
        pixels.push(std::make_pair(i, j + 1));
        pixels.push(std::make_pair(i, j - 1));
    }

    return connected;
}

void GenerateGridCollision(GridComponent& grid)
{
    std::map<std::pair<int, int>, int> cellMap = grid.cellMap;
    grid.polygons.clear();

    while (cellMap.size() > 0)
    {
        std::map<std::pair<int, int>, int> connected = floodFill(cellMap);

        GridPolygon polygon;

        std::pair<int, int> terminatePos = connected.begin()->first;
        std::pair<int, int> currentPos = terminatePos;
        EdgeOnCell currentEdge = EdgeOnCell::Left;

        std::pair<int, int> startCell = terminatePos;

        while (connected.size())
        {
            int x = currentPos.first;
            int y = currentPos.second;

            bool hasChanged = false;
            std::pair<int, int> nextPos = currentPos;
            EdgeOnCell nextEdge = currentEdge;

            switch (currentEdge)
            {
            case EdgeOnCell::Left:
            {
                if (connected.find(std::make_pair(x - 1, y - 1)) != connected.end())
                {
                    nextPos.first = x - 1;
                    nextPos.second = y - 1;
                    nextEdge = EdgeOnCell::Top;
                    hasChanged = true;
                }
                else if (connected.find(std::make_pair(x, y - 1)) != connected.end())
                {
                    nextPos.first = x;
                    nextPos.second = y - 1;
                }
                else
                {
                    nextEdge = EdgeOnCell::Bottom;
                    hasChanged = true;
                }
            }
            break;

            case EdgeOnCell::Bottom:
            {
                if (connected.find(std::make_pair(x + 1, y - 1)) != connected.end())
                {
                    nextPos.first = x + 1;
                    nextPos.second = y - 1;
                    nextEdge = EdgeOnCell::Left;
                    hasChanged = true;
                }
                else if (connected.find(std::make_pair(x + 1, y)) != connected.end())
                {
                    nextPos.first = x + 1;
                    nextPos.second = y;
                }
                else
                {
                    nextEdge = EdgeOnCell::Right;
                    hasChanged = true;
                }

            }
            break;

            case EdgeOnCell::Right:
            {
                if (connected.find(std::make_pair(x + 1, y + 1)) != connected.end())
                {
                    nextPos.first = x + 1;
                    nextPos.second = y + 1;
                    nextEdge = EdgeOnCell::Bottom;

                    hasChanged = true;
                }
                else if (connected.find(std::make_pair(x, y + 1)) != connected.end())
                {
                    nextPos.first = x;
                    nextPos.second = y + 1;
                }
                else
                {
                    nextEdge = EdgeOnCell::Top;
                    hasChanged = true;
                }
            }
            break;

            case EdgeOnCell::Top:
            {
                if (connected.find(std::make_pair(x - 1, y + 1)) != connected.end())
                {
                    nextPos.first = x - 1;
                    nextPos.second = y + 1;
                    nextEdge = EdgeOnCell::Right;
                    hasChanged = true;
                }
                else if (connected.find(std::make_pair(x - 1, y)) != connected.end())
                {
                    nextPos.first = x - 1;
                    nextPos.second = y;
                }
                else
                {
                    nextEdge = EdgeOnCell::Left;
                    hasChanged = true;
                }
            }
            break;

            }

            if (nextPos == terminatePos && nextEdge == EdgeOnCell::Left)
            {
                if (currentEdge == polygon[0].onCell)
                    polygon[0].startCell = startCell;
                else
                    polygon.push_back({ startCell, currentPos, currentEdge });

                break;
            }

            if (hasChanged)
            {                
                std::string onCell = "Left";

                if (currentEdge == EdgeOnCell::Right)
                    onCell = "Right";
                else if (currentEdge == EdgeOnCell::Bottom)
                    onCell = "Bottom";
                else if (currentEdge == EdgeOnCell::Top)
                    onCell = "Top";

                polygon.push_back({ startCell, currentPos, currentEdge });
                startCell = nextPos;
            }

            currentEdge = nextEdge;
            currentPos = nextPos;
        }

        for (auto& edge : polygon)
        {
            std::string onCell = "Left";

            if (edge.onCell == EdgeOnCell::Right)
                onCell = "Right";
            else if (edge.onCell == EdgeOnCell::Bottom)
                onCell = "Bottom";
            else if (edge.onCell == EdgeOnCell::Top)
                onCell = "Top";
        }

        grid.polygons.push_back(polygon);
    }
}