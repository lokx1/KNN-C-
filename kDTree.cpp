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
        
        
          
        cout << *node << " ";
        
       
        inorderTraversalRec(node->right);
    
    
}   

void kDTree::preorderTraversal() const
{
    this->preorderTraversalRec(root);
}

void kDTree::preorderTraversalRec(kDTreeNode *node) const
{
    if(node==nullptr) return;
       
       
           
        cout << *node << " ";
        
        
        preorderTraversalRec(node->left);
        preorderTraversalRec(node->right);
        

}

void kDTree::postorderTraversal() const
{
   this-> postorderTraversalRec(root);
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
            this->count--; // Giảm số lượng node trong cây
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
             kDTreeNode* minNode = findMin(node->left, cd, level + 1);
            node->data = minNode->data; // Thay thế dữ liệu node hiện tại bằng node nhỏ nhất bên trái
            node->left = recRemove(node->left, minNode->data, level + 1); // Xóa node nhỏ nhất bên trái
            node->right = node->left; // Di chuyển cây con bên trái lên thay cho node bên phải
            node->left = nullptr; // Xóa liên kết bên trái cũ
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
     bool swapped;
    size_t n = points.size();
    do {
        swapped = false;
        for (size_t i = 1; i < n; i++) {
            if (points[i - 1][axis] > points[i][axis]) {
                swap(points[i - 1], points[i]);
                swapped = true;
            }
        }
        n--; // Each pass places the next-largest element in its final position.
    } while (swapped);
    
}
kDTreeNode *kDTree::recBuildTree(const vector<vector<int>> &pointList, int level)
{   // Bước 0: Kiểm tra điều kiện dừng
    if (pointList.empty()) {
        return nullptr;
    }

    
    int axis = level % k;
    vector<vector<int>> sortedPoints = pointList;
    insertionSort(sortedPoints, axis);

    size_t midIndex = (pointList.size()-1)/2;
  while (midIndex > 0 && sortedPoints[midIndex][axis] == sortedPoints[midIndex -1][axis]) {
        midIndex--;  // Điều chỉnh về bên trái nếu giá trị trùng nhau
    }


    kDTreeNode* node = new kDTreeNode(sortedPoints[midIndex]);
    this->count++;

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

double kDTree::distanceSquared(const vector<int> &a, const vector<int> &b)
{
    double distance = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        distance += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return distance;
}

void kDTree::nearestNeighbourRec(kDTreeNode *temp, const vector<int> &target, kDTreeNode *&best, int level)
{

if (temp == nullptr) return;  // Điều kiện dừng khi node duyệt là null

    int alpha = level % this->k;  // Tính giá trị alpha
    double d = abs(target[alpha] - temp->data[alpha]);  // Khoảng cách hình chiếu của target lên trục alpha

    // Xác định node tiếp theo để duyệt dựa trên giá trị của alpha
    kDTreeNode* next = (target[alpha] < temp->data[alpha]) ? temp->left : temp->right;
    // Duyệt nhánh chính trước
    nearestNeighbourRec(next, target, best, level + 1);


    // Sau khi đã duyệt một bên, kiểm tra và cập nhật best
    if (best == nullptr || distanceSquared(target, temp->data) < distanceSquared(target, best->data)) {
        best = temp;  // Cập nhật best nếu temp gần hơn
    }

    // Khoảng cách từ target đến best hiện tại
    double R = distanceSquared(target, best->data);

    // Chỉ kiểm tra nhánh kia nếu d < R và nếu nhánh kia còn có thể cung cấp node gần hơn
    if (d < R) {
        kDTreeNode* other = (target[alpha] < temp->data[alpha]) ? temp->right : temp->left;
        nearestNeighbourRec(other, target, best, level + 1);
    }

}

void kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode *&best)
{
    best=nullptr;
    nearestNeighbourRec(root,target,best,0);
}

void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList)
{
    
    kNearestNeighbourRec(root, target, k, bestList, 0);
}

