#include <iostream> //std::cout, std::cerr (error printing)
#include <fstream> //std::ifstream --> INPUT file stream, read a file. std::ofstream --> WRITE to a file
#include <string> //std::string std::stoi, std::stof
#include <vector> //std::vector --> self-regulating array
#include <iomanip> //std::setprecision,manipulation of varaibles/data
#include "pixel.h" //our custom header

void average_colors(std::vector<Pixel> &pixel_list);
void flip_vertical(std::vector<Pixel> &pixel_list);

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		std::cerr << "Erroneous entry; please use: " << argv[0] << " <input filename>\n";
		return 1;
	}

	std::string filename = argv[1]; //copy user-entered string <filename> and store in a string object. this object will be used to name and create an input file below
	std::ifstream infile(filename); //input filestream object called infile, open it
	if(!infile.is_open()) //if is_open is FALSE, the input file wasn't opened correctly
	{
		std::cerr << "Could not open " << filename << ".\n"; //print error to screen; couldn't find/open
		return 1;
	}
	
	std::vector<Pixel> pixel_list; //array of pixels, grows automatically. <Pixel> determines the TYPE of data stored in the array
	std::string line; //grabs one text line at a time. each one overwrites the next. "Variable declared OUTSIDE LOOP! getline NEEDS an object to WRITE TO. When it can't write to something, it returns a fail flag essentially. Line is also used in our while loop condition."
	
	while(std::getline(infile, line)) //getline returns false when no lines left to read, and loop ends.
	{
		if(line.empty()) //"<------------------------------------------------------------------------------------------------"
			continue; //for trailing newlines
		int c1 = line.find(','); //
		int c2 = line.find(',', c1 + 1); //start new search on a new interval immediately AFTER c1 is found. the plus one ensures we start AFTER the comma we already found. 
		int c3 = line.find(',', c2 + 1); //these variables store the DISTANCE/difference between each comma. it varies per pixel. 
		int c4 = line.find(',', c3 + 1);


		std::string x_chunk = line.substr(0, c1); //these strings are ALL overwritten each pass of the loop. Their values are tokenized/converted to integers or floats, and pushed to FIELDS of a new pixel p, using p.x, p.y, etc.
		std::string y_chunk = line.substr(c1 + 1, c2 - c1 - 1); //substr takes two parameters: "Start", and "Length". 
		std::string r_chunk = line.substr(c2 + 1, c3 - c2 - 1); //Start right AFTER the comma. Subtract to get the NUMBER of characters between commas, then subtract 1 to land on the right index. substr copies THAT CHUNK of data, and it self-sizes with each loop. 
		std::string g_chunk = line.substr(c3 + 1, c4 - c3 - 1);
		std::string b_chunk = line.substr(c4 + 1); //follows through to end
		
		//conversion from text to number values
		Pixel p; //store in p of type pixel
		p.x = std::stoi(x_chunk); //convert x and y to ints
		p.y = std::stoi(y_chunk); //
		p.r = std::stof(r_chunk); //convert R B G to doubles or floats.
		p.g = std::stof(g_chunk);
		p.b = std::stof(b_chunk);

		pixel_list.push_back(p); //adds each pixel (basically a completed struct with pixel's data) to pixel list.
	}

	infile.close(); //close the file (pixel.dat)
	
	std::cout << "Read " << pixel_list.size() << " pixels\n"; //prints number of recorded pixels to screen
	
	average_colors(pixel_list); //function calls
	flip_vertical(pixel_list); //>>>>
	
	std::ofstream outfile("flipped.dat"); //create flipped.dat file. ofstream CREATES outfile with the name applied, and OPENS it simultaneously. it STAYS open from there until closed. 
	if(!outfile.is_open())
	{
		std::cerr << "Could not create file.\n"; //"std::cerr" STILL prints to screen during a crash/event, because it is UNBUFFERED. Using cerr also keeps errors out of data for running grep/reports later. 
		return 1;
	}

	outfile << std::setprecision(9); //outfile << (command) essentially pushes a setting to our output stream. in this case, we set precision to 9 significant figures to ensure all data is captured. "MUST BE DONE BEFORE FOR LOOP, or none of it will apply to the data we write."
	
	int count = pixel_list.size(); //pre-loop size counter
	for(int i = 0; i < count; i++) 
	{
		outfile << pixel_list[i].x << "," << pixel_list[i].y << "," << pixel_list[i].r << "," << pixel_list[i].g << "," << pixel_list[i].b << "\n"; //writing flipped.dat using now-modified pixel.dat data. This is also where we add all of the commas back in, though it's much easier because all we need to do is insert one in between each data field. 
	}
	outfile.close(); //close file. don't have to really, program end will also close the file. unless there's some other reason I don't know about?
	std::cout << "Flipped.dat created.\n"; //verification prompt. 

	return 0;
}

void average_colors(std::vector<Pixel> &pixel_list) //vector of type PIXEL, taking pixel_list by REFERENCE. must pass pixel_list by REFERENCE in order for changes to me made (and saved) to our original data extracted from pixel.dat. Remove the "&", and the function modifies a copy and destroys it. this is more relevant for flip_vertical than average_colors, but it STILL avoids making a total copy of pixel.dat and wasting resources. 
{
	int count = pixel_list.size(); //int count set equal to .size() return on pixel_list
	double total_r = 0; //sum accumulators
	double total_g = 0;
	double total_b = 0;
	
	for(int i = 0; i < count; i++)
	{
		total_r += pixel_list[i].r; //for loop sums each field
		total_g += pixel_list[i].g;
		total_b += pixel_list[i].b;
	}

	std::cout << "Avg r-value: " << total_r/count << "\n"; //each field sum divided by number of indices in pixel_list
	std::cout << "Avg g-value: " << total_g/count << "\n"; 
	std::cout << "Avg b-value: " << total_b/count << "\n"; 
}

void flip_vertical(std::vector<Pixel> &pixel_list)
{
	std::vector<Pixel> original = pixel_list; //copy list before manipulating. from here, flip_vertical reads from the copy of original while it overwrites pixel_list permanently, which main then loops through to write flipped.dat.  
	int count = pixel_list.size();

	for(int i = 0; i < count; i++) //for loop sandwhiches pixel list from top to bottom. x stays the same, but y flips from the outside-in. vv
				       //													--
				       //													^^
	{
		int x = pixel_list[i].x;
		int y = pixel_list[i].y;

		int swap_y = 255 - y; //image has 256 rows, indexed from 0 to 255. starting from max length and subtracting by increments of 1, 1 per loop.
		int swap_index = x * 256 + swap_y; //this gets big fast, but there are over a hundred thousand pixels to write, so it makes sense. 

		pixel_list[i].r = original[swap_index].r; //x and y are arbitrary. They're positions, not data. R G B are the values that truly get moved around/flipped. 
		pixel_list[i].g = original[swap_index].g;
		pixel_list[i].b = original[swap_index].b;
	}
}



