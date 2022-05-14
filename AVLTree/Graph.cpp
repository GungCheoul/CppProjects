#include "Graph.h"
#include <map>

Graph::Graph(ofstream * flog)
{
	this->flog = flog; // flog = "log.txt"
	mList = new map<int, int>[50]; // allocate mList as array
	vertex = new CityData*[40]; // allocate vertex as array
	for (int i = 0; i < 40; i++) // allocate whole vertexes
		vertex[i] = new CityData();
	size = 0; // intialize
	mstMatrix = new int[40]; // allocate mstMatrix
	mstbuilt = false; // checking MST is built or not
}

Graph::~Graph() // release data
{
	delete mList;
	vertex = NULL;
	size = 0;
	delete mstMatrix;
}

void Graph::Insert(AVLNode* node) // insert AVLTree data into vertex
{
	if (node != NULL) { // until node is NULL, save by inorder(ascending order of cityname)
		Insert(node->GetLeft()); // go to left
		vertex[size]->SetLocationId(node->GetCityData()->GetLocationId()); // set LocationId
		vertex[size]->Setname(node->GetCityData()->Getname()); // set cityname
		size++; // increase the size of vertex. at the end, size means AVLTree's node number
		Insert(node->GetRight()); // go to right
	}
}

bool Graph::Build(AVLTree * root) // build Graph
{
	if (!root) // no data in AVLTree
		return false;
	else {
		Insert(root->Getroot()); // inserting node data
		for (int i = 0; i < size; i++) { // creating complete graph, size x size
			for (int j = 0; j < size; j++)
				mList[i][j] = abs(vertex[i]->GetLocationId() - vertex[j]->GetLocationId()); // i = from node, j = to node, save absolute value of weight
		}
		for (int i = 0; i < size; i++) { // creating weighted graph
			for (int j = 0; j < size; j++) {
				gpWeighted.insert({ i, { mList[i][j], j } }); // sorted by ascending order of from node, for Graph
				mstWeighted.insert({ mList[i][j], { i, j } }); // sorted by ascending order of weight, for MST
			}
		}
		return true;
	}
}

void Graph::Print_GP()
{
	multimap<int, pair<int, int>>::iterator weight; // iterator of gpWeighted
	weight = gpWeighted.begin(); // start by begin
	cout.setf(ios::left);
	for (int i = 0; i < size; i++) { // size x size matrix format
		for (int j = 0; j < size; j++) {
			*flog << setw(7) << weight->second.first; // print absolute value of weight, saved in gpWeighted
			weight++; // next data
		}
		*flog << endl;
	}
}

void Graph::Print_MST() // printing MST
{
	map<pair<int, int>, int>::iterator printMST; // iterator of MST
	map<pair<int, int>, int> search; // to find the connected vertex
	map<pair<int, int>, int>::iterator mst; // iterator of search
	int First = MST.begin()->first.first; // first connection in MST, from vertex
	int Next = MST.begin()->first.second; // first connection in MST, to vertex
	int Value = MST.begin()->second; // first weight
	int Total = 0; // total weight
	search = MST; // intialize as MST
	search.erase({ First, Next }); // erase first connection data to avoid overlap
	for (printMST = MST.begin(); printMST != MST.end(); printMST++) { // until MST is end
		*flog << "(" << vertex[First]->Getname() << ", " << vertex[Next]->Getname() << ")" << ", " << Value << endl; // print to "log.txt"
		First = Next;
		Total += Value; // calculate all weights
		if (!search.empty()) { // until search is empty
			mst = search.begin(); // start with begin
			while (mst->first.first != Next && mst->first.second != Next) // find the node which is connected with Next
				mst++;
			if (mst->first.first == First) Next = mst->first.second; // set Next
			else Next = mst->first.first;
			Value = mst->second; // Next Value
			search.erase({ mst->first.first, mst->first.second }); // erase to avoid overlap
		}
	}
	*flog << "Toal: " << Total << endl; // print total weight
}

bool Graph::Kruskal() // Kruskal algorithm
{
	int x, y; // node number
	make_set(); // initialize by itself
	map<int, pair<int, int>>::iterator weight; // iterator for mstWeighted
	weight = mstWeighted.begin(); // start with begin
	for (int i = 0; i < size; i++) { // size by size matrix
		for (int j = 0; j < size; j++) {
			x = weight->second.first; // src node number
			y = weight->second.second; // des node number
			if (find(x) != find(y)) { // check the MST is making a cycle
				union_set(x, y); // connect MST nodes
				MST.insert({ { x, y }, weight->first }); // insert node number and weight to MST
			}
			weight++; // next node
		}
	}
	mstbuilt = true; // MST is built
	return true;
}

void Graph::make_set() // initialize by itself
{
	for (int i = 0; i < size; i++)
		mstMatrix[i] = i;
}

void Graph::union_set(int x, int y) // connect MST node
{
	int i = find(x); // src node number
	int j = find(y); // des node number
	// connect node
	if (i > j) mstMatrix[i] = j;
	else if (i < j) mstMatrix[j] = i;
}

int Graph::find(int x) // check the cycle of MST
{
	if (x == mstMatrix[x]) // if node number is same with array number
		return x;
	return mstMatrix[x] = find(mstMatrix[x]); // if not
}
