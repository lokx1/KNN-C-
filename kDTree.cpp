#include "kDTree.hpp"
#include "main.hpp"
/* TODO: You can implement methods, functions that support your data structures here.
 * */


kDTreeNode* kDTree::copy(const kDTreeNode * temp)
{
    if(temp==nullptr) return nullptr;
    else {
        kDTreeNode *newNode = new kDTreeNode(temp->data);
        newNode->data=temp->data;
        newNode->left=copy(temp->left);
        newNode->right=copy(temp->right);
        return newNode;
    }

}

const kDTree &kDTree::operator=(const kDTree &other)
{
    // TODO: insert return statement here
    if (this != &other) { 
        delete this->root;  
        this->k = other.k;
        this->count = other.count;
        this->root = this->copy(other.root);
    }
    return *this;
  
}

kDTree::kDTree(const kDTree &other)
{
    this->k = other.k;
    this->count = other.count;
    this->root=this->copy(other.root);
}

void kDTree::   inorderTraversal() const
{   
    inorderTraversalRec(root); 
}

void kDTree::inorderTraversalRec(kDTreeNode *node) const
{
    if (node==nullptr) return;
    
        inorderTraversalRec(node->left);
        
        
        node->print();
        
       
        inorderTraversalRec(node->right);
    
    
}   

void kDTree::preorderTraversal() const
{
    preorderTraversalRec(root);
}

void kDTree::preorderTraversalRec(kDTreeNode *node) const
{
    if(node==nullptr) return;
       
       
           
        node->print();
        
        
        preorderTraversalRec(node->left);
        preorderTraversalRec(node->right);
        

}

void kDTree::postorderTraversal() const
{
    postorderTraversalRec(root);
}

int kDTree::height() const
{
    return heightRec(root);
}

int kDTree::heightRec(kDTreeNode *node) const
{
    if (node == nullptr) {
        return 0;
    } else {
        int leftHeight = heightRec(node->left);
        int rightHeight = heightRec(node->right);
        return 1 + max(leftHeight, rightHeight);
    }
}

int kDTree::nodeCount() const
{
    return this->count;

}

int kDTree::leafCount() const
{
    return leafCountRec(root);
}

int kDTree::leafCountRec(kDTreeNode *node) const
{
    if(node==nullptr) return 0;
    if(node->left==nullptr && node->right==nullptr) return 1;
    else return leafCountRec(node->left) + leafCountRec(node->right);
}

kDTreeNode* kDTree::recInsert(kDTreeNode *node, const vector<int> &point, int level)
{
    if(node==nullptr){
        
        return new kDTreeNode(point);
    }
    int axis = level % k;
    if(point[axis] < node->data[axis]){
        node->left = recInsert(node->left,point,level+1);
    }
    else{
        node->right = recInsert(node->right,point,level+1);
    }   
    return node;
}

void kDTree::insert(const vector<int> &point)
{
    this->root=this->recInsert(root,point,0);
    this->count++;

}

kDTreeNode *kDTree::findMin(kDTreeNode *node, int d, int level)
{
     if (node == nullptr) {
        return nullptr; // Bước 1: Điều kiện dừng
    }

    int alpha = level % k; // Tính trục hiện tại

    if (alpha == d) {
        // Bước 2: Nếu node đang xét theo chiều alpha
        if (node->left != nullptr) {
            // Tiếp tục tìm kiếm bên trái nếu có
            return findMin(node->left, d, level + 1);
        } else {
            // Con bên trái không có, node hiện tại là nhỏ nhất
            return node;
        }
    } else {
        // Bước 3: Nếu node đang xét khác chiều alpha
        kDTreeNode* leftMin = findMin(node->left, d, level + 1);
        kDTreeNode* rightMin = findMin(node->right, d, level + 1);

        // So sánh để tìm giá trị nhỏ nhất giữa node hiện tại, bên trái và bên phải
        kDTreeNode* minNode = node; // Giả định node hiện tại là nhỏ nhất
        if (leftMin != nullptr && leftMin->data[d] < minNode->data[d]) {
            minNode = leftMin;
        }
        if (rightMin != nullptr && rightMin->data[d] < minNode->data[d]) {
            minNode = rightMin;
        }
        return minNode;
    }
}

kDTreeNode *kDTree::recRemove(kDTreeNode *node, const vector<int> &point, int level)
{ 
    if (node == nullptr) {
        return nullptr; // Bước 1: Điều kiện dừng, không tìm thấy nút cần xóa
    }

    int cd = level % k; // Chiều hiện tại dựa trên level

    // Kiểm tra nếu node hiện tại chứa điểm cần xóa
    if (node->data == point) {
        // Bước 2: Xóa nếu là node lá
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        // Bước 3: Xử lý nếu node không phải là lá
        if (node->right != nullptr) {
            // Ưu tiên tìm node nhỏ nhất theo chiều alpha từ cây con bên phải
            kDTreeNode* minNode = findMin(node->right, cd, level + 1);
            node->data = minNode->data; // Thay thế dữ liệu
            node->right = recRemove(node->right, minNode->data, level + 1); // Xóa node đã thay thế
        } else {
            // Chuyển nhánh bên trái lên thay thế và xóa nhánh trái
            kDTreeNode* temp = node->left;
            delete node;
            return temp; // Trả về nhánh trái để nó trở thành nhánh mới của cha nút này
        }
    } else if (point[cd] < node->data[cd]) {
        node->left = recRemove(node->left, point, level + 1); // Tìm kiếm và xóa trong cây con bên trái
    } else {
        node->right = recRemove(node->right, point, level + 1); // Tìm kiếm và xóa trong cây con bên phải
    }

    return node;
}

