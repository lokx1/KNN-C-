#include "main.hpp"
#include "Dataset.hpp"

/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */

struct kDTreeNode
{
    vector<int> data;
    int label;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->label = 0;
        this->left = left;
        this->right = right;
    }
    kDTreeNode(vector<int> data, int label, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->label = label;
        this->left = nullptr;
        this->right = nullptr;
    }

    friend ostream &operator<<(ostream &os, const kDTreeNode &node)
    {
        os << "(";
        for (int i = 0; i < node.data.size(); i++)
        {
            os << node.data[i];
            if (i != node.data.size() - 1)
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
    
};

class kDTree
{
private:
    int k;
    kDTreeNode *root;
    int count;
   
    
public:
    kDTree(int k = 2);
    ~kDTree();
    kDTreeNode* copy(const kDTreeNode *temp);
    const kDTree &operator=(const kDTree &other);
    kDTree(const kDTree &other);
  
      void clear(kDTreeNode *node);
    int heightRec(kDTreeNode *node) const;
  
    
     void inorderTraversalRec(kDTreeNode *node) const;
    void preorderTraversalRec(kDTreeNode *node) const;
    void postorderTraversalRec(kDTreeNode *node) const;
    void preorderTraversal() const;
    void inorderTraversal() const;
    void postorderTraversal() const;
  
   
    int height() const;
    int nodeCount() const;
    int leafCount() const;
    int leafCountRec(kDTreeNode *node) const;
    kDTreeNode* recInsert(kDTreeNode *node, const vector<int> &point, int level);
    void insert(const vector<int> &point);
    kDTreeNode* findMin(kDTreeNode *node, int d, int level);
    kDTreeNode* recRemove(kDTreeNode *node, const vector<int> &point, int level);
    void remove(const vector<int> &point);
    bool recSearch(kDTreeNode *node, const vector<int> &point, int level) const;
    bool search(const vector<int> &point);
    kDTreeNode *recBuildTree(const vector<vector<int>> &pointList, int level);
    void buildTree(const vector<vector<int>> &pointList);
   double distanceSquared(const vector<int> &a, const vector<int> &b);
    void nearestNeighbourRec(kDTreeNode* temp, const vector<int>& target, kDTreeNode* &best, int level);
    void nearestNeighbour(const vector<int> &target, kDTreeNode *&best);
    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList);

    void kNearestNeighbourRec(kDTreeNode* temp, const vector<int>& target, int k, vector<kDTreeNode*>& bestList, int level);
   kDTreeNode * buildTreeLableRec(const vector<vector<int>> &pointList, const vector<int> &label,int level);
    void buildTreeLable(const vector<vector<int>> &pointList,const vector<int> &label);
   friend class kNN;
};

class kNN
{
private:
    int k;
    Dataset *X_train;
    Dataset *y_train;
    int numClasses;
    kDTree train;
public:
    kNN(int k = 5):k(k){}
    void fit(Dataset &X_train, Dataset &y_train);
    Dataset predict(Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);
    void print_Y(const Dataset& y)
{
    OUTPUT  << y.columnName[0] << ": ";
    for (auto it : y.data)
    {
        OUTPUT  << it.front() << " ";
    }
    OUTPUT << endl;

}
};

// Please add more or modify as needed
int mostFrequentLabel(const std::vector<kDTreeNode*>& nodeList);