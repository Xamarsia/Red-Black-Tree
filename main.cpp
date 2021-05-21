#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <vector>
#include "tree.h"
#include "treeUtils.h"

int main()
{
    std::cout << ".............................................." << std::endl;
    Tree<int, std::string> temp;
    temp.add(8, "11111");
    temp.add(5, "22222");
    temp.add(18, "33333");
    temp.add(15, "44444");
    temp.add(17, "55555");
    temp.add(25, "66666");
    temp.add(40, "77777");
    temp.add(80, "88888");
    temp.add(4, "99999");
    temp.add(6, "000000");
    std::cout << temp[8] << std::endl;
    auto t = temp.keys();
    const std::vector<int> &p = t;
    print<int>(p);

    keyValue<int, std::string>(temp);

    std::cout << "\n"
              << temp.contains(999) << " " << temp.contains(6) << std::endl;
}