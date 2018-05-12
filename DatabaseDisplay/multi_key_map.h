#ifndef MULTI_KEY_MAP_H
#define MULTI_KEY_MAP_H
#include <unordered_map>
#include <QTableWidgetItem>
class Multi_key_map
{
private:
    std::unordered_map<int, std::unordered_map<int, QTableWidgetItem*>*> coordinates;
public:
    QTableWidgetItem* getValue(int line, int column);
    void setValue(int line, int column, QTableWidgetItem* value);
    void removeObject(int line, int column);
    void removeLine(int line);
    int ColumnSize(int line);
    int LineSize();
};

#endif // MULTI_KEY_MAP_H
