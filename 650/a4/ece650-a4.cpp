//to compile, put minisat in assignment folder run with cmake:   mkdir build cd build cmake ../ make ./ece650-a4.cpp
#include <iostream>
#include <vector>
#include <regex>
#include "minisat/minisat/core/SolverTypes.h"
#include "minisat/minisat/core/Solver.h"
#include <sstream>
#include <fstream>
#include <string.h>


std::vector <int> getNumbers(std::string str);
void print(std::vector<int> const &input); // a simple function to print vertices for testing - delete it before push
void min_vertex_cover(int k, std::vector <std::vector <int>> edges);



int main(int argc, char **argv){

    std::vector <std::vector <int>> edges_vector; 
    //the edges_vector is a vector of edges (edges themselves are a vector with 2 items in it, the source vertex and the dest vertex)
    
    while (!std::cin.eof()) {
    
        std::string line;  //an empty string variable to take line input

        std::getline(std::cin, line);
    
    std::vector <int> my_returned_vector = getNumbers(line);  //extracts all numbers from input command into a vector
    
    int max_vertices;
    bool e_valid = false;

    
    //if the input is V command
    if (line[0] == 'V'){
        if (my_returned_vector.empty() || my_returned_vector[0] <1){
            std::cerr << "Error: Vertices are undefined or less than 1" << std::endl;
        }
        else{
            max_vertices = my_returned_vector[0];
            edges_vector.clear();
        }
    }



    //if E command is given, check if all E values are <= V, if yes save to edges_vector
    else if (line[0] == 'E'){
        if (my_returned_vector.empty()){
            std::cerr <<"Error: No Edges entered"<<std::endl;
        }
        edges_vector.clear(); //clear any previously stored vector
        for(int i : my_returned_vector){ //iterate through all elements of my_returned_vector

            if (i > max_vertices || i <= 0){
                e_valid = false;
                max_vertices = 0; //reset the V command
                std::cerr << "Error: Edges cannot have nodes as zero, -ve or greater than largest vertex. You must re-enter both V and E commands" << std::endl;
                break;
                }

            else if (i < max_vertices){
                e_valid = true;
                }    
            }

            if (e_valid == true){
                //make your edges_vector = my_returned_vector;
                int size = my_returned_vector.size();

                for (int i = 0; i < size; i = i+2){     //to make a vector of vectors
                    edges_vector.push_back({my_returned_vector[i],my_returned_vector[i+1]});
                }

            }  

        //call min_vertex_cover function with max_vert as k and edges_vector 
        if (e_valid){
            min_vertex_cover(max_vertices,edges_vector);
        }     


        //some print statements                                                                     DELETE COMMENT THIS AND 2 LINE BELOW
        /*std::cout<< "Maximum no of vertices is: "<<max_vertices <<"\n";
        std::cout << "the edges vector is: <";                                                           
        for (std::vector i : edges_vector){
            std::cout << "<" << i[0] << "," << i[1] << ">";
            //print(i);
        }
        std::cout<<">"<< std::endl;*/
           
        //std::cout << "i = " << i << std::endl;
        //std::cout << typeid(i).name() << '\n';
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


// a fucntion to print vectors for checking - stackoverflow

void print(std::vector<int> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ", ";
        //std::cout << typeid(input.at(i)).name() << "\n";
    }
}

//function for finding min vertex cover and printing out

void min_vertex_cover(int max_k, std::vector <std::vector <int>> edges){
    
    if (edges.empty()) {
        std::cerr << "Error: No Edges Entered, Vertex Cover = {}" << std::endl;
        return;
    } 

    int curr_k = 1;

    while (curr_k < max_k){
        //std::cout<<"Current k is :"<<curr_k<<std::endl;

        Minisat::Solver solver;

        //The reduction uses n × k atomic propositions
        // create max_k positive literals over prop_size variables
        std::vector <std::vector<Minisat::Lit>> vec_vec_lit(max_k);
        
        for (int n = 0; n < max_k; n++) 
        {
            for (int k = 0; k < curr_k; k++) 
            {
                Minisat::Lit literal = Minisat::mkLit(solver.newVar());
                vec_vec_lit[n].push_back(literal);
            }
        }

        //At least one vertex is the ith vertex in the vertex cover:
        //a clause (x1,i ∨ x2,i ∨ · · · ∨ xn,i)
        for (int k = 0; k < curr_k; k++) 
        {
            Minisat::vec<Minisat::Lit> literals;
            for (int n = 0; n < max_k; n++) 
            {
                literals.push(vec_vec_lit[n][k]);
            }
            solver.addClause(literals);
            literals.clear();
        }


        //No one vertex can appear twice in a vertex cover
        //a clause (¬xm,p ∨ ¬xm,q)

        for (int m = 0; m < max_k; ++m) 
        {
            for (int p = 0; p < curr_k - 1; p++) 
            {
                for (int q = p + 1; q < curr_k; q++) 
                {
                    solver.addClause(~vec_vec_lit[m][p], ~vec_vec_lit[m][q]); 
                }
            }
        }

        

      //No more than one vertex appears in the mth position of the vertex cover. (¬xp,m ∨ ¬xq,m)
        for (int m = 0; m < curr_k; ++m) 
        {
            for (int p = 0; p < max_k - 1; ++p) 
            {
                for (int q = p + 1; q < max_k; ++q) 
                {
                    solver.addClause(~vec_vec_lit[p][m], ~vec_vec_lit[q][m]);
                }
            }
        }      


        
        // Every edge is incident to at least one vertex in the vertex cover. (xi,1 ∨ xi,2 ∨ · · · ∨ xi,k ∨ xj,1 ∨ xj,2 ∨ · · · ∨ xj,k)
        for (int i = 0; i<edges.size();i++){
            Minisat::vec<Minisat::Lit> literals;
            for (int k = 0; k < curr_k; k++) 
            {
                literals.push(vec_vec_lit[(edges[i][0]-1)][k]);
                literals.push(vec_vec_lit[(edges[i][1]-1)][k]);
            }
            solver.addClause(literals);
            literals.clear();

        }

    
        bool res = solver.solve();

        if (res) 
        {
            std::vector<int> covers;
            for (int i = 0; i < max_k; i++) 
            {
                for (int j = 0; j < curr_k; ++j) 
                {
                    if (Minisat::toInt(solver.modelValue(vec_vec_lit[i][j])) == 0) 
                    {
                        covers.push_back(i+1);
                    }
                }
            }
            
        //print all v covers
            for(auto i: covers){
                std::cout<<i <<" ";
            };
            std::cout<<""<<std::endl;
            break;
        } 
        

    curr_k++;
    }
 

}