#include <iostream>
#include <vector>
#include <regex>
#include <queue>


std::vector <int> getNumbers(std::string str);
void print(std::vector<int> const &input); // a simple function to print vertices for testing - delete it before push
void shortest_path(int max_vert, std::vector <std::vector <int>> edge_vector, int source, int dest);


/*we will make each node into an object - the Node will have attributes 
(node value, a vector of all its neighbors (also nodes), a boolean telling if its visited, and the parent node)*/

class Node {
public:	
    int node_value;
	std::vector<Node*> node_neighbors;
    bool visited = false;
    Node* parent= nullptr;
    
	Node(int node_val, int max_vert){ //constructor for our node - takes in the node value and max possible no. of vertices
        node_value = node_val;
        node_neighbors.reserve(max_vert); //reserve a memory space of size max_vert for the neighbors
    };
	
};



int main(){

    std::vector <std::vector <int>> edges_vector;
    while (!std::cin.eof()) {
    
        std::string line;  //an empty string variable to take line input

        //std::cout << "Enter edges or vertex or s command: ";                                                 //COMMENT THIS OUT
        std::getline(std::cin, line);
    
    std::vector <int> my_returned_vector = getNumbers(line);  //extracts all numbers from input command into a vector
    
    int max_vertices, v_src, v_dst;
    bool e_valid = false;
     //a vector of vectors with coordinates in it

    
    if (line[0] == 'V'){
        if (my_returned_vector[0] <1){
            std::cout << "Error: Vertices are undefined or less than 1" << std::endl;
        }
        else{
            max_vertices = my_returned_vector[0];
            edges_vector.clear();
        }
        std::cout << "V " << max_vertices << std::endl;                                             //DELETE COMMENT THIS
    }



    //if E command is given, check if all E values are <= V, if yes save to edges_vector
    else if (line[0] == 'E'){

        for(int i : my_returned_vector){ //iterate through all elements of my_returned_vector

            if (i > max_vertices || i <= 0){
                e_valid = false;
                max_vertices = 0; //reset the V command
                std::cout << "Error: Edges cannot have nodes as zero, -ve or greater than largest vertex. You must re-enter both V and E commands" << std::endl;
                break;
                }

            else if (i < max_vertices){
                e_valid = true;
                }    
            }

            if (e_valid == true){
            //edges_vector = my_returned_vector;

                int size = my_returned_vector.size();


                for (int i = 0; i < size; i = i+2){     //to make a vector of vectors
                    edges_vector.push_back({my_returned_vector[i],my_returned_vector[i+1]});
                }

            }   



        std::cout << "E {";                                                           //DELETE COMMENT THIS AND 2 LINE BELOW
        for (int itra = 0; itra < edges_vector.size()-1; itra++){
            std::cout << "<" << edges_vector[itra][0] << "," << edges_vector[itra][1] << ">,";
        }
        std::cout<<"<"<<edges_vector[edges_vector.size()-1][0]<<","<<edges_vector[edges_vector.size()-1][1]<<">}"<< std::endl;
           
        //std::cout << "i = " << i << std::endl;
        //std::cout << typeid(i).name() << '\n';
    }

 


    //check if s command is valid, then return shortest path, if path doesnt exist give error.
    else if (line[0] == 's'){

        if (my_returned_vector[0] <= max_vertices && my_returned_vector[1] <= max_vertices){


        v_src = my_returned_vector[0];
        v_dst = my_returned_vector[1];

        //std::cout << v_src <<"   "<<v_dst<<std::endl;

            if (v_src == v_dst){ //if source and dst are same do this
            std::cout << v_src <<std::endl;
                }
            else{
             shortest_path(max_vertices, edges_vector, v_src, v_dst);
                }
        }

        else {

            std::cout << "Error: Can't have a vertex greater than max vertex in S command. Please re-enter s command\n";
        }
        //std::cout << "src is: " << v_src << " and dst is: " << v_dst <<std::endl;
        //std::cout << "src is type: " << typeid(v_src).name() << " and dst is: " << typeid(v_dst).name() <<std::endl;

        }

    }

return 0;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//FUNCTIONS
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*The following function takes in a string, extracts numbers using regex, converts to integer and appends to a vector*/

std::vector <int> getNumbers(std::string str){
 
std::vector <int> my_vector;
std::regex regexRule ("-?[0-9]+");  //this regex rule extracts all numbers with a separation (comma in our case) between them
std::sregex_iterator currentMatch(str.begin(), str.end(), regexRule);
std::sregex_iterator lastMatch;

while(currentMatch != lastMatch){
    std::smatch match = *currentMatch;
    //std::cout << match.str() << "\n"; //prints out all numbers
    my_vector.push_back(stoi(match.str()));
    currentMatch++;
    }
return my_vector;    
}


// a fucntion to print vectors for checking

void print(std::vector<int> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ", ";
        //std::cout << typeid(input.at(i)).name() << "\n";
    }
}

void print_s(std::vector<int> const &input)
{
    if (input.size() == 0){
        std::cout << "Error: No path found\n";
    }

    else{
        for (int i = input.size()-1; i > 0 ; i--) {
            std::cout << input.at(i) << "-";
            //std::cout << typeid(input.at(i)).name() << "\n";
        }
        std::cout << input.at(0) <<std::endl;
    }
}



void shortest_path(int max_vert, std::vector <std::vector <int>> edge_vector, int source, int dest){

std::vector <Node> graph;
for (int i = 1; i <= max_vert; i++){
    graph.push_back(Node(i, max_vert));
}



 

/*starting a queue with source node in graph vector as my first Node in queue 
(indices start at zero, vertices start at 1 hence the -1) - remember node needs max vert to reserve memory*/

// construdting full graph by definining neighbors
for (std::vector <int> i : edge_vector){

    Node* first_node = &graph[i[0] - 1]; //first element of queue
    Node* second_node = &graph[i[1]-1];
    first_node->node_neighbors.push_back(second_node);
    second_node->node_neighbors.push_back(first_node);
}

std::queue <Node*> myqueue; //we will put all nodes in this queue starting at the source and start checking its neighbors till we reach destination
Node* source_node = &graph[source - 1];
myqueue.push(source_node);

bool dest_found = false;

while(!myqueue.empty() && dest_found == false){
    Node* mycurrentnode = myqueue.front();
    myqueue.pop();

    for (Node* i : mycurrentnode->node_neighbors){

        if (i->node_value == dest){
            i->parent = mycurrentnode;
            dest_found = true;
            break;
        }
        
        else if (i->visited == false){
            i->visited = true;
            myqueue.push(i);
            i->parent = mycurrentnode;
        }
    }
}

std::vector <int> sp_vector;

Node* sp_node = &graph[dest-1];

    if (!sp_node->parent){
        print_s(sp_vector);
        }

    else{
        sp_vector.push_back(sp_node->node_value);

            while (sp_node->node_value != source){
            sp_vector.push_back(sp_node->parent->node_value);
            sp_node = sp_node->parent;
                }

    print_s(sp_vector);
    }
}