void kDTree::kNearestNeighbourRec(kDTreeNode *temp, const vector<int> &target, int k, vector<kDTreeNode *> &bestList, int level)
{    if (temp == nullptr) return;  // Nếu node hiện tại là null, dừng việc duyệt

    int alpha = level % this->k;  // Tính chỉ số chiều hiện tại để so sánh
    double d = abs(target[alpha] - temp->data[alpha]);  // Khoảng cách từ điểm hiện tại đến mặt phẳng phân chia

    // Chọn nhánh tiếp theo để duyệt dựa trên giá trị tại chiều alpha
    kDTreeNode *nextBranch = (target[alpha] < temp->data[alpha]) ? temp->left : temp->right;
    kDTreeNode *oppositeBranch = (target[alpha] < temp->data[alpha]) ? temp->right : temp->left;

    // Duyệt nhánh ưu tiên trước
    kNearestNeighbourRec(nextBranch, target, k, bestList, level + 1);

    // Tính khoảng cách Euclidean từ điểm mục tiêu đến node hiện tại
    double r = distanceSquared(target, temp->data);

    if (bestList.size() < k) {
        bestList.push_back(temp);  // Nếu danh sách chưa đầy, thêm node hiện tại vào
        kNearestNeighbourRec(oppositeBranch, target, k, bestList, level + 1);  // Duyệt nhánh đối diện nếu cần
    } else {
        // Tìm khoảng cách lớn nhất trong danh sách
        double maxDistance = distanceSquared(target, bestList[0]->data);
        int maxIndex = 0;
        for (int i = 0; i < k; i++) {
            double currentDistance = distanceSquared(target, bestList[i]->data);
            if (currentDistance >= maxDistance) {
                maxDistance = currentDistance;
                maxIndex = i;
            }
        }
        // Nếu node hiện tại gần hơn khoảng cách lớn nhất trong danh sách, thay thế nó
        if (r < maxDistance) {
            bestList.erase(bestList.begin()+maxIndex);
            bestList.push_back(temp);
               
                // Cập nhật danh sách với node gần hơn
        }

        // Chỉ kiểm tra nhánh đối diện nếu khoảng cách hình chiếu d nhỏ hơn khoảng cách lớn nhất hiện tại trong bestList
        if (!bestList.empty() && d < maxDistance) {
            kNearestNeighbourRec(oppositeBranch, target, k, bestList, level + 1);
        }
   
        



    }
     for (int i = 1; i < bestList.size(); i++) {
        kDTreeNode* key = bestList[i];
        double keyDistance = distanceSquared(target, key->data);
        int j = i - 1;
        while (j >= 0 && distanceSquared(target, bestList[j]->data) > keyDistance) {
            bestList[j + 1] = bestList[j];
            j--;
        }
        bestList[j + 1] = key;
    }
    
}

kDTreeNode * kDTree::buildTreeLableRec(const vector<vector<int>> &pointList, const vector<int> &label, int level)
{   if (pointList.empty()) {
        return nullptr;
    }

    int axis = level % k;
    vector<pair<vector<int>, int>> pairedPointsLabels;
    for (size_t i = 0; i < pointList.size(); ++i) {
        pairedPointsLabels.push_back({pointList[i], label[i]});
    }

    // Insertion sort to sort by the 'axis' value while maintaining labels
    for (size_t i = 1; i < pairedPointsLabels.size(); ++i) {
        auto key = pairedPointsLabels[i];
        int j = i - 1;
        while (j >= 0 && pairedPointsLabels[j].first[axis] > key.first[axis]) {
            pairedPointsLabels[j + 1] = pairedPointsLabels[j];
            j = j - 1;
        }
        pairedPointsLabels[j + 1] = key;
    }

    size_t midIndex = pairedPointsLabels.size() / 2;
    while (midIndex > 0 && pairedPointsLabels[midIndex].first[axis] == pairedPointsLabels[midIndex - 1].first[axis]) {
        midIndex--; // Adjust to the left if values are the same
    }

    vector<vector<int>> leftPoints;
    vector<int> leftLabels;
    vector<vector<int>> rightPoints;
    vector<int> rightLabels;

    for (size_t i = 0; i < midIndex; ++i) {
        leftPoints.push_back(pairedPointsLabels[i].first);
        leftLabels.push_back(pairedPointsLabels[i].second);
    }

    for (size_t i = midIndex + 1; i < pairedPointsLabels.size(); ++i) {
        rightPoints.push_back(pairedPointsLabels[i].first);
        rightLabels.push_back(pairedPointsLabels[i].second);
    }

    kDTreeNode* node = new kDTreeNode(pairedPointsLabels[midIndex].first, pairedPointsLabels[midIndex].second);
    node->left = buildTreeLableRec(leftPoints, leftLabels, level + 1);
    node->right =buildTreeLableRec(rightPoints, rightLabels, level + 1);

    return node;


}