void kDTree::remove(const vector<int> &point)
{
    root=recRemove(root,point,0);
    

}

bool kDTree::recSearch(kDTreeNode *node, const vector<int> &point, int level) const
{
    if(node == nullptr) return false;
    int axis = level %k;
    if(point==node->data) return true;
    if(point[axis] < node->data[axis]) return recSearch(node->left,point,level+1);
    else return recSearch(node->right,point,level+1);   
}

bool kDTree::search(const vector<int> &point)
{
    return recSearch(root,point,0);
}

vector<vector<int>> convertListToVector(const list<list<int>> &lst)
{
    vector<vector<int>> vec;
    vec.reserve(lst.size()); // Tối ưu hóa bằng cách đặt trước kích thước

    // Duyệt qua mỗi list<int> trong list<list<int>>
    for (const auto& sublist : lst) {
        vector<int> subVec(sublist.begin(), sublist.end());  // Chuyển đổi list<int> thành vector<int>
        vec.push_back(subVec);  // Thêm subVec vào vec chính
    }

    return vec;  // Trả về vector đã chuyển đổi
}

list<list<int>> convertVectorToList(const vector<vector<int>> &vec)
 {
    list<list<int>> lst;

    // Duyệt qua mỗi vector<int> trong vector<vector<int>>
    for (const auto& subVec : vec) {
        list<int> sublist(subVec.begin(), subVec.end());  // Chuyển đổi vector<int> thành list<int>
        lst.push_back(sublist);  // Thêm sublist vào list chính
    }

    return lst;  // Trả về list đã chuyển đổi
}

void insertionSort(vector<vector<int>> &points, int axis)
{
    for (size_t i = 1; i < points.size(); ++i) {
        vector<int> key = points[i];
        size_t j = i;
        while (j > 0 && points[j - 1][axis] > key[axis]) {
            points[j] = points[j - 1];
            j--;
        }
        points[j] = key;
    }
}
kDTreeNode *kDTree::recBuildTree(const vector<vector<int>> &pointList, int level)
{   // Bước 0: Kiểm tra điều kiện dừng
    if (pointList.empty()) {
        return nullptr;
    }

    // Lấy kích thước chiều dựa trên một điểm bất kỳ (giả định tất cả các điểm đều có cùng số chiều)
    int k = pointList[0].size();
    int axis = level % k; // Bước 1: Tính chiều alpha để sắp xếp

    // Bước 2: Sắp xếp lại điểm dựa trên chiều alpha sử dụng insertion sort
    vector<vector<int>> sortedPoints = pointList;
    insertionSort(sortedPoints, axis);

    // Bước 3: Tìm chỉ số trung vị để chia danh sách điểm
    size_t midIndex = sortedPoints.size() / 2;

    // Bước 4 và 5: Tạo node mới từ điểm trung vị
    kDTreeNode* node = new kDTreeNode(sortedPoints[midIndex]);
    this->count++; // Tăng số lượng node trong cây

    // Bước 6 và 7: Gọi đệ quy để xây dựng cây con từ các điểm ở bên trái và bên phải điểm trung vị
    vector<vector<int>> leftPoints(sortedPoints.begin(), sortedPoints.begin() + midIndex);
    vector<vector<int>> rightPoints(sortedPoints.begin() + midIndex + 1, sortedPoints.end());
    node->left = recBuildTree(leftPoints, level + 1);
    node->right = recBuildTree(rightPoints, level + 1);

    return node;
     
}

void kDTree::buildTree(const vector<vector<int>> &pointList)
{
    this->root=this->recBuildTree(pointList,0);
}

int kDTree::distanceSquared(const vector<int> &a, const vector<int> &b)
{
    int distance = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        distance += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return distance;
}

void kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode *best,kDTreeNode *node, int level)
{
     if (node == nullptr) return;  // Kiểm tra điều kiện dừng

    // Cập nhật điểm láng giềng gần nhất nếu thấy điểm tốt hơn
    if (best == nullptr || distanceSquared(target, node->data) < distanceSquared(target, best->data)) {
        best = node;
    }

    // Xác định chiều hiện tại dựa trên độ sâu
    int axis = level % k;
    kDTreeNode *next = target[axis] < node->data[axis] ? node->left : node->right;
    kDTreeNode *other = target[axis] < node->data[axis] ? node->right : node->left;

    // Đệ quy tìm kiếm trong nhánh tiềm năng chứa điểm gần nhất trước
    nearestNeighbour(target, best, next, level + 1);

    // Kiểm tra nhánh kia nếu có khả năng tìm thấy điểm gần hơn
    if ((target[axis] - node->data[axis]) * (target[axis] - node->data[axis]) < distanceSquared(target, best->data)) {
        nearestNeighbour(target, best, other, level + 1);
    }
}

