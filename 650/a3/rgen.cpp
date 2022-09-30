#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>

int random_number (int min, int max);
std::string randomStreetNameGen();
void generateStreetNamesVector(int s_range, std::vector<std::string> &street_name_vec);

class Point {
public:    
    int X;
    int Y;

    Point(int x, int y){
        X = x;
        Y = y;
    }
};

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// A C++ program to check if two given line segments intersect
// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
//adapted from program at https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/?ref=lbp

bool onSegment(Point p, Point q, Point r)
{
    if(p.X == q.X && p.X == r.X){ //line is horizontal
        if (q.Y < std::max(p.Y, r.Y) && q.Y > std::min(p.Y, r.Y)){return true;}
    }

    if (p.Y == q.Y && p.Y == r.Y){
        if (q.X < std::max(p.X, r.X) && q.X > std::min(p.X, r.X)){return true;}
    }

	if (q.X < std::max(p.X, r.X) && q.X > std::min(p.X, r.X) &&
		q.Y < std::max(p.Y, r.Y) && q.Y > std::min(p.Y, r.Y))
	return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (q.Y - p.Y) * (r.X - q.X) -
			(q.X - p.X) * (r.Y - q.Y);

	if (val == 0) return 0; // collinear

	return (val > 0)? 1: 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(Point p1, Point q1, Point p2, Point q2, bool ss)
{
	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);
    
    //std::cout << "o1: " << o1 << ", o2: " << o2 << ", o3: " << o3 << ", o4: " << o4 << std::endl;
    //std::cout << "p1.X: " << p1.X << ", p1.Y: " << p1.Y << ", q1.X: " << q1.X << ", q1.Y: " << q1.Y <<std::endl;
    //std::cout  << "p2.X: " << p2.X << ", p2.Y: " << p2.Y << ", q2.X: " << q2.X << ", q2.Y: " << q2.Y << std::endl;
	// General case
	if ((o1 != o2) && (o3 != o4)){ 
        if (ss == true && !(q1.X == p2.X && q1.Y == p2.Y)){
            return false;
        }else{
		    return true;
        }
    }
	// Special Cases
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return false;

	// p1, q1 and q2 are collinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1) ) return false;

	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return false;

	// p2, q2 and q1 are collinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return false;

	return true; // Doesn't fall in any of the above cases
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/



std::string randomStreetNameGen(){
    // Returns a string of random alphabets of
    // length of string fixed at n = 20.
    //the alphabet array has a size of 26 (max alphabets)

    const int MAX = 26;
    int n = 20;
    int seed = random_number(0,1000000000);
    srand(seed); //we set a random seed for our rand function to ensure we have unique string everytime

    char alphabet[MAX] = {'A', 'B', 'C', 'D', 'E', 'F', 'G',
                          'H', 'I', 'J', 'K', 'L', 'M', 'N',
                          'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                          'V', 'W', 'X', 'Y', 'Z'};
 
    std::string res = "";
    for (int i = 0; i < n; i++)
        res = res + alphabet[rand() % MAX];
     
    return res;
}


void generateStreetNamesVector(int s_range, std::vector<std::string> &street_name_vec){
    //the function doesnt return anything, it pushbacks new names into our street_names vector, we pass in address of that vector to this fcn
    //we pass it our randomly generated number of streets integer as well

    for (int i = 0; i < s_range; i++){
        street_name_vec.push_back(randomStreetNameGen());
    }
}


int random_number (int min, int max){

    //THIS CODE FOR RANDOM NUMBER GENERATION FROM DEV/URANDOM IS TAKEN FROM ADVANCED LINUX PROGRAMMING LISTING 6.1
    /* Return a random integer between MIN and MAX, inclusive. Obtain
    randomness from /dev/random. */
    

        /* Store a file descriptor opened to /dev/random in a static
        variable. That way, we don’t need to open the file every time
        this function is called. */
    static int dev_random_fd = -1;
    char* next_random_byte;
    int bytes_to_read;
    unsigned random_value;

        /* Make sure MAX is greater than MIN. */
    assert (max >= min);

        /* If this is the first time this function is called, open a file
        descriptor to /dev/random. */
    if (dev_random_fd == -1) {
        dev_random_fd = open ("/dev/random", O_RDONLY);
        assert (dev_random_fd != -1);
    }
        /* Read enough random bytes to fill an integer variable. */
    next_random_byte = (char*) &random_value;
    bytes_to_read = sizeof (random_value);
        /* Loop until we’ve read enough bytes. Because /dev/random is filled
        from user-generated actions, the read may block and may only
        return a single random byte at a time. */
    do {
        int bytes_read;
        bytes_read = read (dev_random_fd, next_random_byte, bytes_to_read);
        bytes_to_read -= bytes_read;
        next_random_byte += bytes_read;
    } while (bytes_to_read > 0);

    /* Compute a random number in the correct range. */
    return min + (random_value % (max - min + 1));
}


int main (int argc, char **argv){
    /*code for using getopt adapted from 
    https://www.youtube.com/watch?v=SjyR74lbZOc&ab_channel=theurbanpenguin */
    
    opterr = 0; //this keep getopt from printing its own error statements

    unsigned int s_val = 10, n_val = 5, l_val = 5, c_val = 20;
    int option;
    std::vector<std::string> street_names; // this vector will store all street names
    //we will only combine street names with a street (points) during print out

    std::vector <std::vector <Point>> streets; 
    //this is a vector, that contains vectors of points

    while((option = getopt(argc, argv, "s:n:l:c:")) != -1){
        switch (option) {
            case 's' :
                /*optarg will retrieve char, we need to convert it to int using 
                atoi and save to s_val*/
                s_val = atoi(optarg);
                if (s_val<2){
                    std::cerr<<"Error: Number of streets must be 2 or more"<<std::endl;
                    exit(1);
                }
                break;

            case 'n' :
                n_val = atoi(optarg);
                if (n_val<1){
                    std::cerr<<"Error: Number of line segments must be 1 or more"<<std::endl;
                    exit(1);
                }
                break;  

            case 'l' :
                l_val = atoi(optarg);
                if (l_val<5){
                    std::cerr<<"Error: Wait time must be 5 seconds or more"<<std::endl;
                    exit(1);
                }
                break;

            case 'c' :
                c_val = atoi(optarg);
                if (c_val<1){
                    std::cerr<<"Error: Value for -c should be 1 or more"<<std::endl;
                    exit(1);
                }
                break;    

            case '?':
            std::cerr << "Error: Incorrect option or missing values for -" << char(optopt) << std::endl;
            exit(1);
        }
    }

/*-------------------------------------------------------------------------------------------------------------------------------*/   

while (true){ //program runs forever after receiving getopt() inputs
    
    //We generate a random number of streets s_rand from the range specified by -s k (range is [2,k]), 
    int s_rand = random_number(2, s_val); 
    //std::cout<< "s_rand is: " << s_rand <<std::endl;

    //Now we generate s_rand number of streets
    for (int i = 0; i < s_rand; i++){
        //std::cout << "street: " << i << " | " <<std::endl;
        
        //each street is a vector of multiple point, where total points = line segments (n_rand +1)
        int n_rand = random_number(1, n_val);                                                                     //PLEASE UNCOMMENT _ YY LOVE
        //int n_rand = 4;
        //std::cout<< "n_rand is: " << n_rand <<std::endl;
        int bad_point_counter = 0;

        std::vector<Point> temp_points;
        for (int j = 0; j < n_rand +1 ; j++){

            //std::cout << "Point: " << j << " | " <<std::endl;
            //generate a point (x,y) where x is between [-c_val and +c_val]
            Point p = Point (random_number(-c_val, c_val),random_number(-c_val, c_val));
            
 /*        int xutt;                //uncomment this to pass in your own coordinates for testing
            int yutt;
            std::cin >> xutt;
            std::cin >> yutt;
            Point p = Point (xutt,yutt); */

            bool point_ok = true; //a flag to check if this is a good point

            
            //TESTS for our point
            
            if (!temp_points.empty()){ //if temp_points already has points in it

                    //check if p already exists in temp_point before push back
                    for (int it=0; it < temp_points.size(); it++){
                        if (temp_points[it].X == p.X && temp_points[it].Y == p.Y){
                            point_ok = false;
                            bad_point_counter++;
                            if (bad_point_counter > 24){
                                std::cerr << "Error: Failed to successfully generate point for 25 tries - exit with error (1)" <<std::endl;
                                exit(1);
                            }
                            //std::cout << bad_point_counter << " bad point counter\n";
                            j--;
                            break;
                        }
                    }
                //if temp_points has one or more line seg (2 or more points) - check if current point and last point
                // (they make a segment) intersect with other seg in same street or onSegment
                int t_size = temp_points.size();
                if (t_size > 2){
                    for (int ir = 0; ir < t_size-1; ir++){
                        if (!doIntersect(temp_points[ir],temp_points[ir+1],temp_points[t_size-1],p, true)){
                            //std::cout << "bad point detected 2";
                            point_ok = false;
                            bad_point_counter++;
                            if (bad_point_counter > 24){
                                std::cerr << "Error: Failed to successfully generate point for 25 tries - exit with error (1)" <<std::endl;
                                exit(1);
                            }
                            //std::cout << bad_point_counter << " bad point counter\n";
                            j--;
                            break;
                        }

                    }
                }

                //check overlap with other streets
                int s_size = streets.size();
                if (s_size > 0){
                    /* std::cout << "checking with other streets"<<std::endl;           //SOME CODE TO PRINT TEST STATEMENT
                    for (auto i: streets){ //print street vecctors
                        std::cout << "Printing next street:\n";
                        for (auto j: i){
                        std::cout << "("<<j.X<<","<<j.Y<<")"<<std::endl;
                        }
                        } */

                    for (int is = 0; is < s_size; is++){
                        int is_size = streets[is].size(); //size of individual street vector
                        for (int ip = 0; ip < is_size-1; ip++){
                            if (!doIntersect(streets[is][ip],streets[is][ip+1],temp_points[t_size-1],p, false)){
                            //std::cout << "bad point detected other street";
                            point_ok = false;
                            bad_point_counter++;
                            if (bad_point_counter > 24){
                                std::cerr << "Error: Failed to successfully generate point for 25 tries - exit with error (1)" <<std::endl;
                                exit(1);
                            }
                            //std::cout << bad_point_counter << " bad point counter\n";
                            j--;
                            break;

                            }
                        }
                    }

                }
                

            }


            if (point_ok){ //if temp_points is empty (first point) add point always
                temp_points.push_back(p);
                bad_point_counter = 0;
            }



            //temp_points.push_back(p); 
            //first check - whether current point == previous temp point - if point rejected use continue
            //second check - self looping, minimum 3 points in temp_points vector - check if cross same line - geeksforgeeks
            //third check - when you have atleast one street - see paint
            //for every rejected point - increase counter by 1 - if counter reaches 25 exit(1);
        }

        streets.push_back(temp_points);
        temp_points.clear();

        //std::cout <<"\n\n" <<std::endl;
    }


/*--------------------------------------------------------------------------------------------------------------------------------*/    
    //issue "rm" commands if street_names vector is not empty
    if(!street_names.empty()){
        for (auto i: street_names){
            std::cout << "rm " <<'"'<< i << '"' <<std::endl;
        }
        street_names.clear(); //clear this vector for next iteration
    }
    
    
    //we then generate s_rand no of street names and pushback to vector street_names
    generateStreetNamesVector(s_rand, street_names);


    //print the street_names vector                                                             //DELETE THIS PRINT LINE
/*     std::cout<<"\nNew streets generated:\n";
    for (auto i: street_names){
        std::cout << i << ' ';
        } std::cout<< '\n'; */

/*--------------------------------------------------------------------------------------------------------------------------------*/
// with streets and street_names vector ready, issue add statements in form add "streetname" (x,y) (x,y)

    for (int itr = 0; itr < s_rand; itr++){
        std::cout <<"add " <<'"' <<street_names[itr]<<'"'<<" ";
        for (auto jtr : streets[itr]){
            std::cout << "("<<jtr.X<<","<<jtr.Y<<") ";
        }
        std::cout<<""<<std::endl;
    }

    std::cout << "gg" <<std::endl;

/*----------------------------------------------------------------------------------------------------------------------------------*/
    //make system wait for l_rand seconds:
    int l_rand = random_number(5, l_val);
    //std::cout <<"Waiting for l_rand seconds: "<<l_rand <<std::endl;
    streets.clear(); //clear the streets vector for next iteration
    sleep(l_rand);

    

    } //closing for while(true)
        
    return 0;
}
