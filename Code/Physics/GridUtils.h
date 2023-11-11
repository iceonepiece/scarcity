#pragma once

#include <map>

struct GridComponent;

std::map<std::pair<int, int>, int> floodFill(std::map<std::pair<int, int>, int>& cellMap);
void GenerateGridCollision(GridComponent& grid);