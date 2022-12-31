#include <iostream>
#include <fstream>
#include <vector>

struct __tall_tree_data {
    int num_trees;
    int tallest;
    int shortest;
};
typedef __tall_tree_data VisibleTrees_t;

struct __tree_data {
    std::vector< std::vector<int>* > Trees;      // array of tree heights
    std::vector<VisibleTrees_t> TopSideTrees;    // data of tallest visible trees on top
    std::vector<VisibleTrees_t> BotSideTrees;
    std::vector<VisibleTrees_t> LeftSideTrees;
    std::vector<VisibleTrees_t> RightSideTrees;
};
typedef __tree_data TreePatchData_t;


TreePatchData_t TreesPatch;

void getTreeData(std::ifstream *trees_file) {
    std::string tree_row;
    while (std::getline(*trees_file, tree_row)) {
        TreesPatch.Trees.push_back(new std::vector<int>);
        for (char t : tree_row) {
            TreesPatch.Trees.back()->push_back(t - '0');
        }
    }
    for (int i : *(TreesPatch.Trees.front())) {
        VisibleTrees_t tallest_top = {0, 0, INT_MAX};
        TreesPatch.TopSideTrees.push_back(tallest_top);
    }
    for (int i : *(TreesPatch.Trees.back())) {
        VisibleTrees_t tallest_bot = {0, 0, INT_MAX};
        TreesPatch.BotSideTrees.push_back(tallest_bot);
    }
    for (std::vector<int>* i : TreesPatch.Trees) {
        VisibleTrees_t tallest_left = {0, 0, INT_MAX};
        VisibleTrees_t tallest_right = {0, 0, INT_MAX};
        TreesPatch.LeftSideTrees.push_back(tallest_left);
        TreesPatch.RightSideTrees.push_back(tallest_right);
    }
}

void checkTree(int tree_height, int row, int col) {
    // check top
    if (tree_height > TreesPatch.TopSideTrees[col].tallest) {
        TreesPatch.TopSideTrees[col].tallest = tree_height;
        TreesPatch.TopSideTrees[col].num_trees++;
    }

    // check bottom
    if (tree_height >= TreesPatch.BotSideTrees[col].tallest) {
        TreesPatch.BotSideTrees[col].tallest = tree_height;
        TreesPatch.BotSideTrees[col].num_trees = 1;
    }
    else {
        TreesPatch.BotSideTrees[col].num_trees++;
    }

    // check left
    if (tree_height > TreesPatch.LeftSideTrees[row].tallest) {
        TreesPatch.LeftSideTrees[row].tallest = tree_height;
        TreesPatch.LeftSideTrees[row].num_trees++;
    }

    // check right
    if (tree_height >= TreesPatch.RightSideTrees[row].tallest) {
        TreesPatch.RightSideTrees[row].tallest = tree_height;
        TreesPatch.RightSideTrees[row].num_trees = 1;
    }
    else {
        TreesPatch.RightSideTrees[row].num_trees++;
    }
}

void checkAllTrees() {
    for (int i = 0; i < TreesPatch.Trees.size(); i++) {
        for (int j = 0; j < TreesPatch.Trees[i]->size(); j++) {
            checkTree(TreesPatch.Trees[i]->at(j), i, j);
        }
    }
}

void showTrees() {
    std::cout << std::endl << "CURRENT TREE PATCH" << std::endl << "  ";
    for (VisibleTrees_t t : TreesPatch.TopSideTrees) {
        std::cout << t.num_trees;
    }
    std::cout << std::endl;
    for (int i = 0; i < TreesPatch.Trees.size(); i++) {
        std::cout << TreesPatch.LeftSideTrees[i].num_trees << " ";
        for (int j : *(TreesPatch.Trees.at(i))) {
            std::cout << j;
        }
        std::cout << " " << TreesPatch.RightSideTrees[i].num_trees << std::endl;
    }
    std::cout << "  ";
    for (VisibleTrees_t t : TreesPatch.BotSideTrees) {
        std::cout << t.num_trees;
    }
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream tree_data(argv[1], std::ios::in);
    if (!tree_data.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    getTreeData(&tree_data);
    checkAllTrees();
    showTrees();

    return 0;
}