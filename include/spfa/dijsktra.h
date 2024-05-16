// // Implemented Dijstra Shortest Path Algorithm using Fibonacci Heap
// // to improve its complexity to O(V*logV + E)
// #include <iostream>
// #include <cmath>
// #include <vector>
// #include <limits.h>
// #include <algorithm>
// #include <unordered_map>
// #define MAX 100		      // Maximum number of vertices
// using namespace std;

// //Node Structure
// struct Node{
// 	bool mark;	      // Whether the node has lost a child since
// 		     	      // it was made child of another node.
// 	int index;	      // Index of the vertex
// 	int key;      	      // Value of the node
// 	int degree;           // Degree of the node
// 	Node *parent;         // Parent Pointer
// 	Node *left, *right;   // Siblings
// 	Node *child;	      // Child Pointer
// };

// Node* createnode(int key,int index){
// 	Node *newnode = new Node;
// 	newnode->index = index;
// 	newnode->key = key;
// 	newnode->parent = newnode->child = NULL;
// 	newnode->left = newnode->right = newnode;
// 	newnode->degree = 0;
// 	newnode->mark = false;
// 	return newnode;
// }

// class Dijsktra{
// 	Node *head;
// 	int nodes,vertices,edges,src;
// 	unordered_map<int,Node*>found;
// 	vector<pair<int,int> >adj[MAX];
// 	public:
// 		// Initializer
// 		Dijsktra(){
// 			cout << "Number of vertices: ";
// 			cin >> vertices;
// 			cout << "Number of edges: ";
// 			cin >> edges;
// 			cout << "Source vertex: ";
// 			cin >> src;
// 			int e = edges;
// 			cout << "Enter edges\n";
// 			while(e--){
// 				int s,d,w;
// 				cin >> s >> d >> w;
// 				// For directed graph
// 				adj[s].push_back(make_pair(d,w));
// 				// For undirected graph
// 				//adj[d].push_back(make_pair(s,w));
// 			}
// 			head = NULL;
// 			nodes = 0;
// 		}
// 		// Search for a node
// 		Node* fib_heap_search(int key){
// 			if(found.find(key)==found.end())
// 				return NULL;
// 			return found[key];
// 		}
// 		// Insertion
// 		void fib_heap_insert(int key,int index){
// 			Node *x = createnode(key,index);
// 			found[index] = x;
// 			if(head==NULL)
// 				head = x;
// 			else{
// 				// Insert new node to root list
// 				Node *l1 = head->left;
// 				Node *l2 = x->left;
// 				l1->right = x;
// 				x->left = l1;
// 				l2->right = head;
// 				head->left = l2;
// 				// Head Updation
// 				if(head->key > x->key)
// 					head = x;
// 			}
// 			nodes += 1;
// 		}
// 		//Create heap function
// 		void create_heap(){
// 			int index = 1;
// 			while(index<=vertices){
// 				int key;
// 				if(index==src)
// 					key = 0;
// 				else key = INT_MAX;
// 				fib_heap_insert(key,index);
// 				index++;
// 			}
// 		}
// 		// Extract Min
// 		Node* fib_heap_extract_min(){
// 			if(head==NULL){
// 				cout << "Error: Empty fibonacci heap\n";
// 				return NULL;
// 			}
// 			Node *z = head;
// 			found.erase(head->index);
// 			if(z!=NULL){
// 				// Adding its children to root list
// 				Node *ch = z->child;
// 				Node *ch_copy = ch;
// 				do{
// 					if(ch==NULL)
// 						break;
// 					Node *l1 = head->left;
// 					l1->right = ch;
// 					ch->left = l1;
// 					Node *chc = ch->right;
// 					ch->right = head;
// 					head->left = ch;
// 					ch->parent = NULL;
// 					ch = chc;
// 				}while(ch!=ch_copy);
// 				// Remove z from the root list
// 				Node *l = z->left, *r = z->right;
// 				l->right = r;
// 				r->left = l;
// 				if(z==z->right)
// 					head = NULL;
// 				else{
// 					head = z->right;
// 					consolidate();
// 				}
// 				nodes--;
// 			}
// 			if(z!=NULL){
// 				z->child = NULL;
// 				z->left = z->right = z;
// 			}
// 			return z;
// 		}
// 		// Linking
// 		void fib_heap_link(Node *y,Node *x){
// 			Node *l = y->left, *r = y->right;
// 			l->right = r;
// 			r->left = l;
// 			x->degree++;
// 			if(x->child==NULL){
// 				x->child = y;
// 				y->left = y->right = y;
// 			}
// 			else{
// 				Node *r = x->child->right;
// 				x->child->right = y;
// 				y->left = x->child;
// 				y->right = r;
// 				r->left = y;
// 			}
// 			y->parent = x;
// 			if(y->mark==true){
// 				y->mark = false;
// 			}
// 		}
// 		// Helper Function: Cut
// 		void cut(Node *x,Node *y){
// 			// Remove node from the child list
// 			Node *child = y->child;
// 			if(child->left==child and child->right==child)
// 				y->child = NULL;
// 			else{
// 				Node *l = x->left;
// 				Node *r = x->right;
// 				l->right = r;
// 				r->left  = l;
// 				if(y->child==x)
// 					y->child = l;
// 			}
// 			y->degree--;
// 			// Adding x to the root list
// 			Node *l = head->left;
// 			l->right = x;
// 			x->left  = l;
// 			x->right = head;
// 			head->left = x;
// 			x->parent = NULL;
// 			x->mark = false;
// 		}
// 		// Helper Function: Cascading-Cut
// 		void cascading_cut(Node *y){
// 			Node *z = y->parent;
// 			if(z!=NULL){
// 				if(y->mark==false)
// 					y->mark = true;
// 				else{
// 					cut(y,z);
// 					cascading_cut(z);
// 				}
// 			}
// 		}
// 		// Decrease Key
// 		void fib_heap_decrease_key(int x_key,int k){
// 			Node *x = fib_heap_search(x_key);
// 			Node *y = x->parent;
// 			x->key = k;
// 			if(x->parent!=NULL and x->key < y->key){
// 				cut(x,y);
// 				cascading_cut(y);
// 			}
// 			if(x->key<head->key)
// 				head = x;
// 		}
// 		// Perform Dijsktra
// 		void shortest_path_length(){
// 			while(nodes){
// 				Node *z = fib_heap_extract_min();
// 				cout << "Vertex: " << z->index << ", Shortest path length: " << z->key << "\n";
// 				int indx = z->index;
// 				for(int i=0;i<adj[indx].size();i++){
// 					int d,w;
// 					d = adj[indx][i].first;
// 					w = adj[indx][i].second;
// 					Node *x = fib_heap_search(d);
// 					if(x!=NULL){
// 						if(x->key > z->key + w)
// 							fib_heap_decrease_key(d,z->key + w);
// 					}
// 				}
// 			}
// 		}
// };

// int main(){
// 	Dijsktra obj;
// 	obj.create_heap();
// 	obj.shortest_path_length();
// 	return 0;
// }