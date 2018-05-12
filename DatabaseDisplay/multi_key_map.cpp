#include "multi_key_map.h"

QTableWidgetItem* Multi_key_map::getValue(int line, int column)
{
    if(coordinates.find(line) == coordinates.end()) return nullptr;
    std::unordered_map<int, QTableWidgetItem*> *temp = coordinates[line];
    if(temp->find(column) == temp->end())
        return nullptr;
    return (*temp)[column];
}
void Multi_key_map::setValue(int line, int column, QTableWidgetItem* value)
{
    if(coordinates.find(line) == coordinates.end())
        coordinates[line] = new std::unordered_map<int, QTableWidgetItem*>;
    (*(coordinates[line]))[column] = value;
}
void Multi_key_map::removeObject(int line, int column)
{
    std::unordered_map<int, QTableWidgetItem*> *temp = coordinates[line];
    temp->erase(temp->find(column));
}
int Multi_key_map::ColumnSize(int line)
{
    if(coordinates.find(line) == coordinates.end()) return 0;
    return coordinates[line]->size();
}
int Multi_key_map::LineSize()
{
    return coordinates.size();
}
void Multi_key_map::removeLine(int line)
{
    coordinates.erase(coordinates.find(line));
}
