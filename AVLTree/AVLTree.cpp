#include "AVLTree.h"

int number = 0; // global variable for the searching

AVLTree::AVLTree(ofstream * flog)
{
	this->flog = flog; // flog = "log.txt"
	root = NULL; // intialize root by NULL
}

AVLTree::~AVLTree()
{
	Release(root); // release the AVLTree
	flog->close(); // close "log.txt"
}

void AVLTree::Release(AVLNode* node) // releasing all data in AVLTree
{
	if (node == NULL) // no data, end function
		return;
	if (node->GetLeft() != NULL) // go to Left node
		Release(node->GetLeft());
	if (node->GetRight() != NULL) // go to Right node
		Release(node->GetRight());
	node->SetLeft(NULL); // release Left node
	node->SetRight(NULL); // release Right node
	if (node != NULL) // release Head node
		delete node;
}

AVLNode * AVLTree::Getroot() // get AVLTree root
{
	return root;
}

void AVLTree::Setroot(AVLNode * node) // set AVLTree root
{
	root = node;
}

int AVLTree::Height(AVLNode * node) // calculate node's height
{
	if (node != NULL) { // recursion function
		int left_h = Height(node->GetLeft()); // left node's height
		int right_h = Height(node->GetRight()); // right node's height
		if (left_h > right_h) // calculate height, get bigger one
			return left_h + 1;
		else
			return right_h + 1;
	}
	return 0; // node is NULL
}

int AVLTree::Bfactor(AVLNode * node) // get Balance Factor
{
	int left_bf = Height(node->GetLeft()); // left node's BF
	int right_bf = Height(node->GetRight()); // right node's BF
	int bf = left_bf - right_bf; // Balance Factor is (left node's BF) - (right node's BF)
	return bf;
}

AVLNode * AVLTree::LL(AVLNode* node) // Left Left rotation
{
	AVLNode* temp = new AVLNode();
	temp = node->GetLeft(); // set temp as node's left node
	node->SetLeft(temp->GetRight()); // set node's left as temp's right
	temp->SetRight(node); // set temp's right as node
	return temp; // switch current node to temp
}

AVLNode * AVLTree::RR(AVLNode* node) // Right Right rotation
{
	AVLNode* temp = new AVLNode();
	temp = node->GetRight(); // set temp as node's right node
	node->SetRight(temp->GetLeft()); // set node's right as temp's left
	temp->SetLeft(node); // set temp's left as node
	return temp; // switch current node to temp
}

AVLNode * AVLTree::LR(AVLNode* node) // Left Right rotation
{
	AVLNode* temp = new AVLNode();
	temp = node->GetLeft(); // set temp as node's left node
	node->SetLeft(RR(temp)); // set node's left as RR rotated temp
	return LL(node); // LL rotate the node
}

AVLNode * AVLTree::RL(AVLNode* node) //ok
{
	AVLNode* temp = new AVLNode();
	temp = node->GetRight(); // set temp as node's right node
	node->SetRight(LL(temp)); // set node's right as LL rotated temp
	return RR(node); // RR rotate the node
}

AVLNode * AVLTree::Balance(AVLNode* node) // Balance AVLTree by comparing BF
{
	int factor = Bfactor(node); // node's BF
	if (factor > 1) { // if BF is bigger than 1, LL or LR
		if (Bfactor(node->GetLeft()) > 0) // node's left BF is bigger than 0, LL
			node = LL(node);
		else // else, LR
			node = LR(node);
	}
	else if (factor < -1) { // if BF is smaller than -1, RR or RL
		if (Bfactor(node->GetRight()) > 0) // node's right BF is bigger than 0, RL
			node = RL(node);
		else // else, RR
			node = RR(node);
	}
	return node; // Balanced node
}

void AVLTree::Balancing(AVLNode* node) // total Balancing AVLTree by recursion
{
	if (node->GetLeft() != NULL && node->GetRight() != NULL) { // until left or right node is NULL
		node = Balance(node);
		node->SetLeft(Balance(node->GetLeft())); // Balance left node
		node->SetRight(Balance(node->GetRight())); // Balance Right node
		Balancing(node->GetLeft()); // recursion
		Balancing(node->GetRight());
	}
}