void kDTree::buildTreeLable(const vector<vector<int>> &pointList, const vector<int> &label)
{   this->count=pointList.size();
    this->root=this->buildTreeLableRec(pointList,label,0);
 
}

void kDTree::postorderTraversalRec(kDTreeNode *node) const
{
        if(node==nullptr) return;

        postorderTraversalRec(node->left);
        postorderTraversalRec(node->right);
              
           
        cout << *node << " ";
       
       
    
}


kDTree::kDTree(int k)
{
    this->k=k;
    this->root=nullptr;
    this->count=0;
}

kDTree::~kDTree()
{
   clear(root);
}

void kNN::fit(Dataset &X_train, Dataset &y_train)
{  

    this->X_train = &X_train;
    this->y_train = &y_train;

    if (X_train.data.size()) {
        int chieu = X_train.data.front().size();
        train.k = chieu;
        vector<vector<int>> pointList ;
        pointList.reserve(X_train.data.size());
        for (const auto& sublist : X_train.data) {
        vector<int> subVec(sublist.begin(), sublist.end());  // Chuyển đổi list<int> thành vector<int>
        pointList.push_back(subVec);  // Thêm subVec vào vec chính
    }
        vector<int> label;
        for (auto it = y_train.data.begin(); it != y_train.data.end(); ++it) {
            if (!it->empty()) {  // Kiểm tra xem vector không rỗng
                label.push_back(it->front());  // Giả định rằng mỗi vector chỉ chứa một nhãn
            }
        }
        
        train.buildTreeLable(pointList, label);

    }
   
}
   
int mostFrequentLabel(const std::vector<kDTreeNode*>& nodeList) {
   std::vector<int> labelCount(10, 0);  // Thay đổi giá trị 10 nếu nhãn của bạn có giới hạn khác
 for (const auto& node : nodeList) {
        if (node != nullptr) {
            labelCount[node->label]++;  // Tăng số lượng nhãn tương ứng
        }
    }
  

    int mostFrequent = 0;
    int maxCount = 0;
    for (int i = 0; i < labelCount.size(); ++i) {
        if (labelCount[i] > maxCount) {
            maxCount = labelCount[i];
            mostFrequent = i;
        }
    }

    return mostFrequent;
}


Dataset kNN::predict(Dataset &X_test)
{
    Dataset result;
    result.columnName.push_back("label");  // Xác nhận cột cho nhãn
  
    vector<vector<int>> testData= convertListToVector(X_test.data);
    vector<int> predictions;


    for(const auto& target:testData){
            vector<kDTreeNode*> bestList;
           
            train.kNearestNeighbour(target, this->k, bestList);
           
            int predictedLabel = mostFrequentLabel(bestList);
      
            predictions.push_back(predictedLabel);
           
        
    }
           
    for(const auto& label : predictions) {
        std::list<int> row; // Tạo một list mới cho mỗi nhãn
        
        row.push_back(label);
        result.data.push_back(row);  // Thêm vào data của result
        

    }   
    return result;
}

double kNN::score(const Dataset &y_test, const Dataset &y_pred)
{
       if (y_test.data.empty() || y_pred.data.empty()) {
            return 0.0; // Trả về 0 nếu một trong các dataset rỗng
        }

        auto it_test = y_test.data.begin();
        auto it_pred = y_pred.data.begin();
        int correctCount = 0;
        int total = 0;

        while (it_test != y_test.data.end() && it_pred != y_pred.data.end()) {
            // Giả định rằng mỗi danh sách chỉ chứa một nhãn
            if (!it_test->empty() && !it_pred->empty() && it_test->front() == it_pred->front()) {
                correctCount++;  // Tăng số lượng dự đoán chính xác
            }
            ++it_test;
            ++it_pred;
            ++total;
        }

        if (total == 0) return 0.0; // Kiểm tra chia cho 0
        return static_cast<double>(correctCount) / total;
}

void kDTree::clear(kDTreeNode *node)
{
     if (node != nullptr) {
        clear(node->left);  // Giải phóng nút con bên trái
        clear(node->right); // Giải phóng nút con bên phải
        delete node;        // Giải phóng nút hiện tại
        node = nullptr;     // Đặt lại con trỏ thành nullptr sau khi đã xóa
    }
}
