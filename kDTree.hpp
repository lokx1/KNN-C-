#include "main.hpp"
#include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
vector<vector<int>> convertListToVector(const list<list<int>>& lst); 
list<list<int>> convertVectorToList(const vector<vector<int>>& vec);
void insertionSort(vector<vector<int>>& points, int axis);
struct kDTreeNode
{
    vector<int> data;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->left = left;
        this->right = right;
    }
    void print() const
    {
    
        cout << "(";
       
        for(int i = 0; i < data.size(); i++)
        {
           
            cout << data[i];
            
            if (i == data.size() - 1) cout << ")";
            else cout << ",";
        }
      
        cout << "\n";
    }
};

class kDTree
{
private:
    int k;
    kDTreeNode *root;
    int count;
    void inorderTraversalRec(kDTreeNode *node) const;
    void preorderTraversalRec(kDTreeNode *node) const;
    void postorderTraversalRec(kDTreeNode *node) const;
    
public:
    kDTree(int k = 2);
    ~kDTree();
    kDTreeNode* copy(const kDTreeNode *temp);
    const kDTree &operator=(const kDTree &other);
    kDTree(const kDTree &other);

    
    int heightRec(kDTreeNode *node) const;
   
    void insertSorted(vector<kDTreeNode *> &list, kDTreeNode *node, const vector<int> &target);
    
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
    int distanceSquared(const vector<int> &a, const vector<int> &b);
    void nearestNeighbour(const vector<int> &target, kDTreeNode *best,kDTreeNode *node, int level);
    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList);
   
    void kNearestNeighbourRec(kDTreeNode* node, const vector<int>& target, int k, vector<kDTreeNode*>& bestList, int level);
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
    kNN(int k = 5);
    void fit(Dataset &X_train, Dataset &y_train);
    Dataset predict(Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);
    
};

// Please add more or modify as needed