void AVLTree::UpdateBF(AVLNode* node) // update BF balanced AVLTree
{
	if (node != NULL) { // recursion
		node->SetmBF(Bfactor(node)); // set node's BF
		UpdateBF(node->GetLeft()); // node's left BF
		UpdateBF(node->GetRight()); // node's right BF
	}
}

AVLNode * AVLTree::Insert(AVLNode* newnode, CityData* node) // insert new node to AVLTree
{
	if (newnode == NULL) { // insert the new data into AVLTree
		AVLNode* Inode = new AVLNode();
		Inode->SetCityData(node); // new inserting data
		return Inode; // insert the node
	}
	else if (strcmp(node->Getname(), newnode->GetCityData()->Getname()) < 0) { // insert to left node
		newnode->SetLeft(Insert(newnode->GetLeft(), node));
		newnode = Balance(newnode); // Balance after insert
	}
	else if (strcmp(node->Getname(), newnode->GetCityData()->Getname()) > 0) { // insert to right node
		newnode->SetRight(Insert(newnode->GetRight(), node));
		newnode = Balance(newnode); // Balance after insert
	}
	newnode->SetmBF(Bfactor(newnode)); // set BF to new node
	return newnode;
}

void AVLTree::Deleting(AVLNode* node, int num) // total deleting function
{
	AVLNode* bfnode = root;
	if (node != NULL) { // finding the inputed LocationId
		if (node->GetCityData()->GetLocationId() == num) { // finded
			Delete(node, num); // go to delete function and delete corresponding data
			Balancing(root); // Balance AVLTree
			UpdateBF(root); // Update the BF
		}
		else { // recursion
			Deleting(node->GetLeft(), num); // go to left node
			Deleting(node->GetRight(), num); // go to right node
		}
	}
}

