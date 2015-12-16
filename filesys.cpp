#include "cfilesys"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>


char FileSystem::encrypt(char plain){

	char  key = 0x4B;


	char  cypher = plain;

	cypher ^= key;


	return cypher;
}

char FileSystem::decrypt(char cypher){

	return encrypt(cypher);
}


int FileSystem::searchDir(std::string name){

	for(int i = 0; i < DIRSIZE; i++){

		if((strcmp(dir[i].name, name.c_str()) == 0) && (dir[i].start != -1))
			return i;
	}

	return -1;
}


int FileSystem::availEntry(){

	int i;

	for(i = 0; i < DIRSIZE; i++){

		if(dir[i].start == -1)
			break;
	}

	if(i == DIRSIZE)
		return -1;
	else
		return i;
}


int FileSystem::allocFAT(int size, int &len, int (&fat_i)[FATSIZE]){

	len = 0;

	int  j;
	int  i = 0;

	for(j = 0; j < FATSIZE; j++){

		if(size > 0){

			if(fat[j] == -2){

				fat_i[i++] = j;
				len++;

				size -= BLOCKSIZE;
			}

		} else
			break;
	}


	if(j == FATSIZE)
		return -1;

	else
		return 0;
}

int FileSystem::import(std::string source, std::string dest){

	std::ifstream  file(source.c_str());

	if(!file.good())
		return -1;


	if(searchDir(dest) != -1)
		return -2;


	int  dir_i = availEntry();

	if(dir_i == -1 )
		return -3;

	else{

		std::string  contents(std::istreambuf_iterator<char>(file), 
				      (std::istreambuf_iterator<char>()));


		int  fat_i[FATSIZE];
		int  len;

		int alloc = allocFAT((int)strlen(contents.c_str()), len, fat_i);

		if(alloc == -1)
			return -4;

		else{

			int  offs;

			for(int i = 0; i < len; i++){

				fat[fat_i[i]] = fat_i[i+1];

				offs = (i*BLOCKSIZE);

				contents.copy(block[fat_i[i]], BLOCKSIZE, offs);
			}

			fat[fat_i[len-1]] = -1;


			strcpy(dir[dir_i].name, dest.substr(0, FILELENGTH).c_str());

			dir[dir_i].size = (int)strlen(contents.c_str());

			dir[dir_i].start = fat_i[0];


			file.close();


			return 0;
		}
	}
}


int FileSystem::copy(std::string source, std::string dest){

	int  source_i = searchDir(source);

	if(source_i == -1)
		return -1;


	if(searchDir(dest) != -1)
		return -2;


	int  dir_i = availEntry();

	if(dir_i == -1)
		return -3;

	else{

		std::string  contents;

		contents.append(block[dir[source_i].start], BLOCKSIZE);


		source_i = fat[dir[source_i].start];

		while(source_i != -1){

			contents.append(block[source_i], BLOCKSIZE);

			source_i = fat[source_i];
		}


		int  fat_i[FATSIZE];
		int  len;

		int alloc = allocFAT((int)strlen(contents.c_str()), len, fat_i);

		if(alloc == -1)
			return -4;

		else{

			int  offs;

			for(int i = 0; i < len; i++){

				fat[fat_i[i]] = fat_i[i+1];

				offs = (i*BLOCKSIZE);

				contents.copy(block[fat_i[i]], BLOCKSIZE, offs);
			}

			fat[fat_i[len-1]] = -1;


			strcpy(dir[dir_i].name, dest.substr(0, FILELENGTH).c_str());

			dir[dir_i].size = (int)strlen(contents.c_str());

			dir[dir_i].start = fat_i[0];


			return 0;
		}
	}
}


int FileSystem::outport(std::string source, std::string dest){

	int  source_i = searchDir(source);

	if(source_i == -1)
		return -1;


	std::ifstream  input(dest.c_str());

	if(input.good()){

		input.close();

		return -2;


	}else{

		std::string  contents;

		contents.append(block[dir[source_i].start], BLOCKSIZE);


		int  fat_i = fat[dir[source_i].start];

		while(fat_i != -1){

			contents.append(block[fat_i], BLOCKSIZE);

			fat_i = fat[fat_i];
		}

		contents = contents.substr(0, dir[source_i].size);


		std::ofstream  output(dest.c_str());

		output << contents;

		output.close();


		return 0;
	}
}

int FileSystem::remove(std::string name){

	int dir_i = searchDir(name);

	if(dir_i == -1)
		return -1;


	int fat_i = dir[dir_i].start;

	while(fat_i != -1){

		for(int j = 0; j < BLOCKSIZE; j++){

			block[fat_i][j] = 0;
		}


		int tmp = fat[fat_i];

		fat[fat_i] = -2;

		fat_i = tmp; 
	}


	dir[dir_i].start = -1;


	return 0;
}


void FileSystem::format(){

	for(int i = 0; i < FATSIZE; i++){

		for(int j = 0; j < BLOCKSIZE; j++){

			block[i][j] = 0;
		}
	}

	for(int i = 0; i < DIRSIZE; i++)
		dir[i].start = -1;

	for(int i = 0; i < FATSIZE; i++)
		fat[i] = -2;
}


int FileSystem::load(){

	std::ifstream  input("master.mfs", std::ios::binary);


	char  *buffer = new char[sizeof(FileSystem)];

	if(input.good()){

		input.read(buffer, sizeof(FileSystem));
		input.close();
	} else
		return -1;

	for(int i = 0; i < sizeof(FileSystem); i++)
		buffer[i] = decrypt(buffer[i]);


	std::ofstream  output("master.mfs", std::ios::binary);

	output.write(buffer, sizeof(FileSystem));
	output.close();

	std::ifstream  d_input("master.mfs", std::ios::binary);

	if(d_input.good()){

		d_input.read((char*)this, sizeof(FileSystem));
		d_input.close();

		return 0;

	} else
		return -1;
}


void FileSystem::save(){

	std::ofstream  output("master.mfs", std::ios::binary);

	output.write((char*)this, sizeof(FileSystem));
	output.close();

	std::ifstream  input("master.mfs", std::ios::binary);


	char  *buffer = new char[sizeof(FileSystem)];

	if(input.good()){

		input.read(buffer, sizeof(FileSystem));
		input.close();
	}

	for(int i = 0; i < sizeof(FileSystem); i++)
		buffer[i] = encrypt(buffer[i]);


	std::ofstream  e_output("master.mfs", std::ios::binary);

	e_output.write(buffer, sizeof(FileSystem));
	e_output.close();
}