void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList)
{
    
    kNearestNeighbourRec(root, target, k, bestList, 0);
}

void kDTree::kNearestNeighbourRec(kDTreeNode *node, const vector<int> &target, int k, vector<kDTreeNode *> &bestList, int level)
{
    if (node == nullptr) return;

    if (bestList.size() < k) {
        insertSorted(bestList, node, target);  // Chèn và duy trì danh sách đã sắp xếp
    } else {
        int lastDist = distanceSquared(target, bestList.back()->data);
        int nodeDist = distanceSquared(target, node->data);
        if (nodeDist < lastDist) {
            insertSorted(bestList, node, target);
        }
    }

    int axis = level % k;
    kDTreeNode *next = target[axis] < node->data[axis] ? node->left : node->right;
    kDTreeNode *other = target[axis] < node->data[axis] ? node->right : node->left;

    kNearestNeighbourRec(next, target, k, bestList, level + 1);

    int distToPlane = (target[axis] - node->data[axis]) * (target[axis] - node->data[axis]);
    if (distToPlane < distanceSquared(target, bestList.back()->data)) {
        kNearestNeighbourRec(other, target, k, bestList, level + 1);
    }
}

void kDTree::postorderTraversalRec(kDTreeNode *node) const
{
        if(node==nullptr) return;

        postorderTraversalRec(node->left);
        postorderTraversalRec(node->right);
              
        node->print();
       
       
    
}
void kDTree::insertSorted(vector<kDTreeNode*>& list, kDTreeNode* node, const vector<int>& target) {
    int nodeDist = distanceSquared(target, node->data);
    auto it = list.begin();
    while (it != list.end() && distanceSquared(target, (*it)->data) < nodeDist) {
        ++it;
    }
    list.insert(it, node);
    if (list.size() > k) {
        list.pop_back();  // Giữ lại chỉ `k` phần tử gần nhất
    }
}

kDTree::kDTree(int k)
{
    this->k=k;
    this->root=nullptr;
    this->count=0;
}

kDTree::~kDTree()
{
}

void kNN::fit(Dataset &X_train, Dataset &y_train)
{    
    // Giả định X_train.data và y_train.data là list<list<int>>
    list<list<int>> combinedData = X_train.data; // Giả sử getData() là phương thức để lấy dữ liệu
    list<list<int>> yData = y_train.data;

    // Kết hợp X_train và y_train
    auto yIter = yData.begin();
    for (auto &row : combinedData) {
        if (yIter != yData.end()) {
            row.push_back(yIter->front()); // Giả sử rằng mỗi list<int> trong yData chỉ chứa một phần tử
            ++yIter;
        }
        else std::__throw_out_of_range("y_train has fewer rows than X_train");
    }

    // Chuyển đổi dữ liệu sang vector<vector<int>> để xử lý
    vector<vector<int>> vectorData = convertListToVector(combinedData);

    // Xây dựng cây k-D
    train.buildTree(vectorData);

}

Dataset kNN::predict(Dataset &X_test)
{
    list<list<int>> predictions;
    int numLabels = 10;  // Giả sử nhãn từ 0 đến 9
    for (auto &point : X_test.data) {
        vector<kDTreeNode*> nearest;
        vector<int> pointVector(point.begin(), point.end());
        train.kNearestNeighbour(pointVector, k, nearest);

        vector<int> counts(numLabels, 0);  // Đếm số lần xuất hiện của mỗi nhãn
        for (auto &neighbour : nearest) {
            int label = neighbour->data.back();
            counts[label]++;
        }

        int bestLabel = 0;
        int bestCount = 0;
        for (int label = 0; label < numLabels; label++) {
            if (counts[label] > bestCount) {
                bestCount = counts[label];
                bestLabel = label;
            }
        }

        predictions.push_back({bestLabel});
    }

    Dataset y_pred;
    y_pred.data = predictions;
    return y_pred;
}

double kNN::score(const Dataset &y_test, const Dataset &y_pred)
{
     auto it_pred = y_pred.data.begin();
    auto it_test = y_test.data.begin();
    int correctCount = 0;
    int totalCount = 0;

    while (it_pred != y_pred.data.end() && it_test != y_test.data.end()) {
        if (!it_test->empty() && !it_pred->empty() && (*it_pred).front() == (*it_test).front()) {
            correctCount++;
        }
        ++it_pred;
        ++it_test;
        totalCount++;
    }

    if (totalCount == 0) return 0.0;  // Tránh chia cho zero
    return static_cast<double>(correctCount) / totalCount;
}