void AVLTree::Delete(AVLNode* node, int num) // delete function
{
	AVLNode* find = new AVLNode(); // finding corresponding data
	AVLNode* parent = new AVLNode(); // corresponding data node's parent
	// use for delete patterns
	AVLNode* d_node = new AVLNode();
	AVLNode* c_node = new AVLNode();
	AVLNode* temp = new AVLNode();
	AVLNode* re_node = new AVLNode();
	AVLNode* re2_node = new AVLNode();

	find = root; // set root
	while (find->GetCityData()->GetLocationId() != num) { // until find same LocationId, comparing by city name
		if (strcmp(node->GetCityData()->Getname(), find->GetCityData()->Getname()) < 0) {
			parent = find;
			find = find->GetLeft();
		}
		else if (strcmp(node->GetCityData()->Getname(), find->GetCityData()->Getname()) > 0) {
			parent = find;
			find = find->GetRight();
		}
	}

	// when deleting node is root
	d_node = root;
	if (root->GetCityData()->GetLocationId() == num) { // if removing data is root node
		if (d_node->GetLeft() == NULL && d_node->GetRight() == NULL) { // root node is terminal node
			delete d_node; // release root node
			root = NULL; // reset to NULL
		}
		else if (d_node->GetLeft() == NULL || d_node->GetRight() == NULL) { // root node has one child node
			if (d_node->GetLeft() == NULL) // has right node
				temp = d_node->GetRight(); // take root node's right node
			if (d_node->GetRight() == NULL) // has left node
				temp = d_node->GetLeft(); // take root node's left node
			delete d_node; // release root node
			Setroot(temp); // new root node is root node's left or right node
		}
		else if (d_node->GetLeft() != NULL && d_node->GetRight() != NULL) { // root node has two child node
			temp = d_node->GetRight(); // move to right node
			if (temp->GetLeft() == NULL) { // no left child node
				re_node = d_node->GetLeft(); // root node's left child node
				re2_node = d_node->GetRight(); // right child node
				Setroot(temp); // new root node
				temp->SetLeft(re_node);
				delete d_node;
			}
			else { // no right child node
				while (temp->GetLeft() != NULL) { // goto leftmost node
					c_node = temp;
					temp = temp->GetLeft();
				}
				c_node->SetLeft(NULL); // remove node to import to root node
				root->SetCityData(temp->GetCityData()); // new root node
				delete temp;
			}
		}
	}

	else if (node->GetLeft() == NULL && node->GetRight() == NULL) { // removing node is terminal node
		if (find == parent->GetLeft()) // removing node is parent node's left node
			parent->SetLeft(NULL); // reset to NULL
		else if (find == parent->GetRight()) // removing node is parent node's right node
			parent->SetRight(NULL); // reset to NULL
		delete find;
	}

	else if (node->GetLeft() == NULL) { // has right child node
		c_node = node->GetRight(); // goto removing node's right node
		if (find == parent->GetLeft()) // removing node is parent node's left node
			parent->SetLeft(c_node); // parent node's new left node is removing node's left node
		else if (find == parent->GetRight()) // removing node is parent node's right node
			parent->SetRight(c_node); // parent node's new right node is removing node's right node
		delete find;
	}

	else if (node->GetRight() == NULL) { // has left child node
		c_node = node->GetLeft(); // goto removing node's left node
		if (find == parent->GetLeft()) // removing node is parent node's left node
			parent->SetLeft(c_node); // parent node's new left node is removing node's left node
		else if (find == parent->GetRight()) // removing node is parent node's right node
			parent->SetRight(c_node); // parent node's new right node is removing node's right node
		delete find;
	}

	else if (node->GetLeft() != NULL && node->GetRight() != NULL) { // has two child node
		temp = node->GetRight(); // move to right child node
		if (temp->GetLeft() == NULL) { // right child node has only right child node
			re_node = node->GetLeft(); // removing node's left node
			re2_node = temp; // removing node's right node
			// remove corresponding node and fill with new left node and right node
			if (find == parent->GetLeft()) { // removing node is left
				parent->SetLeft(re2_node);
				re2_node->SetLeft(re_node);
			}
			else if (find == parent->GetRight()) { // removing node is right
				parent->SetRight(re2_node);
				re2_node->SetLeft(re_node);
			}
			delete find;
		}
		else { // right child node has only left child node
			while (temp->GetLeft() != NULL) { // goto node which has next higher data
				parent = temp;
				temp = temp->GetLeft(); // parent node's left node is removing node, next higher node
			}
			parent->SetLeft(NULL); // removing importing node
			node->SetCityData(temp->GetCityData()); // new data
			delete temp;
		}
	}
}

void AVLTree::Search(AVLNode* search, int num) // search corresponding data
{
	if (search != NULL) { // until node is NULL
		if (search->GetCityData()->GetLocationId() == num) // finded the data, print to "log.txt"
			*flog << "( " << search->GetCityData()->GetLocationId() << ", " << search->GetCityData()->Getname() << ", " << search->GetCityData()->Getcountry() << " )" << endl;
		Search(search->GetLeft(), num); // search left node
		Search(search->GetRight(), num); // search right node
	}
}

void AVLTree::Searching(AVLNode* search, int num) // searching function, whether data is exists
{
	if (search != NULL) { // until node is NULL
		if (search->GetCityData()->GetLocationId() == num) // if data exists, save global variable number as 1
			number = 1;
		Searching(search->GetLeft(), num); // go to left node
		Searching(search->GetRight(), num); // go to right node
	}
}

void AVLTree::Inorder(AVLNode* node) // print by inorder
{
	if (node != NULL) { // unitl node is NULL, inorder and print AVLTree data to "log.txt"
		Inorder(node->GetLeft());
		*flog << "( " << node->GetCityData()->GetLocationId() << ", " << node->GetCityData()->Getname() << ", " << node->GetCityData()->Getcountry() << " )" << endl;
		Inorder(node->GetRight());
	}
}

bool AVLTree::Print() // print function
{
	if (root == NULL) // if root is NULL, it does not work
		return false;
	else { // not NULL, print from root by inorder
		Inorder(root);
		return true;
	}
}
